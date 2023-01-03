#pragma once
#include <string>
#include <functional>
#include <common/type_id.h>
#include "../printer.h"
#include <script/ast/visitor.h>
#include <script/debug_info.h>

#define AST_NODE(x)                                                                                                    \
	virtual const char* to_string() override                                                                           \
	{                                                                                                                  \
		return #x;                                                                                                     \
	}                                                                                                                  \
	virtual size_t kind()                                                                                              \
	{                                                                                                                  \
		return type_id<x>::id();                                                                                       \
	}                                                                                                                  \
	virtual void accept(ASTVisitor& visitor) override                                                                  \
	{                                                                                                                  \
		visitor.visit(*this);                                                                                          \
	}

namespace script
{
	namespace ast
	{
		struct Node
		{
			size_t start, end;
			std::string raw;
			DebugInfo debug;

			virtual const char* to_string() = 0;
			virtual void accept(ASTVisitor& visitor) = 0;
			virtual void print(Printer&) = 0;
			virtual size_t kind() = 0;
			template<typename T>
			T* cast()
			{
				auto k = type_id<T>::id();
				if (k != this->kind())
					return NULL;
				return (T*)this;
			}

			virtual ~Node()
			{
			}
		};
	}; // namespace ast
};	   // namespace compiler