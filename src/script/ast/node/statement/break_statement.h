#pragma once
#include "../statement.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BreakStatement : Statement
		{
			//std::unique_ptr<Node> label;
		};
	}; // namespace ast
};	   // namespace compiler