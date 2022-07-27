#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ReturnStatement : Statement
		{
			std::unique_ptr<Expression> argument;
		};
	}; // namespace ast
};	   // namespace compiler