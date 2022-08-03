#pragma once
#include <script/ast/node/statement.h>
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct DirectiveStatement : Statement
		{
			std::string directive;
			std::string value; //only used for one thing atm, but may change in the future

			AST_NODE(DirectiveStatement)

			virtual void print(Printer& out) override
			{
				out.print("directive statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler