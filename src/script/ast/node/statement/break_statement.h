#pragma once
#include "../statement.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BreakStatement : Statement
		{
			//std::unique_ptr<Node> label;
			
			virtual void accept(ASTVisitor& visitor)
			{
				visitor.visit(*this);
			}

			virtual void print(Printer& out) override
			{
				out.print("break statement:");
			}
		};
	}; // namespace ast
};	   // namespace compiler