#pragma once
#include "node.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BlockStatement : Node
		{
			std::vector<std::unique_ptr<Node>> body;
		};
	}; // namespace ast
};	   // namespace compiler