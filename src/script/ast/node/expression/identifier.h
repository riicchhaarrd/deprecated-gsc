#pragma once
#include "../expression.h"
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Identifier : Expression
		{
			std::string file_reference; //optional e.g util\string::tolower
			std::string name;
			Identifier(const std::string s)
			{
				name = s;
			}
			Identifier(const std::string s, const std::string file_ref)
			{
				name = s;
				file_reference = file_ref;
			}
			virtual void print(Printer& out) override
			{
				out.print("identifier: {file_reference: \"%s\", name: \"%s\"}", file_reference.c_str(), name.c_str());
			}
		};
	}; // namespace ast
};	   // namespace compiler