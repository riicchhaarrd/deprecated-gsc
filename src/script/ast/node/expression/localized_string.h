#pragma once
#include "identifier.h"
#include <string>

namespace script
{
	namespace ast
	{
		struct LocalizedString : Expression
		{
			std::string reference;

			AST_NODE(LocalizedString)

			virtual void print(Printer& out) override
			{
				out.print("localized string %s", reference.c_str());
			}
		};
	}; // namespace ast
};	   // namespace compiler