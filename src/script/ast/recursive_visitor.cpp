#include "recursive_visitor.h"

namespace script
{
	namespace ast
	{
		void RecursiveASTVisitor::visit(Program& n)
		{
			if (!pre_visit(n))
				return;
			for (auto& it : n.body)
			{
				it->accept(*this);
			}
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(FunctionDeclaration& n)
		{
			if (!pre_visit(n))
				return;
			n.body->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(BlockStatement& n)
		{
			if (!pre_visit(n))
				return;
			for (auto& it : n.body)
			{
				it->accept(*this);
			}
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(IfStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.test)
				n.test->accept(*this);
			if (n.consequent)
				n.consequent->accept(*this);
			if (n.alternative)
				n.alternative->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(WhileStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.test)
				n.test->accept(*this);
			if (n.body)
				n.body->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(ForStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.init)
				n.init->accept(*this);
			if (n.test)
				n.test->accept(*this);
			if (n.body)
				n.body->accept(*this);
			if (n.update)
				n.update->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(DoWhileStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.body)
				n.body->accept(*this);
			if (n.test)
				n.test->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(ReturnStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.argument)
				n.argument->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(BreakStatement& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(ContinueStatement& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(WaitStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.duration)
				n.duration->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(WaitTillFrameEndStatement& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(ExpressionStatement& n)
		{
			if (!pre_visit(n))
				return;
			if (n.expression)
				n.expression->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(EmptyStatement& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(LocalizedString& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(Literal& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(Identifier& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(FunctionPointer& n)
		{
			if (!pre_visit(n))
				return;
			//if (n.identifier)
				//n.identifier->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(BinaryExpression& n)
		{
			if (!pre_visit(n))
				return;
			if (n.left)
				n.left->accept(*this);
			if (n.right)
				n.right->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(AssignmentExpression& n)
		{
			if (!pre_visit(n))
				return;
			if (n.lhs)
				n.lhs->accept(*this);
			if (n.rhs)
				n.rhs->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(CallExpression& n)
		{
			if (!pre_visit(n))
				return;
			if (n.object)
				n.object->accept(*this);
			for (auto& arg : n.arguments)
				arg->accept(*this);
			if (n.callee)
				n.callee->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(ConditionalExpression& n)
		{
			if (!pre_visit(n))
				return;
			if (n.condition)
				n.condition->accept(*this);
			if (n.consequent)
				n.consequent->accept(*this);
			if (n.alternative)
				n.alternative->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(MemberExpression& n)
		{
			if (!pre_visit(n))
				return;
			if (n.object)
				n.object->accept(*this);
			if (n.prop)
				n.prop->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(UnaryExpression& n)
		{
			if (!pre_visit(n))
				return;
			if (n.argument)
				n.argument->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(VectorExpression& n)
		{
			if (!pre_visit(n))
				return;
			for (auto& it : n.elements)
				it->accept(*this);
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(ArrayExpression& n)
		{
			if (!pre_visit(n))
				return;
			for (auto& it : n.elements)
			{
				it->accept(*this);
			}
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(SwitchStatement& n)
		{
			if (!pre_visit(n))
				return;
			n.discriminant->accept(*this);
			for (auto& it : n.cases)
			{
				it->accept(*this);
			}
			post_visit(n);
		}

		void RecursiveASTVisitor::visit(SwitchCase& n)
		{
			if (!pre_visit(n))
				return;
			if (n.test)
				n.test->accept(*this);
			for (auto& it : n.consequent)
				it->accept(*this);
			post_visit(n);
		}
		void RecursiveASTVisitor::visit(Directive& n)
		{
			if (!pre_visit(n))
				return;
			post_visit(n);
		}
	}; // namespace ast
};	   // namespace script