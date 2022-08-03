#pragma once
#include "identifier.h"
#include <string>

namespace script
{
	namespace ast
	{
		struct FunctionPointer : Expression
		{
			std::unique_ptr<Identifier> identifier;

			AST_NODE(FunctionPointer)

			virtual void print(Printer& out) override
			{
				out.print("function pointer:");
				out.indent();
				identifier->print(out);
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler