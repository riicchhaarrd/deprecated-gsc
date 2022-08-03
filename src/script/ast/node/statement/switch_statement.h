#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>
#include <vector>

namespace script
{
	namespace ast
	{
		struct SwitchCase : Node
		{
			std::unique_ptr<Expression> test;
			std::vector<std::shared_ptr<Statement>> consequent;
			AST_NODE(SwitchCase)

			virtual void print(Printer& out) override
			{
				out.print("switch case:");
			}
		};

		struct SwitchStatement : Statement
		{
			std::unique_ptr<Expression> discriminant;
			std::vector<std::shared_ptr<SwitchCase>> cases;

			AST_NODE(SwitchStatement)

			virtual void print(Printer& out) override
			{
				out.print("switch statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler