#pragma once
#include "../statement.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ContinueStatement : Statement
		{
			AST_NODE(ContinueStatement)

			virtual void print(Printer& out) override
			{
				out.print("continue statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler