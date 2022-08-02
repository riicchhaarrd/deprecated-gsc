#pragma once
#include "visitor.h"
#include "nodes.h"

//nice ideas from
//https://stackoverflow.com/questions/11464735/visitor-pattern-for-ast
//https://github.com/nikic/PHP-Parser/blob/master/doc/component/Walking_the_AST.markdown
/*
A common pattern is that enterNode is used to collect some information and then leaveNode performs modifications based
on that. At the time when leaveNode is called, all the code inside the node will have already been visited and necessary
information collected.
As you usually do not want to implement all four methods, it is recommended that you extend NodeVisitorAbstract instead
of implementing the interface directly. The abstract class provides empty default implementations.
*/

namespace compiler
{
	class RecursiveASTVisitor : public ASTVisitor
	{
		// Inherited via ASTVisitor
		virtual void visit(ast::FunctionDeclaration&) override;
		virtual void visit(ast::BlockStatement&) override;
		virtual void visit(ast::IfStatement&) override;
		virtual void visit(ast::WhileStatement&) override;
		virtual void visit(ast::ForStatement&) override;
		virtual void visit(ast::DoWhileStatement&) override;
		virtual void visit(ast::ReturnStatement&) override;
		virtual void visit(ast::ContinueStatement&) override;
		virtual void visit(ast::BreakStatement&) override;
		virtual void visit(ast::WaitStatement&) override;
		virtual void visit(ast::WaitTillFrameEndStatement&) override;
		virtual void visit(ast::ExpressionStatement&) override;
		virtual void visit(ast::EmptyStatement&) override;
		virtual void visit(ast::LocalizedString&) override;
		virtual void visit(ast::Literal&) override;
		virtual void visit(ast::Identifier&) override;
		virtual void visit(ast::FunctionPointer&) override;
		virtual void visit(ast::BinaryExpression&) override;
		virtual void visit(ast::AssignmentExpression&) override;
		virtual void visit(ast::CallExpression&) override;
		virtual void visit(ast::ConditionalExpression&) override;
		virtual void visit(ast::MemberExpression&) override;
		virtual void visit(ast::UnaryExpression&) override;
		virtual void visit(ast::VectorExpression&) override;
		virtual void visit(ast::ArrayExpression&) override;
		virtual void visit(ast::Program&) override;
		virtual void visit(ast::DeveloperBlock&) override;
		virtual void visit(ast::SwitchCase&) override;
		virtual void visit(ast::SwitchStatement&) override;
	  public:
		virtual bool pre_visit(ast::SwitchCase&)
		{
			return true;
		}
		virtual bool pre_visit(ast::SwitchStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::DeveloperBlock&)
		{
			return true;
		}
		virtual bool pre_visit(ast::ContinueStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::Program&)
		{
			return true;
		}
		virtual bool pre_visit(ast::FunctionDeclaration&)
		{
			return true;
		}
		virtual bool pre_visit(ast::BlockStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::IfStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::WhileStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::ForStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::DoWhileStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::ReturnStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::BreakStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::WaitStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::WaitTillFrameEndStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::ExpressionStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::EmptyStatement&)
		{
			return true;
		}
		virtual bool pre_visit(ast::LocalizedString&)
		{
			return true;
		}
		virtual bool pre_visit(ast::Literal&)
		{
			return true;
		}
		virtual bool pre_visit(ast::Identifier&)
		{
			return true;
		}
		virtual bool pre_visit(ast::FunctionPointer&)
		{
			return true;
		}
		virtual bool pre_visit(ast::BinaryExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::AssignmentExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::CallExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::ConditionalExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::MemberExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::UnaryExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::VectorExpression&)
		{
			return true;
		}
		virtual bool pre_visit(ast::ArrayExpression&)
		{
			return true;
		}

		virtual void post_visit(ast::Program&)
		{
		}
		virtual void post_visit(ast::FunctionDeclaration&)
		{
		}
		virtual void post_visit(ast::BlockStatement&)
		{
		}
		virtual void post_visit(ast::IfStatement&)
		{
		}
		virtual void post_visit(ast::WhileStatement&)
		{
		}
		virtual void post_visit(ast::ForStatement&)
		{
		}
		virtual void post_visit(ast::DoWhileStatement&)
		{
		}
		virtual void post_visit(ast::ReturnStatement&)
		{
		}
		virtual void post_visit(ast::BreakStatement&)
		{
		}
		virtual void post_visit(ast::WaitStatement&)
		{
		}
		virtual void post_visit(ast::WaitTillFrameEndStatement&)
		{
		}
		virtual void post_visit(ast::ExpressionStatement&)
		{
		}
		virtual void post_visit(ast::EmptyStatement&)
		{
		}
		virtual void post_visit(ast::LocalizedString&)
		{
		}
		virtual void post_visit(ast::Literal&)
		{
		}
		virtual void post_visit(ast::Identifier&)
		{
		}
		virtual void post_visit(ast::FunctionPointer&)
		{
		}
		virtual void post_visit(ast::BinaryExpression&)
		{
		}
		virtual void post_visit(ast::AssignmentExpression&)
		{
		}
		virtual void post_visit(ast::CallExpression&)
		{
		}
		virtual void post_visit(ast::ConditionalExpression&)
		{
		}
		virtual void post_visit(ast::MemberExpression&)
		{
		}
		virtual void post_visit(ast::UnaryExpression&)
		{
		}
		virtual void post_visit(ast::VectorExpression&)
		{
		}
		virtual void post_visit(ast::ArrayExpression&)
		{
		}
		virtual void post_visit(ast::DeveloperBlock&)
		{
		}
		virtual void post_visit(ast::ContinueStatement&)
		{
		}
		virtual void post_visit(ast::SwitchCase&)
		{
		}
		virtual void post_visit(ast::SwitchStatement&)
		{
		}
		void visit(ast::Node& n)
		{
			n.accept(*this);
		}
	};

}; // namespace compiler