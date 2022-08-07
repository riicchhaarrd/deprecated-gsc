#include "compiler.h"
#include <script/ast/nodes.h>

namespace script
{
	namespace compiler
	{
		Compiler::Compiler()
		{
		}
		void Compiler::visit(ast::Program& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::FunctionDeclaration& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::SwitchCase&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::Directive& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::BlockStatement& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::IfStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::WhileStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ForStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::DoWhileStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ReturnStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::BreakStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::WaitStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::WaitTillFrameEndStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ExpressionStatement& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::EmptyStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ContinueStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::SwitchStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::LocalizedString&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::Literal&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::Identifier&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::FunctionPointer&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::BinaryExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::AssignmentExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::CallExpression& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ConditionalExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::MemberExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::UnaryExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::VectorExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ArrayExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}
	}; // namespace compiler
}; // namespace script