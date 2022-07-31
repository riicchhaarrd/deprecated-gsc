#pragma once
#include "../ast/node/program.h"
#include "../ast/node/function_declaration.h"

namespace compiler
{
	class ScriptCompiler
	{
		const ast::Program& tree;
	  public:
		ScriptCompiler(const ast::Program& t) : tree(t)
		{
		}

		bool compile_function(std::unique_ptr<ast::FunctionDeclaration>& func)
		{
		}
	};
};