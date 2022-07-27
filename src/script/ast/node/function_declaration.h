#pragma once
#include "expression/identifier.h"
#include "statement.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct FunctionDeclaration : Node
		{
			std::unique_ptr<Identifier> id;
			std::vector<std::unique_ptr<Identifier>> parameters;
			std::unique_ptr<Statement> body;
			//std::unique_ptr<Node> return_data_type;
			bool variadic = false;
			std::vector<std::unique_ptr<Identifier>> declarations;
		};
	}; // namespace ast
};	   // namespace compiler