#include "compiler.h"
#include <script/ast/nodes.h>
#include <parse/token.h>
#include <script/ast/gsc_writer.h>
#include <iostream>

namespace script
{
	namespace compiler
	{
		class CompileVisitor : public ast::ASTVisitor
		{
		  public:
			virtual void visit(ast::Program& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::FunctionDeclaration& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::SwitchCase& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::Directive& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}

			// statements
			virtual void visit(ast::BlockStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::IfStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::WhileStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::ForStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::DoWhileStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::ReturnStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::BreakStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::WaitStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::WaitTillFrameEndStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::ExpressionStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::EmptyStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::ContinueStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::SwitchStatement& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}

			// expressions
			virtual void visit(ast::LocalizedString& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::Literal& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::Identifier& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::FunctionPointer& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::BinaryExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::AssignmentExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::CallExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::ConditionalExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::MemberExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::UnaryExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::VectorExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
			virtual void visit(ast::ArrayExpression& n)
			{
				throw CompileException("invalid node {}", __LINE__);
			}
		};

		using namespace vm;

		Compiler::Compiler(script::ReferenceMap& refmap) : m_refmap(refmap)
		{
		}
		CompiledFiles Compiler::compile()
		{
			ast::FunctionDeclaration* func = nullptr;
			std::string file;
			try
			{
				for (auto& refmap_iter : m_refmap)
				{
					file = refmap_iter.first;
					m_compiledfunctions = &m_files[util::string::to_lower(file)];
					printf("compiling program %s\n", refmap_iter.first.c_str());
					m_currentfile = refmap_iter.first;
					for (auto& fun_iter : refmap_iter.second.function_map)
					{
						func = fun_iter.second;
						//printf("\tcompiling function: %s\n", fun_iter.first.c_str());
						fun_iter.second->accept(*this);
					}
				}
				printf("-------------------------------------------------------------------------------\n");
				printf("Compile done!\n");
				#if 0
				for (auto& cf : m_compiledfunctions)
				{
					printf("===function: %s\n", cf.first.c_str());
					for (auto& instr : cf.second.instructions)
					{
						printf("\t%s\n", instr->to_string().c_str());
					}
				}
				#endif
			}
			catch (CompileException& e)
			{
				printf("===============================================================================\n");
				printf("File: %s\n", file.c_str());
				if (last_expression_statement)
				{
					GSCWriter wr(std::cout);
					wr.visit(*last_expression_statement);
				}
				printf("Failed to compile %s\n", e.what());
				printf("===============================================================================\n");
			}
			return m_files;
		}
		void Compiler::visit(ast::Program& n)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::FunctionDeclaration& n)
		{
			m_function = &(*m_compiledfunctions)[util::string::to_lower(n.function_name)];
			m_function->name = n.function_name;
			m_function->parameters = n.parameters;
			n.body->accept(*this);
			auto instr = instruction<PushUndefined>();
			add(instr);
			auto ret = instruction<Ret>();
			add(ret);
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
			for (auto& stmt : n.body)
				stmt->accept(*this);
		}

		void Compiler::visit(ast::IfStatement& n)
		{
			n.test->accept(*this);
			auto test = instruction<Test>();
			add(test);
			auto jz = instruction<JumpZero>();
			add(jz);
			n.consequent->accept(*this);
			auto skip = label();
			jz->dest = skip;
			add(skip);
			if (n.alternative)
			{
				n.test->accept(*this);
				auto test2 = instruction<Test>();
				add(test2);
				auto jz2 = instruction<JumpNotZero>();
				add(jz2);
				n.consequent->accept(*this);
				auto skip2 = label();
				jz2->dest = skip2;
				add(skip2);
			}
		}

		void Compiler::visit(ast::WhileStatement& n)
		{
			auto beg = label();
			auto end = label();
			add(beg);
			n.test->accept(*this);
			auto test = instruction<Test>();
			add(test);
			auto jz = instruction<JumpZero>();
			jz->dest = end;
			add(jz);
			continue_labels.push(beg);
			exit_labels.push(end);
			n.body->accept(*this);
			exit_labels.pop();
			continue_labels.pop();
			auto jmp = instruction<Jump>();
			jmp->dest = beg;
			add(jmp);
			add(end);
		}

		void Compiler::visit(ast::ForStatement& n)
		{
			if (n.init)
				n.init->accept(*this);
			auto beg = label();
			auto end = label();
			add(beg);
			if (n.test)
				n.test->accept(*this);
			else
			{
				auto constant1 = instruction<Constant1>();
				add(constant1);
			}
			auto test = instruction<Test>();
			add(test);
			auto jz = instruction<JumpZero>();
			jz->dest = end;
			add(jz);
			continue_labels.push(beg);
			exit_labels.push(end);
			n.body->accept(*this);
			exit_labels.pop();
			continue_labels.pop();
			auto jmp = instruction<Jump>();
			jmp->dest = beg;
			if (n.update)
				n.update->accept(*this);
			add(jmp);
			add(end);
		}

