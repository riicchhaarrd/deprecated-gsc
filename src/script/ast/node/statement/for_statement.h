#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ForStatement : Statement
		{
			std::unique_ptr<Statement> init; //kinda special statement, init-statement
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> update;
			std::unique_ptr<Statement> body;
		};
	}; // namespace ast
};	   // namespace compiler