#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ConditionalExpression : Node //TernaryExpression
		{
			std::unique_ptr<Node> condition;
			std::unique_ptr<Node> consequent;
			std::unique_ptr<Node> alternative;
		};
	}; // namespace ast
};	   // namespace compiler