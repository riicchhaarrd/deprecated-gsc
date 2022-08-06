#pragma once
#include "visitor.h"

namespace script
{
	namespace ast
	{
		class ASTRecallVisitor : public ASTVisitor
		{
			ast::Node* last_visited_node = nullptr;
		  public:
			ast::Node* get_last_visited_node()
			{
				return last_visited_node;
			}
			virtual void visit_impl(Program&) = 0;
			virtual void visit_impl(FunctionDeclaration&) = 0;
			virtual void visit_impl(SwitchCase&) = 0;
			virtual void visit_impl(Directive&) = 0;

			// statements
			virtual void visit_impl(BlockStatement&) = 0;
			virtual void visit_impl(IfStatement&) = 0;
			virtual void visit_impl(WhileStatement&) = 0;
			virtual void visit_impl(ForStatement&) = 0;
			virtual void visit_impl(DoWhileStatement&) = 0;
			virtual void visit_impl(ReturnStatement&) = 0;
			virtual void visit_impl(BreakStatement&) = 0;
			virtual void visit_impl(WaitStatement&) = 0;
			virtual void visit_impl(WaitTillFrameEndStatement&) = 0;
			virtual void visit_impl(ExpressionStatement&) = 0;
			virtual void visit_impl(EmptyStatement&) = 0;
			virtual void visit_impl(ContinueStatement&) = 0;
			virtual void visit_impl(SwitchStatement&) = 0;

			// expressions
			virtual void visit_impl(LocalizedString&) = 0;
			virtual void visit_impl(Literal&) = 0;
			virtual void visit_impl(Identifier&) = 0;
			virtual void visit_impl(FunctionPointer&) = 0;
			virtual void visit_impl(BinaryExpression&) = 0;
			virtual void visit_impl(AssignmentExpression&) = 0;
			virtual void visit_impl(CallExpression&) = 0;
			virtual void visit_impl(ConditionalExpression&) = 0;
			virtual void visit_impl(MemberExpression&) = 0;
			virtual void visit_impl(UnaryExpression&) = 0;
			virtual void visit_impl(VectorExpression&) = 0;
			virtual void visit_impl(ArrayExpression&) = 0;

			virtual void visit(Program& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(FunctionDeclaration& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(SwitchCase& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(Directive& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}

			// statements
			virtual void visit(BlockStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(IfStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(WhileStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(ForStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(DoWhileStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(ReturnStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(BreakStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(WaitStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(WaitTillFrameEndStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(ExpressionStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(EmptyStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(ContinueStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(SwitchStatement& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}

			// expressions
			virtual void visit(LocalizedString& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(Literal& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(Identifier& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(FunctionPointer& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(BinaryExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(AssignmentExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(CallExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(ConditionalExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(MemberExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(UnaryExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(VectorExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
			virtual void visit(ArrayExpression& n)
			{
				last_visited_node = &n;
				visit_impl(n);
			}
		};
	}; // namespace ast
}; // namespace script