#pragma once

namespace script
{
	namespace ast
	{
		struct Node;
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
		struct ContinueStatement;
		struct SwitchStatement;
		struct SwitchCase;
		struct Directive;

		class ASTVisitor
		{
		  public:
			virtual ~ASTVisitor()
			{
			}
			void visit_node(Node& n);
			virtual void visit(Program&) = 0;
			virtual void visit(FunctionDeclaration&) = 0;
			virtual void visit(SwitchCase&) = 0;
			virtual void visit(Directive&) = 0;

			// statements
			virtual void visit(BlockStatement&) = 0;
			virtual void visit(IfStatement&) = 0;
			virtual void visit(WhileStatement&) = 0;
			virtual void visit(ForStatement&) = 0;
			virtual void visit(DoWhileStatement&) = 0;
			virtual void visit(ReturnStatement&) = 0;
			virtual void visit(BreakStatement&) = 0;
			virtual void visit(WaitStatement&) = 0;
			virtual void visit(WaitTillFrameEndStatement&) = 0;
			virtual void visit(ExpressionStatement&) = 0;
			virtual void visit(EmptyStatement&) = 0;
			virtual void visit(ContinueStatement&) = 0;
			virtual void visit(SwitchStatement&) = 0;

			// expressions
			virtual void visit(LocalizedString&) = 0;
			virtual void visit(Literal&) = 0;
			virtual void visit(Identifier&) = 0;
			virtual void visit(FunctionPointer&) = 0;
			virtual void visit(BinaryExpression&) = 0;
			virtual void visit(AssignmentExpression&) = 0;
			virtual void visit(CallExpression&) = 0;
			virtual void visit(ConditionalExpression&) = 0;
			virtual void visit(MemberExpression&) = 0;
			virtual void visit(UnaryExpression&) = 0;
			virtual void visit(VectorExpression&) = 0;
			virtual void visit(ArrayExpression&) = 0;
		};
	}; // namespace ast
}; // namespace script