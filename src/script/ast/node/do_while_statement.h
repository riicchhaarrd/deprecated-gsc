#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct DoWhileStatement : Node
		{
			std::unique_ptr<Node> test;
			std::unique_ptr<Node> body;
		};
	}; // namespace ast
};	   // namespace compiler