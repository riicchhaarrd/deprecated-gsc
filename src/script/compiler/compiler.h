#pragma once
#include <unordered_map>
#include <memory>
#include <core/default_filesystem.h>
#include <script/ast/traverser.h>
#include <script/compiler/bytecode_generator.h>
#include "traverse_info.h"

namespace script
{
	namespace compiler
	{

		class Compiler
		{
		  public:
			using Imports = std::unordered_map<std::string, std::unique_ptr<Compiler>>;

		  private:
			Imports& m_imports;

		  public:
			Compiler(Imports& imports) : m_imports(imports)
			{
			}

			bool compile(ast::Node& program, vm::Constants& constants, std::vector<vm::Function>& functions)
			{
				ast::NodeTraverser nt;
				TraverseInfo ti;
				nt.add_visitor(&ti.local_variables_visitor);
				nt.add_visitor(&ti.imported_functions_visitor);
				nt.add_visitor(&ti.function_visitor);

				nt.traverse(program);
				default_filesystem fs;

				BytecodeGenerator bcgen(ti, constants, functions);
				bcgen.visit_node(program);

				// std::ofstream out("F:\\SteamLibrary\\steamapps\\common\\Call of Duty 2\\main\\maps\\moscow2.gsc");
				// GSCWriter writer(out);
				// writer.visit_node(program);
#if 0
			for (auto& it : imported_functions_visitor.imports())
			{
				if (m_imports.contains(it))
					continue;
				script::ast::ASTGenerator gen;
				gen.generate(fs, "F:/iwd/", "F:/iwd/" + it + ".gsc");
				auto& r = gen.root();
				m_imports[it] = std::make_unique<Compiler>(m_imports);
				auto* ptr = m_imports[it].get();
				ptr->compile(r);
				printf("import: %s\n", it.c_str());
			}
#endif
				for (auto& fn : ti.local_variables_visitor.functions())
				{
					printf("%s: %d local variables\n", fn.first.c_str(), fn.second);
				}
				return true;
			}
		};
	};
};