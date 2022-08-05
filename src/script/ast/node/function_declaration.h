#pragma once
#include "expression/identifier.h"
#include "statement.h"
#include <string>
#include <vector>
#include <memory>

namespace script
{
	namespace ast
	{
		struct FunctionDeclaration : Node
		{
			std::string function_name;
			//std::vector<std::unique_ptr<Identifier>> parameters;
			std::vector<std::string> parameters;
			std::unique_ptr<Statement> body;
			//std::unique_ptr<Node> return_data_type;
			bool variadic = false;
			std::vector<std::unique_ptr<Identifier>> declarations;
			virtual void print(Printer& out) override
			{
				out.print("function '%s':", function_name.c_str());
				out.indent();
				body->print(out);
				out.unindent();
			}
			AST_NODE(FunctionDeclaration)
		};
	}; // namespace ast
};	   // namespace compiler