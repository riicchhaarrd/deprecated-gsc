#include "bytecode_generator.h"
#include <script/ast/nodes.h>

namespace script
{
	namespace compiler
	{
		BytecodeGenerator::BytecodeGenerator(TraverseInfo& ti_, vm::Constants& constants,
											 std::vector<vm::Function>& functions)
			: ti(ti_), m_constants(constants), m_functions(functions)
		{
			auto &fi = ti.function_visitor.function_info();
			m_functions.resize(fi.size());

			size_t i = 0;
			for (auto& it : fi)
			{
				m_function_by_name[it.first] = &m_functions[i++];
				m_function_by_name[it.first]->index = i - 1;
				m_function_by_name[it.first]->name = it.second.name;
				m_function_by_name[it.first]->numparameters = it.second.numparameters;
			}

		}
		void BytecodeGenerator::visit(ast::Program& n)
		{
			for (auto& it : n.body)
				it->accept(*this);
		}

		void BytecodeGenerator::visit(ast::FunctionDeclaration& n)
		{
			m_function = m_function_by_name[n.function_name];
			n.body->accept(*this);
		}

		void BytecodeGenerator::visit(ast::SwitchCase&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void BytecodeGenerator::visit(ast::Directive& n)
		{
			//throw CompileException("unimplemented {}", __LINE__);
			printf("directive unimplemented, skipping\n");
		}

		void BytecodeGenerator::visit(ast::BlockStatement& n)
		{
			//TODO: maybe cleanup variables that go out of scope (atm its just all variables are locally bound to the function)
			for (auto& it : n.body)
				it->accept(*this);
		}

		void BytecodeGenerator::visit(ast::IfStatement& n)
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
			n.expression->accept(*this);
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
			std::unique_ptr<vm::CallInstruction> instr;
			auto *id = dynamic_cast<ast::Identifier*>(n.callee.get());
			if (id) //regular function call
			{
				std::string function_name = id->name;
				if (!id->file_reference.empty())
					throw CompileException("external calls not supported yet");

				auto* fi = ti.function_visitor.get_function_info(function_name);
				if (!fi)
				{
					if (n.threaded)
						throw CompileException("Function {} does not exist", id->full_identifier_string());
					if (!id->file_reference.empty())
						throw CompileException("Function {} does not exist", id->full_identifier_string());

					auto c = instruction<vm::CallBuiltinInstruction>();
					c->name = register_string(function_name);
					instr = std::move(c);
				}
				else
				{
					auto c = instruction<vm::CallFunctionInstruction>();
					c->function_index = m_function_by_name[function_name]->index;
					instr = std::move(c);
				}

			}
			instr->is_threaded = n.threaded;
			instr->is_method_call = n.object != nullptr;
			//TODO: FIXME
			add(instr);
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