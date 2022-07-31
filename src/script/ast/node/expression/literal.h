#pragma once
#include <script/ast/node/expression.h>
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Literal : Expression
		{
			enum class Type
			{
				kString,
				kInteger,
				kNumber,
				kVector
			} type;
			std::string value;

			virtual void accept(ASTVisitor& visitor) override
			{
				visitor.visit(*this);
			}

			virtual void print(Printer& out) override
			{
				out.print("literal: {value: %s, type: %d}", value.c_str(), type);
			}
		};
	}; // namespace ast
};	   // namespace compiler