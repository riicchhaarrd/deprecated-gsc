#pragma once
#include "../statement.h"
#include <string>

namespace compiler
{
	namespace ast
	{
		struct EmptyStatement : Statement
		{
			virtual void print(Printer& out) override
			{
				out.print("empty statement");
			}
		};
	}; // namespace ast
};	   // namespace compiler