#include "script.h"
#include <common/exception.h>
#include <core/default_filesystem.h>
#include <script/ast/ast_generator.h>
#include <script/ast/gsc_writer.h>
#include <script/ast/recursive_visitor.h>
#include <script/ast/type_visitor.h>
#include <script/compiler/compiler.h>
#include <script/compiler/exception.h>
#include <script/stockfunctions.h>
#include <script/vm/types.h>
#include <script/vm/virtual_machine.h>
#include <set>
#include <variant>

namespace script
{
	class ReferenceSolver
	{
		std::string path_base;
		std::string m_filename;
		const std::string& m_library_path;
		script::ReferenceMap& m_global_reference_map;

	  public:
		ReferenceSolver(filesystem_api &fs, const std::string path_base_, const std::string file_name_,
						script::ReferenceMap& global_reference_map, const std::string& library_path)
			: m_global_reference_map(global_reference_map),
			  path_base(path_base_),
			  m_filename(file_name_),
			  m_library_path(library_path)
		{
			// first get all the functions that we reference
			script::ast::ASTGenerator m_generator;
			bool ret = false;
			//if (m_filename.find('/') == std::string::npos)
				//ret = m_generator.generate(fs, m_library_path, m_library_path + m_filename + ".gsc");
			//else
				ret = m_generator.generate(fs, path_base, path_base + m_filename + ".gsc");
			if (!ret)
			{
				throw script::compiler::CompileException("Failed to read file {}, {}", m_filename,
														 path_base + m_filename + ".gsc");
				//LOG_ERROR("Failed to read file '%s'\n", m_filename.c_str());
			}
			script::LoadedProgramReference& lpr = m_global_reference_map[file_name_];
			lpr.program = std::move(m_generator.root());
			lpr.name = m_filename;

			script::ast::NodeTypeVisitor<script::ast::FunctionDeclaration> ntv;
			auto& fun = ntv.find(lpr.program.get());
			for (auto* f : fun)
			{
				lpr.function_map[util::string::to_lower(f->function_name)] = f;
			}

			// lpr.m_parent_visitor.visit_node(*lpr.program.get());

			script::compiler::FunctionCallReferenceVisitor fcrv(m_filename);
			fcrv.visit_node(*lpr.program.get());

			// add the references found to the global references
			// not using it atm, we just load and keep the whole tree, but was maybe nice to only specific functions
			for (auto& pair_ : fcrv.references())
			{
				// for (auto & function_ : pair_.second)
				// m_global_reference_map[pair_.first].insert(function_);
				// if (pair_.first != m_filename)
				if (m_global_reference_map.find(pair_.first) == m_global_reference_map.end())
				{
					ReferenceSolver solver(fs, path_base_, pair_.first, global_reference_map, m_library_path);
				}
			}
		}
	};

	ScriptEngine::ScriptEngine(filesystem_api &fs) : m_fs(fs)
	{
	}
	ScriptEngine::~ScriptEngine()
	{
	}

	void ScriptEngine::run()
	{
		if (!m_vm)
			return;
		//if (m_vm->thread_count() == 0)
			//return;
		try
		{
			m_vm->run();
		}
		catch (vm::Exception& ex)
		{
			m_vm->dump(m_vm->get_last_thread());
			auto* lt = m_vm->get_last_thread();
			if (lt)
			{
				while (!lt->function_name_stack.empty())
				{
					auto& it = lt->function_name_stack.top();
					printf("->%s\n", it.c_str());
					lt->function_name_stack.pop();
				}
			}
			auto& dbg = m_vm->get_debug_info();
			LOG_ERROR("Script Error: [%s:%s:%d] '%s' [%s]\n", dbg.file.c_str(), dbg.function.c_str(), dbg.line, ex.what(), dbg.expression_string.c_str());
			m_vm.reset();
		}
	}

	void ScriptEngine::register_function(const std::string name, StockFunction sf)
	{
		auto fnd = m_registeredfunctions.find(name);
		if (fnd != m_registeredfunctions.end())
		{
			LOG_ERROR("function '%s' already defined", name.c_str());
			return;
		}
		m_registeredfunctions[name] = sf;
	}

	void ScriptEngine::notify(vm::ObjectPtr object, const std::string str, std::vector<vm::Variant>* args)
	{
		if (!m_vm)
			return;
		try
		{
			m_vm->notify_event_string(object, str, args);
		}
		catch (vm::Exception& ex)
		{
			LOG_ERROR("Script Error: %s\n", ex.what());
			m_vm.reset();
		}
	}

	void ScriptEngine::execute_thread(vm::ObjectPtr object, const std::string file, const std::string function, size_t nargs)
	{
		if (!m_vm)
			return;
		try
		{
			m_vm->exec_thread(nullptr, object ? object : m_vm->get_level_object(), file, function, nargs, object ? true : false);
		}
		catch (vm::Exception& ex)
		{
			LOG_ERROR("Script Error: %s\n", ex.what());
			m_vm.reset();
		}
	}
	bool ScriptEngine::load_file(const std::string path)
	{
		try
		{
			script::ReferenceMap refmap;
			ReferenceSolver rs(m_fs, "", path, refmap, m_library_path);
			script::compiler::Compiler compiler(refmap);
			auto cf = compiler.compile();
			for (auto& it : cf)
			{
				m_compiledfiles[it.first] = it.second;
			}
		}
		catch (script::ast::ASTException& e)
		{
			LOG_WARNING("Compile Error: %s\n", e.what());
			return false;
		}
		catch (script::compiler::CompileException& e)
		{
			LOG_WARNING("Compile Error: %s\n", e.what());
			return false;
		}
		return true;
	}
	void ScriptEngine::create_virtual_machine()
	{
		m_vm = std::make_unique<script::vm::VirtualMachine>(m_compiledfiles);
		//m_vm->set_flags(vm::flags::kVerbose);
		script::register_stockfunctions(*m_vm);
		for (auto& it : m_registeredfunctions)
			m_vm->register_function(it.first, it.second);
	}
}; // namespace script