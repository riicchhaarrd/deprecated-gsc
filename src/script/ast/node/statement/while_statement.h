#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct WhileStatement : Statement
		{
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> body;
		};
	}; // namespace ast
};	   // namespace compiler