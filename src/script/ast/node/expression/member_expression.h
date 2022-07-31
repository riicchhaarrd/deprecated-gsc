#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct MemberExpression : Expression
		{
			std::unique_ptr<Expression> object;
			std::unique_ptr<Expression> prop;
			int op;

			AST_NODE(MemberExpression)

			virtual void print(Printer& out) override
			{
				out.print("member expression:");
				out.indent();

				out.print("object:");
				out.indent();
				object->print(out);
				out.unindent();

				out.print("property:");
				out.indent();
				prop->print(out);
				out.unindent();

				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler