#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct IfStatement : Statement
		{
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> consequent;
			std::unique_ptr<Statement> alternative;
		};
	}; // namespace ast
};	   // namespace compiler