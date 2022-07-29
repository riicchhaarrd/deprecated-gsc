#pragma once
#include <string>
#include <functional>
#include "../printer.h"

namespace compiler
{
	namespace ast
	{
		struct Node
		{
			size_t start, end;
			std::string raw;
			using VisitorFunction = std::function<void(Node*)>;
			virtual void visit(VisitorFunction)
			{
			}

			virtual void print(Printer&) = 0;

			virtual ~Node()
			{
			}
		};
	}; // namespace ast
};	   // namespace compiler