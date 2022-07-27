#pragma once
#include "../statement.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BlockStatement : Statement
		{
			std::vector<std::unique_ptr<Statement>> body;
		};
	}; // namespace ast
};	   // namespace compiler