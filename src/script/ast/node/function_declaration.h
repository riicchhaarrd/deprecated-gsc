#pragma once
#include "node.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct FunctionDeclaration : Node
		{
			std::unique_ptr<Node> id;
			std::vector<std::unique_ptr<Node>> parameters;
			std::unique_ptr<Node> body;
			std::unique_ptr<Node> return_data_type;
			bool variadic = false;
			std::vector<std::unique_ptr<Node>> declarations;
		};
	}; // namespace ast
};	   // namespace compiler