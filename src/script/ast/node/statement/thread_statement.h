#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ThreadStatement : Statement
		{
			std::unique_ptr<Expression> expression;
			virtual void print(Printer& out) override
			{
				out.print("thread statement:");
				out.indent();
				expression->print(out);
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler