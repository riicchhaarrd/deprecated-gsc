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

namespace script
{
	namespace ast
	{
		class RecursiveASTVisitor : public ASTVisitor
		{
			// Inherited via ASTVisitor
			virtual void visit(FunctionDeclaration&) override;
			virtual void visit(BlockStatement&) override;
			virtual void visit(IfStatement&) override;
			virtual void visit(WhileStatement&) override;
			virtual void visit(ForStatement&) override;
			virtual void visit(DoWhileStatement&) override;
			virtual void visit(ReturnStatement&) override;
			virtual void visit(ContinueStatement&) override;
			virtual void visit(BreakStatement&) override;
			virtual void visit(WaitStatement&) override;
			virtual void visit(WaitTillFrameEndStatement&) override;
			virtual void visit(ExpressionStatement&) override;
			virtual void visit(EmptyStatement&) override;
			virtual void visit(LocalizedString&) override;
			virtual void visit(Literal&) override;
			virtual void visit(Identifier&) override;
			virtual void visit(FunctionPointer&) override;
			virtual void visit(BinaryExpression&) override;
			virtual void visit(AssignmentExpression&) override;
			virtual void visit(CallExpression&) override;
			virtual void visit(ConditionalExpression&) override;
			virtual void visit(MemberExpression&) override;
			virtual void visit(UnaryExpression&) override;
			virtual void visit(VectorExpression&) override;
			virtual void visit(ArrayExpression&) override;
			virtual void visit(Program&) override;
			virtual void visit(SwitchCase&) override;
			virtual void visit(SwitchStatement&) override;

		  public:
			virtual bool pre_visit(SwitchCase&)
			{
				return true;
			}
			virtual bool pre_visit(SwitchStatement&)
			{
				return true;
			}
			virtual bool pre_visit(ContinueStatement&)
			{
				return true;
			}
			virtual bool pre_visit(Program&)
			{
				return true;
			}
			virtual bool pre_visit(FunctionDeclaration&)
			{
				return true;
			}
			virtual bool pre_visit(BlockStatement&)
			{
				return true;
			}
			virtual bool pre_visit(IfStatement&)
			{
				return true;
			}
			virtual bool pre_visit(WhileStatement&)
			{
				return true;
			}
			virtual bool pre_visit(ForStatement&)
			{
				return true;
			}
			virtual bool pre_visit(DoWhileStatement&)
			{
				return true;
			}
			virtual bool pre_visit(ReturnStatement&)
			{
				return true;
			}
			virtual bool pre_visit(BreakStatement&)
			{
				return true;
			}
			virtual bool pre_visit(WaitStatement&)
			{
				return true;
			}
			virtual bool pre_visit(WaitTillFrameEndStatement&)
			{
				return true;
			}
			virtual bool pre_visit(ExpressionStatement&)
			{
				return true;
			}
			virtual bool pre_visit(EmptyStatement&)
			{
				return true;
			}
			virtual bool pre_visit(LocalizedString&)
			{
				return true;
			}
			virtual bool pre_visit(Literal&)
			{
				return true;
			}
			virtual bool pre_visit(Identifier&)
			{
				return true;
			}
			virtual bool pre_visit(FunctionPointer&)
			{
				return true;
			}
			virtual bool pre_visit(BinaryExpression&)
			{
				return true;
			}
			virtual bool pre_visit(AssignmentExpression&)
			{
				return true;
			}
			virtual bool pre_visit(CallExpression&)
			{
				return true;
			}
			virtual bool pre_visit(ConditionalExpression&)
			{
				return true;
			}
			virtual bool pre_visit(MemberExpression&)
			{
				return true;
			}
			virtual bool pre_visit(UnaryExpression&)
			{
				return true;
			}
			virtual bool pre_visit(VectorExpression&)
			{
				return true;
			}
			virtual bool pre_visit(ArrayExpression&)
			{
				return true;
			}

			virtual void post_visit(Program&)
			{
			}
			virtual void post_visit(FunctionDeclaration&)
			{
			}
			virtual void post_visit(BlockStatement&)
			{
			}
			virtual void post_visit(IfStatement&)
			{
			}
			virtual void post_visit(WhileStatement&)
			{
			}
			virtual void post_visit(ForStatement&)
			{
			}
			virtual void post_visit(DoWhileStatement&)
			{
			}
			virtual void post_visit(ReturnStatement&)
			{
			}
			virtual void post_visit(BreakStatement&)
			{
			}
			virtual void post_visit(WaitStatement&)
			{
			}
			virtual void post_visit(WaitTillFrameEndStatement&)
			{
			}
			virtual void post_visit(ExpressionStatement&)
			{
			}
			virtual void post_visit(EmptyStatement&)
			{
			}
			virtual void post_visit(LocalizedString&)
			{
			}
			virtual void post_visit(Literal&)
			{
			}
			virtual void post_visit(Identifier&)
			{
			}
			virtual void post_visit(FunctionPointer&)
			{
			}
			virtual void post_visit(BinaryExpression&)
			{
			}
			virtual void post_visit(AssignmentExpression&)
			{
			}
			virtual void post_visit(CallExpression&)
			{
			}
			virtual void post_visit(ConditionalExpression&)
			{
			}
			virtual void post_visit(MemberExpression&)
			{
			}
			virtual void post_visit(UnaryExpression&)
			{
			}
			virtual void post_visit(VectorExpression&)
			{
			}
			virtual void post_visit(ArrayExpression&)
			{
			}
			virtual void post_visit(ContinueStatement&)
			{
			}
			virtual void post_visit(SwitchCase&)
			{
			}
			virtual void post_visit(SwitchStatement&)
			{
			}
			void visit(Node& n)
			{
				n.accept(*this);
			}
		};
	}; // namespace ast
}; // namespace compiler