#pragma once
#include <string>
#include <functional>
#include "../printer.h"
#include <script/ast/visitor.h>

#define AST_NODE(x) \
virtual const char *to_string() override \
{ \
	return #x; \
} \
virtual void accept(ASTVisitor& visitor) override \
{ \
	visitor.visit(*this); \
}

namespace script
{
	namespace ast
	{
		struct Node
		{
			size_t start, end;
			std::string raw;

			virtual const char* to_string() = 0;
			virtual void accept(ASTVisitor& visitor) = 0;
			virtual void print(Printer&) = 0;

			virtual ~Node()
			{
			}
		};
	}; // namespace ast
};	   // namespace compiler