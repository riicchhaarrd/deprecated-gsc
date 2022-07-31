#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct WaitTillFrameEndStatement : Statement
		{
			AST_NODE(WaitTillFrameEndStatement)

			virtual void print(Printer& out) override
			{
				out.print("waittillframeend statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler