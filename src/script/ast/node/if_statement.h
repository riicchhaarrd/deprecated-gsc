#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct IfStatement : Node
		{
			std::unique_ptr<Node> test;
			std::unique_ptr<Node> consequent;
			std::unique_ptr<Node> alternative;
		};
	}; // namespace ast
};	   // namespace compiler