#pragma once
#include "../expression.h"
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

			virtual void print(Printer& out) override
			{
				out.print("literal: {value: %s, type: %d}", value.c_str(), type);
			}
		};
	}; // namespace ast
};	   // namespace compiler