#include "bytecode_generator.h"
#include <script/ast/nodes.h>

namespace script
{
	namespace compiler
	{
		BytecodeGenerator::BytecodeGenerator()
		{
		}
		void BytecodeGenerator::visit(ast::Program& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::FunctionDeclaration& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::SwitchCase&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::Directive& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::BlockStatement& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::IfStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::WhileStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::ForStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::DoWhileStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::ReturnStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::BreakStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::WaitStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::WaitTillFrameEndStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::ExpressionStatement& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::EmptyStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::ContinueStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::SwitchStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::LocalizedString&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::Literal&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::Identifier&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::FunctionPointer&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::BinaryExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::AssignmentExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::CallExpression& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::ConditionalExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::MemberExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::UnaryExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::VectorExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::ArrayExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}
	}; // namespace compiler
}; // namespace script