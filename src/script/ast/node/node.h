#pragma once
#include <string>
#include <functional>
#include "../printer.h"
#include <script/ast/visitor.h>

namespace compiler
{
	namespace ast
	{
		struct Node
		{
			size_t start, end;
			std::string raw;

			virtual void accept(ASTVisitor& visitor) = 0;
			virtual void print(Printer&) = 0;

			virtual ~Node()
			{
			}
		};
	}; // namespace ast
};	   // namespace compiler