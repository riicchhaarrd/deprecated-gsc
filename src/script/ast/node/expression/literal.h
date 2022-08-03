#pragma once
#include <script/ast/node/expression.h>
#include <string>

namespace script
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
				kVector,
				kAnimation,
				kUndefined
			} type;
			std::string value;

			AST_NODE(Literal)

			virtual void print(Printer& out) override
			{
				out.print("literal: {value: %s, type: %d}", value.c_str(), type);
			}
		};
	}; // namespace ast
};	   // namespace compiler