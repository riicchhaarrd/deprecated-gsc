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
		script::ReferenceMap& m_global_reference_map;

	  public:
		ReferenceSolver(filesystem_api &fs, const std::string path_base_, const std::string file_name_,
						script::ReferenceMap& global_reference_map)
			: m_global_reference_map(global_reference_map), path_base(path_base_), m_filename(file_name_)
		{
			// first get all the functions that we reference
			script::ast::ASTGenerator m_generator;
			m_generator.generate(fs, path_base, path_base + m_filename + ".gsc");

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
					ReferenceSolver solver(fs, path_base_, pair_.first, global_reference_map);
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
			LOG_ERROR("Script Error: %s\n", ex.what());
			m_vm.reset();
		}
	}
	void ScriptEngine::execute_thread(const std::string file, const std::string function, size_t nargs)
	{
		if (!m_vm)
			return;
		try
		{
			m_vm->exec_thread(nullptr, file, function, nargs);
		}
		catch (vm::Exception& ex)
		{
			LOG_ERROR("Script Error: %s\n", ex.what());
			m_vm.reset();
		}
	}
	bool ScriptEngine::load(const std::string path)
	{
		try
		{
			//maps/mp/gametypes/dm
			script::ReferenceMap refmap;
			ReferenceSolver rs(m_fs, "", path, refmap);
			printf("loaded files:\n");
			for (auto& it : refmap)
			{
				printf("\t%s\n", it.first.c_str());
			}
			script::compiler::Compiler compiler(refmap);
			m_compiledfiles = compiler.compile();
			// register_stockfunctions(interpreter);
			// script::FunctionArguments args;
			// interpreter.call_function("maps/mp/gametypes/dm", "main", args);

			m_vm = std::make_unique<script::vm::VirtualMachine>(m_compiledfiles);
			script::register_stockfunctions(*m_vm);
		}
		catch (script::ast::ASTException& e)
		{
			LOG_ERROR("Compile Error: %s\n", e.what());
			return false;
		}
		catch (script::compiler::CompileException& e)
		{
			LOG_ERROR("Compile Error: %s\n", e.what());
			return false;
		}
		return true;
	}
}; // namespace script