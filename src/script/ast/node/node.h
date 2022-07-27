#pragma once
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Node
		{
			size_t start, end;
			std::string raw;

			virtual ~Node()
			{
			}
		};
	}; // namespace ast
};	   // namespace compiler