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
						debug = fun_iter.second->debug;
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
			m_function->file = m_currentfile;
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
			{
				std::stringstream ss;
				GSCWriter wr(ss);
				wr.visit_statement(stmt);
				debug.expression_string = ss.str();
				stmt->accept(*this);
			}
		}

		void Compiler::visit(ast::IfStatement& n)
		{
			n.test->accept(*this);
			auto test = instruction<Test>();
			add(test);
			auto jz = instruction<JumpZero>();
			auto skip = label();
			jz->dest = skip;
			add(jz);
			n.consequent->accept(*this);
			std::shared_ptr<vm::Label> jmp_label;
			if (n.alternative)
			{
				auto constant0 = instruction<Constant0>();
				add(constant0);

				jmp_label = label();
				auto jmp = instruction<Jump>();
				jmp->dest = jmp_label;
				add(jmp);
			}

			add(skip);

			if (n.alternative)
			{
				auto constant1 = instruction<Constant1>();
				add(constant1);

				if (jmp_label)
					add(jmp_label);

				auto test2 = instruction<Test>();
				add(test2);
				auto jz2 = instruction<JumpZero>();
				auto skip2 = label();
				jz2->dest = skip2;
				add(jz2);
				n.alternative->accept(*this);
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
			{
				n.init->accept(*this);
				auto instr = instruction<Pop>();
				add(instr);
			}
			auto beg = label();
			auto end = label();
			auto end_of_for = label();
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
			continue_labels.push(end_of_for);
			exit_labels.push(end);
			n.body->accept(*this);
			exit_labels.pop();
			continue_labels.pop();
			auto jmp = instruction<Jump>();
			jmp->dest = beg;

			add(end_of_for);

			if (n.update)
			{
				n.update->accept(*this);
				auto instr = instruction<Pop>();
				add(instr);
			}
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
			else
			{
				auto instr = instruction<PushUndefined>();
				add(instr);
			}
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
				auto instr = instruction<BinOp>();
				instr->op = parse::TokenType_kEq;
				add(instr);
				auto test = instruction<Test>();
				add(test);
				auto jz = instruction<JumpZero>();
				jz->dest = (i+1) >= labels.size() ? end : labels[i + 1];
				add(jz);

				for (auto& stmt : sc->consequent)
				{
					exit_labels.push(end);
					stmt->accept(*this);
					exit_labels.pop();
				}
				auto jmp = instruction<Jump>();
				jmp->dest = end;
				add(jmp);
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
			if (n.op == parse::TokenType_kAndAnd)
			{
				n.left->accept(*this);
				
				//check if left is > 0

				auto test = instruction<Test>();
				add(test);

				auto jz = instruction<JumpZero>();
				
				auto skip = label();
				jz->dest = skip;

				add(jz);

				//if we didn't jump (jz), then evaluate rhs
				n.right->accept(*this);
				//evaluate rhs and if it's not zero, then the result is 1

				auto test_right = instruction<Test>();
				add(test_right);

				auto jz_right = instruction<JumpZero>();

				jz_right->dest = skip;

				add(jz_right);

				auto constant1 = instruction<Constant1>();
				add(constant1);

				auto ed = label();
				auto jmp = instruction<Jump>();
				jmp->dest = ed;
				add(jmp);

				add(skip);

				auto constant0 = instruction<Constant0>();
				add(constant0);
				add(ed);
			}
			else
			{
				n.right->accept(*this);
				n.left->accept(*this);
				auto instr = instruction<BinOp>();
				instr->op = n.op;
				add(instr);
			}
		}

		bool get_property(ast::Expression& n, std::string& prop, int op)
		{
			//auto* id = dynamic_cast<ast::Identifier*>(&n);
			auto* id = n.cast<ast::Identifier>();
			if (id && op == '.')
			{
				prop = id->name;
				return true;
			}
			//auto* lit = dynamic_cast<ast::Literal*>(&n);
			auto* lit = n.cast<ast::Literal>();
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
				instr->op = n.op;
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
				n.rhs->accept(*this);
				LValueVisitor vis(this);
				n.lhs->accept(vis);
			}
			else
			{
				n.rhs->accept(*this);
				n.lhs->accept(*this);

				auto instr = instruction<BinOp>();
				switch (n.op)
				{
				case parse::TokenType_kMinusAssign:
					instr->op = '-';
					break;
				case parse::TokenType_kPlusAssign:
					instr->op = '+';
					break;
				case parse::TokenType_kMultiplyAssign:
					instr->op = '*';
					break;
				case parse::TokenType_kDivideAssign:
					instr->op = '/';
					break;
				case parse::TokenType_kModAssign:
					instr->op = '%';
					break;
				case parse::TokenType_kOrAssign:
					instr->op = '|';
					break;
				case parse::TokenType_kXorAssign:
					instr->op = '^';
					break;
				case parse::TokenType_kAndAssign:
					instr->op = '&';
					break;
				default:
					throw CompileException("unhandled operator {}", n.op);
					break;
				}
				add(instr);
				LValueVisitor vis(this);
				n.lhs->accept(vis);
			}
			auto instr = instruction<StoreRef>();
			add(instr);
			LValueVisitor vis(this);
			n.lhs->accept(vis);
		}

		void Compiler::handle_waittill(ast::CallExpression& n)
		{
			if (n.arguments.size() < 1)
				throw CompileException("waittill requires a minimum of 1 argument");
			auto instr = instruction<WaitTill>();
			for (size_t i = 1; i < n.arguments.size(); ++i)
			{
				//auto* id = dynamic_cast<ast::Identifier*>(n.arguments[i].get());
				auto* id = n.arguments[i]->cast<ast::Identifier>();
				if (!id)
					throw CompileException("expected identifier");
				if (!id->file_reference.empty())
				{
					throw CompileException("unexpected file reference");
				}

				auto ps = instruction<PushString>();
				ps->value = id->name;
				ps->length = id->name.size();
				add(ps);
			}
			instr->is_method_call = n.object != nullptr;
			instr->numargs = n.arguments.size() - 1;
			n.arguments[0]->accept(*this);
			if (instr->is_method_call)
			{
				n.object->accept(*this);
			}
			add(instr);
		}

		void Compiler::visit(ast::CallExpression& n)
		{
			//auto* id = dynamic_cast<ast::Identifier*>(n.callee.get());
			auto* id = n.callee->cast<ast::Identifier>();
			if (id && id->name == "waittill")
			{
				if (!id->file_reference.empty())
					throw CompileException("expected empty file reference");
				handle_waittill(n);
				return;
			}
			for (auto it = n.arguments.rbegin(); it != n.arguments.rend(); ++it)
				(*it)->accept(*this);
			std::shared_ptr<Call> instr;

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
					if (!n.pointer)
					{
						auto call = instruction<CallFunction>();
						call->function = id->name;
						instr = std::move(call);
					}
					else
					{
						n.callee->accept(*this);
						auto call = instruction<CallFunctionPointer>();
						instr = std::move(call);
					}
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
			instr->op = n.op;
			add(instr);
		}

		void Compiler::visit(ast::UnaryExpression& n)
		{
			switch (n.op)
			{
			case '-':
			{
				n.argument->accept(*this);
				auto constant0 = instruction<Constant0>();
				add(constant0);
				auto instr = instruction<BinOp>();
				instr->op = '-';
				add(instr);
			} break;
			case '!':
			{
				n.argument->accept(*this);
				auto instr = instruction<LogicalNot>();
				add(instr);
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
				if (n.op == parse::TokenType_kPlusPlus)
				{
					auto instr = instruction<BinOp>();
					instr->op = '+';
					add(instr);
				}
				else
				{
					auto instr = instruction<BinOp>();
					instr->op = '-';
					add(instr);
				}
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