		void Compiler::visit(ast::DoWhileStatement&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::ReturnStatement& n)
		{
			if (n.argument)
				n.argument->accept(*this);
			auto instr = instruction<Ret>();
			add(instr);
		}

		void Compiler::visit(ast::BreakStatement& n)
		{
			if (exit_labels.empty())
				throw CompileException("no exit label for break statement");
			auto instr = instruction<Jump>();
			instr->dest = exit_labels.top();
			add(instr);
		}

		void Compiler::visit(ast::WaitStatement& n)
		{
			n.duration->accept(*this);
			auto instr = instruction<Wait>();
			add(instr);
		}

		void Compiler::visit(ast::WaitTillFrameEndStatement&)
		{
			auto instr = instruction<WaitTillFrameEnd>();
			add(instr);
		}

		void Compiler::visit(ast::ExpressionStatement& n)
		{
			last_expression_statement = &n;
			n.expression->accept(*this);
			auto instr = instruction<Pop>();
			add(instr);
		}

		void Compiler::visit(ast::EmptyStatement& n)
		{
			//auto instr = instruction<Nop>();
			//add(instr);
		}

		void Compiler::visit(ast::ContinueStatement& n)
		{
			if (continue_labels.empty())
				throw CompileException("no label for continue statement");
			auto instr = instruction<Jump>();
			instr->dest = continue_labels.top();
			add(instr);
		}

		void Compiler::visit(ast::SwitchStatement& n)
		{
			auto end = label();
			ast::SwitchCase* default_switch_case = nullptr;

			std::vector<std::shared_ptr<Label>> labels;
			for (size_t i = 0; i < n.cases.size(); ++i)
			{
				labels.push_back(label());
			}

			size_t numcases = 0;
			for (size_t i = 0; i < n.cases.size(); ++i)
			{
				auto& sc = n.cases[i];
				if (!sc->test)
				{
					default_switch_case = sc.get();
					continue;
				}
				if (i > 0)
					add(labels[i]);
				n.discriminant->accept(*this);
				sc->test->accept(*this);
				auto cmp = instruction<Compare>();
				add(cmp);
				auto jnz = instruction<JumpNotZero>();
				jnz->dest = (i+1) >= labels.size() ? end : labels[i + 1];
				add(jnz);

				for (auto& stmt : sc->consequent)
				{
					exit_labels.push(end);
					stmt->accept(*this);
					exit_labels.pop();
				}
				auto jmp = instruction<Jump>();
				jmp->dest = end;
			}
			//add default case
			if (default_switch_case)
			{
				add(labels[labels.size() - 1]);
				for (auto& stmt : default_switch_case->consequent)
				{
					exit_labels.push(end);
					stmt->accept(*this);
					exit_labels.pop();
				}
			}
			add(end);
		}

		void Compiler::visit(ast::LocalizedString& n)
		{
			auto instr = instruction<PushLocalizedString>();
			instr->value = n.reference;
			instr->length = n.reference.size();
			add(instr);
		}

		void Compiler::visit(ast::Literal& n)
		{
			switch (n.type)
			{
			case ast::Literal::Type::kInteger:
			{
				auto instr = instruction<PushInteger>();
				instr->value = atoi(n.value.c_str());
				add(instr);
			} break;
			case ast::Literal::Type::kNumber:
			{
				auto instr = instruction<PushNumber>();
				instr->value = atof(n.value.c_str());
				add(instr);
			} break;
			case ast::Literal::Type::kString:
			{
				auto instr = instruction<PushString>();
				instr->value = n.value;
				instr->length = n.value.size();
				add(instr);
			} break;
			case ast::Literal::Type::kAnimation:
			{
				auto instr = instruction<PushAnimationString>();
				instr->value = n.value;
				add(instr);
			} break;
			case ast::Literal::Type::kUndefined:
			{
				auto instr = instruction<PushUndefined>();
				add(instr);
			} break;
			default:
				throw CompileException("unhandled literal type {}", (int)n.type);
				break;
			}
		}

		void Compiler::visit(ast::Identifier& n)
		{
			if (!n.file_reference.empty())
			{
				auto instr = instruction<PushFunctionPointer>();
				instr->file = n.file_reference;
				instr->function = n.name;
				add(instr);
			}
			else
			{
				auto instr = instruction<LoadValue>();
				instr->variable_name = n.name;
				add(instr);
			}
		}

		void Compiler::visit(ast::FunctionPointer& n)
		{
			auto instr = instruction<PushFunctionPointer>();
			instr->file = m_currentfile;
			instr->function = n.function_name;
			add(instr);
		}

		void Compiler::visit(ast::BinaryExpression& n)
		{
			n.left->accept(*this);
			n.right->accept(*this);
			switch (n.op)
			{
				// got "lazy" and just wanted to get things working so i'm kinda mapping ast nodes 1:1 to instructions
				// now
				case parse::TokenType_kAndAnd:
				case parse::TokenType_kOrOr:
				case parse::TokenType_kGeq:
				case parse::TokenType_kLeq:
				case '<':
				case '>':
				{
					auto instr = instruction<BinOp>();
					instr->op = n.op;
					add(instr);
				} break;
				case '+':
				{
					auto instr = instruction<Add>();
					add(instr);
				} break;
				case '&':
				{
					auto instr = instruction<And>();
					add(instr);
				} break;
				case '|':
				{
					auto instr = instruction<Or>();
					add(instr);
				} break;
				case '-':
				{
					auto instr = instruction<Sub>();
					add(instr);
				} break;
				case parse::TokenType_kEq:
				{
					auto instr = instruction<Compare>();
					add(instr);
					auto jz = instruction<JumpZero>();
					auto l = label();
					jz->dest = l;
					add(jz);
					auto constant0 = instruction<Constant0>();
					add(constant0);
					auto jnz = instruction<JumpNotZero>();
					auto label2 = label();
					jnz->dest = label2;
					add(jnz);
					add(l);
					auto constant1 = instruction<Constant1>();
					add(constant1);
					add(label2);
				} break;
				case parse::TokenType_kNeq:
				{
					auto instr = instruction<Compare>();
					add(instr);
					auto jz = instruction<JumpZero>();
					auto l = label();
					jz->dest = l;
					add(jz);
					auto constant0 = instruction<Constant1>();
					add(constant0);
					auto jnz = instruction<JumpNotZero>();
					auto label2 = label();
					jnz->dest = label2;
					add(jnz);
					add(l);
					auto constant1 = instruction<Constant0>();
					add(constant1);
					add(label2);
				} break;
				case '*':
				{
					auto instr = instruction<Mul>();
					add(instr);
				} break;
				case '/':
				{
					auto instr = instruction<Div>();
					add(instr);
				} break;
				case '%':
				{
					auto instr = instruction<Mod>();
					add(instr);
				} break;
				default:
					throw CompileException("unimplemented operator {}", n.op);
					break;
			}
		}

		bool get_property(ast::Expression& n, std::string& prop, int op)
		{
			auto* id = dynamic_cast<ast::Identifier*>(&n);
			if (id && op == '.')
			{
				prop = id->name;
				return true;
			}
			auto* lit = dynamic_cast<ast::Literal*>(&n);
			if (lit)
			{
				if (lit->type == ast::Literal::Type::kInteger || lit->type == ast::Literal::Type::kString)
				{
					prop = lit->value;
					return true;
				}
			}
			return false;
		}
		class LValueVisitor : public CompileVisitor
		{
			Compiler* compiler;
		  public:
			LValueVisitor(Compiler* c) : compiler(c)
			{
			}
			virtual void visit(ast::MemberExpression& n)
			{
				std::string field_name;
				if (!get_property(*n.prop.get(), field_name, n.op))
				{
					n.prop->accept(*compiler);
				}
				else
				{
					auto instr = compiler->instruction<PushString>();
					instr->value = field_name;
					instr->length = field_name.size();
					compiler->add(instr);
				}
				n.object->accept(*this);
				auto instr = compiler->instruction<LoadObjectFieldRef>();
				compiler->add(instr);
			}
			virtual void visit(ast::Identifier& n)
			{
				if (!n.file_reference.empty())
				{
					throw CompileException("unsupported file reference in lvalue expression");
				}
				else
				{
					auto instr = compiler->instruction<LoadRef>();
					instr->variable_name = n.name;
					compiler->add(instr);
				}
			}
		};

		void Compiler::visit(ast::AssignmentExpression& n)
		{
			if (n.op == '=')
			{
				LValueVisitor vis(this);
				n.lhs->accept(vis);

				n.rhs->accept(*this);

			}
			else
			{
				LValueVisitor vis(this);
				n.lhs->accept(vis);

				n.rhs->accept(*this);
				n.lhs->accept(*this);
				switch (n.op)
				{
				case parse::TokenType_kPlusAssign:
				{
					auto instr = instruction<Add>();
					add(instr);
				}
				break;
				case parse::TokenType_kMinusAssign:
				{
					auto instr = instruction<Sub>();
					add(instr);
				}
				break;
				case parse::TokenType_kMultiplyAssign:
				{
					auto instr = instruction<Mul>();
					add(instr);
				}
				break;
				case parse::TokenType_kDivideAssign:
				{
					auto instr = instruction<Div>();
					add(instr);
				}
				break;
				case parse::TokenType_kModAssign:
				{
					auto instr = instruction<Mod>();
					add(instr);
				}
				break;
				default:
					throw CompileException("unimplemented operator {}", n.op);
					break;
				}
			}
			auto instr = instruction<StoreRef>();
			add(instr);
			LValueVisitor vis(this);
			n.lhs->accept(vis);
		}

