#pragma once
#include <script/ast/node/statement.h>
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct DeveloperBlock : Statement
		{
			std::vector<std::unique_ptr<Statement>> body;

			AST_NODE(DeveloperBlock)

			virtual void print(Printer& out) override
			{
				out.print("developer block:");
				out.indent();
				for (auto& stmt : body)
					stmt->print(out);
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler