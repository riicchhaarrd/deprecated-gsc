#pragma once
#include "expression/identifier.h"
#include "statement.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct FunctionDeclaration : Node
		{
			std::unique_ptr<Identifier> id;
			std::vector<std::unique_ptr<Identifier>> parameters;
			std::unique_ptr<Statement> body;
			//std::unique_ptr<Node> return_data_type;
			bool variadic = false;
			std::vector<std::unique_ptr<Identifier>> declarations;
			virtual void print(Printer& out) override
			{
				out.print("function declaration:");
				out.indent();
				id->print(out);
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler