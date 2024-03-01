#include <script/ast/ast_generator.h>
#include <core/default_filesystem.h>
#include <script/ast/recursive_visitor.h>
#include <script/ast/gsc_writer.h>
#include <stdexcept>
#include <variant>
#include <cstring>
#include <set>
#include <common/exception.h>
#include <script/ast/type_visitor.h>
#include <script/vm/types.h>
#include <script/vm/virtual_machine.h>
#include <script/compiler/compiler.h>
#include <script/compiler/exception.h>
#include <script/stockfunctions.h>
#include <chrono>
#include <thread>
#include <cassert>
#include <core/time.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

class ReferenceSolver
{
	std::string path_base;
	std::string m_filename;
	const std::string& m_library_path;
	script::ReferenceMap& m_global_reference_map;

  public:
	ReferenceSolver(const std::string path_base_, const std::string file_name_,
					script::ReferenceMap& global_reference_map, const std::string& library_path)
		: m_global_reference_map(global_reference_map),
		  path_base(path_base_),
		  m_filename(file_name_),
		  m_library_path(library_path)
	{
		default_filesystem fs;
		// first get all the functions that we reference
		script::ast::ASTGenerator m_generator;

		bool ret = false;
		//if (m_filename.find('/') == std::string::npos)
			//ret = m_generator.generate(fs, m_library_path, m_library_path + m_filename + ".gsc");
		//else
			ret = m_generator.generate(fs, path_base, path_base + m_filename + ".gsc");
		if (!ret)
		{
			throw script::compiler::CompileException("Failed to read file {}", m_filename.c_str());
		}

		script::LoadedProgramReference& lpr = m_global_reference_map[file_name_];
		lpr.program = std::move(m_generator.root());
		lpr.name = m_filename;

		script::ast::NodeTypeVisitor<script::ast::FunctionDeclaration> ntv;
		auto &fun = ntv.find(lpr.program.get());
		for (auto* f : fun)
		{
			lpr.function_map[util::string::to_lower(f->function_name)] = f;
		}

		//lpr.m_parent_visitor.visit_node(*lpr.program.get());

		script::compiler::FunctionCallReferenceVisitor fcrv(m_filename);
		fcrv.visit_node(*lpr.program.get());

		//add the references found to the global references
		//not using it atm, we just load and keep the whole tree, but was maybe nice to only specific functions
		for (auto& pair_ : fcrv.references())
		{
			//for (auto & function_ : pair_.second)
				//m_global_reference_map[pair_.first].insert(function_);
			//if (pair_.first != m_filename)
			if (m_global_reference_map.find(pair_.first) == m_global_reference_map.end())
			{
				ReferenceSolver solver(path_base_, pair_.first, global_reference_map, m_library_path);
			}
		}
	}
};

#ifndef EMSCRIPTEN
#define EMSCRIPTEN_KEEPALIVE
#endif
bool verbose = false;

extern "C" EMSCRIPTEN_KEEPALIVE void run_file(const char* base_path, const char* file, const char *function)
{
	//printf("run_file(%s, %s)\n", file, function);
	default_filesystem fs;
	try
	{
		script::ReferenceMap refmap;
		ReferenceSolver rs(base_path, file, refmap, "data/scriptlib/");
		//printf("loaded files:\n");
		for (auto& it : refmap)
		{
			//printf("\t%s\n", it.first.c_str());
		}
		script::compiler::Compiler compiler(refmap);
		auto cf = compiler.compile();
		// register_stockfunctions(interpreter);
		// script::FunctionArguments args;
		// interpreter.call_function("maps/mp/gametypes/dm", "main", args);

		script::vm::VirtualMachine vm(cf);
		if (verbose)
			vm.set_flags(script::vm::flags::kVerbose);
		script::register_stockfunctions(vm);
		vm.exec_thread(nullptr, vm.get_level_object(), file, function, 0, false);
		// vm.exec_thread(vm.get_level_object(), "maps/mp/gametypes/_callbacksetup", "CodeCallback_StartGameType", 0);

		do
		{
			vm.run();
			core::sleep(1000 / 20);
			// std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 20));
			// printf("%d threads\n", vm.thread_count());
		} while (vm.thread_count() > 0);
	}
	catch (script::ast::ASTException& e)
	{
		printf("AST Error: %s\n", e.what());
	}
	catch (script::compiler::CompileException& e)
	{
		printf("Compile Error: %s\n", e.what());
	}
	catch (script::vm::Exception& e)
	{
		printf("VM Error: %s\n", e.what());
	}
}

int main(int argc, const char **argv)
{
	#ifndef EMSCRIPTEN

	const char* base_dir = "./";
	const char* function = "main";
	const char* script_file = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			if (i + 1 < argc) {
				base_dir = argv[++i];
			}
			else {
				printf("Error: No directory specified.\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			if (i + 1 < argc) {
				script_file = argv[++i];
			}
			else {
				printf("Error: No script specified.\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-f") == 0)
		{
			if (i + 1 < argc) {
				function = argv[++i];
			}
			else {
				printf("Error: No function specified.\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "--verbose") == 0)
		{
			verbose = true;
		}
	}
	if (!script_file)
	{
		printf("Usage: %s -s <script>\n", argv[0]);
		return 1;
	}
	run_file(base_dir, script_file, function);
	#endif
	return 0;
}
