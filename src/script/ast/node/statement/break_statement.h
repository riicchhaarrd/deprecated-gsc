#pragma once
#include "../statement.h"
#include <string>
#include <memory>

namespace script
{
	namespace ast
	{
		struct BreakStatement : Statement
		{
			//std::unique_ptr<Node> label;

			AST_NODE(BreakStatement)

			virtual void print(Printer& out) override
			{
				out.print("break statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler