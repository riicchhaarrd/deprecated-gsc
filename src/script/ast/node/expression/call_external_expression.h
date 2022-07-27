#pragma once
#include "call_expression.h"
#include "identifier.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct CallExternalExpression : CallExpression
		{
			std::unique_ptr<Identifier> file_reference;
			virtual void visit(VisitorFunction fn) override
			{
				fn(file_reference.get());
				fn(callee.get());
				for (auto& arg : arguments)
					fn(arg.get());
			}
		};
	}; // namespace ast
};	   // namespace compiler