		void Compiler::visit(ast::CallExpression& n)
		{
			for (auto it = n.arguments.rbegin(); it != n.arguments.rend(); ++it)
				(*it)->accept(*this);
			std::shared_ptr<Call> instr;

			auto* id = dynamic_cast<ast::Identifier*>(n.callee.get());
			if (id)
			{
				if (!id->file_reference.empty())
				{
					auto call = instruction<CallFunctionFile>();
					call->file = id->file_reference;
					call->function = id->name;
					instr = std::move(call);
				}
				else
				{
					auto call = instruction<CallFunction>();
					call->function = id->name;
					instr = std::move(call);
				}
			}
			else
			{
				if (!n.pointer)
				{
					throw CompileException("expected pointer");
				}
				n.callee->accept(*this);
				auto call = instruction<CallFunctionPointer>();
				instr = std::move(call);
			}

			instr->numargs = n.arguments.size();
			instr->is_threaded = n.threaded;
			instr->is_method_call = n.object != nullptr;
			if (instr->is_method_call)
			{
				n.object->accept(*this);
			}
			else
			{
				//auto instr = instruction<PushUndefined>();
				//add(instr);
			}
			add(instr);
		}

		void Compiler::visit(ast::ConditionalExpression&)
		{
			throw CompileException("unimplemented {}", __LINE__);
		}

		void Compiler::visit(ast::MemberExpression& n)
		{
			std::string field_name;
			if (!get_property(*n.prop.get(), field_name, n.op))
			{
				n.prop->accept(*this);
			}
			else
			{
				auto instr = instruction<PushString>();
				instr->value = field_name;
				instr->length = field_name.size();
				add(instr);
			}
			n.object->accept(*this);
			auto instr = instruction<LoadObjectFieldValue>();
			add(instr);
		}

		void Compiler::visit(ast::UnaryExpression& n)
		{
			switch (n.op)
			{
			case '-':
			{
				n.argument->accept(*this);
				auto instr = instruction<Negate>();
				add(instr);
			} break;
			case '!':
			{
				n.argument->accept(*this);
				auto test = instruction<Test>();
				add(test);
				auto jz = instruction<JumpZero>();
				add(jz);
				auto constant1 = instruction<Constant1>();
				add(constant1);
				auto jmp = instruction<Jump>();
				auto skip2 = label();
				jmp->dest = skip2;
				auto skip = label();
				jz->dest = skip;
				add(skip);
				auto constant0 = instruction<Constant0>();
				add(constant0);
				add(skip2);
			} break;
			case '~':
			{
				n.argument->accept(*this);
				auto instr = instruction<Not>();
				add(instr);
			} break;
			case parse::TokenType_kPlusPlus:
			case parse::TokenType_kMinusMinus:
			{
				if (n.prefix)
					throw CompileException("unsupported prefix operator -- or ++");
				n.argument->accept(*this);
				auto constant1 = instruction<Constant1>();
				add(constant1);
				std::shared_ptr<vm::Instruction> instr;
				if (n.op == parse::TokenType_kPlusPlus)
					instr = instruction<Add>();
				else
					instr = instruction<Sub>();
				add(instr);
				LValueVisitor vis(this);
				n.argument->accept(vis);
				auto sr = instruction<StoreRef>();
				add(sr);
				n.argument->accept(*this);
			} break;
			default:
				throw CompileException("invalid operator {}", n.op);
				break;
			}
		}

		void Compiler::visit(ast::VectorExpression& n)
		{
			for (auto it = n.elements.rbegin(); it != n.elements.rend(); ++it)
				(*it)->accept(*this);
			auto instr = instruction<PushVector>();
			instr->nelements = n.elements.size();
			add(instr);
		}

		void Compiler::visit(ast::ArrayExpression& n)
		{
			//maybe better to PushArray once then iterate over the elements and then do AddArray or something, probably better for calling functions from C/C++
			for (auto it = n.elements.rbegin(); it != n.elements.rend(); ++it)
				(*it)->accept(*this);
			auto instr = instruction<PushArray>();
			instr->nelements = n.elements.size();
			add(instr);
		}
	}; // namespace compiler
}; // namespace script