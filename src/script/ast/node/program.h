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

			virtual void accept(ASTVisitor& visitor)
			{
				visitor.visit(*this);
			}
		};
	}; // namespace ast
};	   // namespace compiler