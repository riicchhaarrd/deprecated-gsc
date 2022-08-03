#pragma once

namespace compiler
{
	namespace ast
	{
		struct Program;
		struct FunctionDeclaration;
		struct BlockStatement;
		struct IfStatement;
		struct WhileStatement;
		struct ForStatement;
		struct DoWhileStatement;
		struct ReturnStatement;
		struct BreakStatement;
		struct WaitStatement;
		struct WaitTillFrameEndStatement;
		struct Literal;
		struct Identifier;
		struct BinaryExpression;
		struct AssignmentExpression;
		struct CallExpression;
		struct ConditionalExpression;
		struct MemberExpression;
		struct UnaryExpression;
		struct VectorExpression;
		struct ArrayExpression;
		struct Expression;
		struct Statement;
		struct FunctionPointer;
		struct LocalizedString;
		struct ExpressionStatement;
		struct EmptyStatement;
		struct DeveloperBlock;
		struct ContinueStatement;
		struct SwitchStatement;
		struct SwitchCase;
		struct DirectiveStatement;
	}; // namespace ast

	class ASTVisitor
	{
	  public:
		virtual ~ASTVisitor()
		{
		}
		virtual void visit(ast::Program&) = 0;
		virtual void visit(ast::FunctionDeclaration&) = 0;
		virtual void visit(ast::SwitchCase&) = 0;

		//statements
		virtual void visit(ast::BlockStatement&) = 0;
		virtual void visit(ast::IfStatement&) = 0;
		virtual void visit(ast::WhileStatement&) = 0;
		virtual void visit(ast::ForStatement&) = 0;
		virtual void visit(ast::DoWhileStatement&) = 0;
		virtual void visit(ast::ReturnStatement&) = 0;
		virtual void visit(ast::BreakStatement&) = 0;
		virtual void visit(ast::WaitStatement&) = 0;
		virtual void visit(ast::WaitTillFrameEndStatement&) = 0;
		virtual void visit(ast::ExpressionStatement&) = 0;
		virtual void visit(ast::EmptyStatement&) = 0;
		virtual void visit(ast::ContinueStatement&) = 0;
		virtual void visit(ast::SwitchStatement&) = 0;
		virtual void visit(ast::DirectiveStatement&) = 0;

		//expressions
		virtual void visit(ast::LocalizedString&) = 0;
		virtual void visit(ast::Literal&) = 0;
		virtual void visit(ast::Identifier&) = 0;
		virtual void visit(ast::FunctionPointer&) = 0;
		virtual void visit(ast::BinaryExpression&) = 0;
		virtual void visit(ast::AssignmentExpression&) = 0;
		virtual void visit(ast::CallExpression&) = 0;
		virtual void visit(ast::ConditionalExpression&) = 0;
		virtual void visit(ast::MemberExpression&) = 0;
		virtual void visit(ast::UnaryExpression&) = 0;
		virtual void visit(ast::VectorExpression&) = 0;
		virtual void visit(ast::ArrayExpression&) = 0;
		virtual void visit(ast::DeveloperBlock&) = 0;
	};

}; // namespace compiler