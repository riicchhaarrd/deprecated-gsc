#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct WaitStatement : Statement
		{
			std::unique_ptr<Expression> duration;
			virtual void print(Printer& out) override
			{
				out.print("wait statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler