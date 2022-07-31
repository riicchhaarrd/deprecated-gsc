#pragma once
#include "node.h"
#include <string>
#include <vector>
#include <memory>
#include <script/ast/node/statement.h>

namespace compiler
{
	namespace ast
	{
		struct Program : Node
		{
			std::vector<std::unique_ptr<Node>> body;
			virtual void print(Printer& out) override
			{
				out.print("program:");
				out.indent();
				for (auto& n : body)
					n->print(out);
				out.unindent();
			}
			AST_NODE(Program)
		};
	}; // namespace ast
};	   // namespace compiler