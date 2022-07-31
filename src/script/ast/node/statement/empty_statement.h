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

			virtual void accept(ASTVisitor& visitor)
			{
				visitor.visit(*this);
			}
		};
	}; // namespace ast
};	   // namespace compiler