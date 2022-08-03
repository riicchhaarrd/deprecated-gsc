#include "recursive_visitor.h"

void compiler::RecursiveASTVisitor::visit(ast::Program& n)
{
	if (!pre_visit(n))
		return;
	for (auto& it : n.body)
	{
		it->accept(*this);
	}
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::FunctionDeclaration& n)
{
	if (!pre_visit(n))
		return;
	n.body->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::BlockStatement& n)
{
	if (!pre_visit(n))
		return;
	for (auto& it : n.body)
	{
		it->accept(*this);
	}
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::IfStatement& n)
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

void compiler::RecursiveASTVisitor::visit(ast::WhileStatement& n)
{
	if (!pre_visit(n))
		return;
	if (n.test)
		n.test->accept(*this);
	if (n.body)
		n.body->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::ForStatement& n)
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

void compiler::RecursiveASTVisitor::visit(ast::DoWhileStatement& n)
{
	if (!pre_visit(n))
		return;
	if (n.body)
		n.body->accept(*this);
	if (n.test)
		n.test->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::ReturnStatement& n)
{
	if (!pre_visit(n))
		return;
	if (n.argument)
		n.argument->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::BreakStatement& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::ContinueStatement& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::WaitStatement& n)
{
	if (!pre_visit(n))
		return;
	if (n.duration)
		n.duration->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::WaitTillFrameEndStatement& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::ExpressionStatement& n)
{
	if (!pre_visit(n))
		return;
	if (n.expression)
		n.expression->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::EmptyStatement& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::LocalizedString& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::Literal& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::Identifier& n)
{
	if (!pre_visit(n))
		return;
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::FunctionPointer& n)
{
	if (!pre_visit(n))
		return;
	if (n.identifier)
		n.identifier->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::BinaryExpression& n)
{
	if (!pre_visit(n))
		return;
	if (n.left)
		n.left->accept(*this);
	if (n.right)
		n.right->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::AssignmentExpression& n)
{
	if (!pre_visit(n))
		return;
	if (n.lhs)
		n.lhs->accept(*this);
	if (n.rhs)
		n.rhs->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::CallExpression& n)
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

void compiler::RecursiveASTVisitor::visit(ast::ConditionalExpression& n)
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

void compiler::RecursiveASTVisitor::visit(ast::MemberExpression& n)
{
	if (!pre_visit(n))
		return;
	if (n.object)
		n.object->accept(*this);
	if (n.prop)
		n.prop->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::UnaryExpression& n)
{
	if (!pre_visit(n))
		return;
	if (n.argument)
		n.argument->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::VectorExpression& n)
{
	if (!pre_visit(n))
		return;
	for (auto& it : n.elements)
		it->accept(*this);
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::ArrayExpression& n)
{
	if (!pre_visit(n))
		return;
	for (auto& it : n.elements)
	{
		it->accept(*this);
	}
	post_visit(n);
}

void compiler::RecursiveASTVisitor::visit(ast::SwitchStatement& n)
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

void compiler::RecursiveASTVisitor::visit(ast::SwitchCase& n)
{
	if (!pre_visit(n))
		return;
	if (n.test)
		n.test->accept(*this);
	for (auto & it : n.consequent)
		it->accept(*this);
	post_visit(n);
}