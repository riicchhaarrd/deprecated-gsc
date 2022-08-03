#pragma once
#include "../expression.h"
#include "identifier.h"
#include <string>
#include <vector>
#include <memory>

namespace script
{
	namespace ast
	{
		struct CallExpression : Expression
		{
			bool threaded = false;
			bool pointer = false;
			std::unique_ptr<Expression> object;
			std::unique_ptr<Expression> callee;
			std::vector<std::unique_ptr<Expression>> arguments;

			AST_NODE(CallExpression)

			virtual void print(Printer& out) override
			{
				out.print("%scall expression:", threaded ? "threaded " : "");
				if (object)
				{
					out.indent();
					out.print("object:");
					out.indent();
					object->print(out);
					out.unindent();
				}

				out.indent();
				out.print("callee:");
				out.indent();
				callee->print(out);

				out.unindent();

				out.print("arguments:");
				out.indent();
				for (auto& arg : arguments)
					arg->print(out);
				out.unindent();
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler