#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ForStatement : Node
		{
			std::unique_ptr<Node> init;
			std::unique_ptr<Node> test;
			std::unique_ptr<Node> update;
			std::unique_ptr<Node> body;
		};
	}; // namespace ast
};	   // namespace compiler