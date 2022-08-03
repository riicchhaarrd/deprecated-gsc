#pragma once
#include "../statement.h"
#include <string>

namespace script
{
	namespace ast
	{
		struct EmptyStatement : Statement
		{
			virtual void print(Printer& out) override
			{
				out.print("empty statement");
			}

			AST_NODE(EmptyStatement)
		};
	}; // namespace ast
};	   // namespace compiler