#pragma once
#include <string>
#include <functional>

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

			virtual ~Node()
			{
			}
		};
	}; // namespace ast
};	   // namespace compiler