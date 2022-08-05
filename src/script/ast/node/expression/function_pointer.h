#pragma once
#include "identifier.h"
#include <string>

namespace script
{
	namespace ast
	{
		struct FunctionPointer : Expression
		{
			//std::unique_ptr<Identifier> identifier;
			std::string function_name;

			AST_NODE(FunctionPointer)

			virtual void print(Printer& out) override
			{
				out.print("function pointer:");
			}
		};
	}; // namespace ast
};	   // namespace compiler