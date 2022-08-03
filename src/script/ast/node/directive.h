#pragma once
#include "node.h"
#include <string>

namespace script
{
	namespace ast
	{
		struct Directive : Node
		{
			std::string directive;
			std::string value;

			virtual void print(Printer& out) override
			{
				out.print("directive:");
			}
			AST_NODE(Directive)
		};
	}; // namespace ast
};	   // namespace script