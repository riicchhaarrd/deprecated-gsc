#pragma once
#include <script/ast/visitor.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <stack>
#include <script/vm/instructions/instructions.h>
#include <script/vm/types.h>
#include "traverse_info.h"

#include <script/vm/function.h>
#include <script/vm/instruction.h>

namespace script
{
	struct LoadedProgramReference
	{
		std::unique_ptr<ast::Program> program;
		std::string name;
		std::unordered_map<std::string, ast::FunctionDeclaration*> function_map;
	};
	using ReferenceMap = std::unordered_map<std::string, LoadedProgramReference>;

	namespace compiler
	{
		struct CompiledFunction
		{
			std::string name;
			std::vector<std::string> parameters;
			std::vector<std::shared_ptr<vm::Instruction>> instructions;
		};
		using CompiledFunctions = std::unordered_map<std::string, CompiledFunction>;
		using CompiledFiles = std::unordered_map<std::string, CompiledFunctions>;

		class Compiler : public ast::ASTVisitor
		{
			script::ReferenceMap& m_refmap;
			CompiledFiles m_files;
			CompiledFunctions *m_compiledfunctions;
			CompiledFunction *m_function;
			std::string m_currentfile;
			std::stack<std::weak_ptr<vm::Label>> exit_labels;
			std::stack<std::weak_ptr<vm::Label>> continue_labels;
			ast::ExpressionStatement* last_expression_statement = nullptr;
			size_t label_index = 0;
		  public:
			Compiler(script::ReferenceMap&);
			CompiledFiles compile();

			template <typename T, typename... Ts> std::shared_ptr<T> instruction(Ts... ts)
			{
				// printf("node(%s)\n", typeid(T).name());
				return std::move(std::make_shared<T>(ts...));
			}
			std::shared_ptr<vm::Label> label()
			{
				auto l = std::make_shared<vm::Label>();
				l->label_index = label_index++;
				return std::move(l);
			}

			template <typename T>
			void add(std::shared_ptr<T>& t)
			{
				m_function->instructions.push_back(t);
			}

			size_t register_string(const vm::String s)
			{
				return 0;
				#if 0
				auto fnd = strings.find(s);
				if (fnd == strings.end())
				{
					strings[s] = register_constant(s);
				}
				return strings[s];
				#endif
			}
			template <typename T> size_t register_constant(T t)
			{
#if 0
				vm::Variant u;
				u = t;
				m_constants.push_back(u);
				return m_constants.size() - 1;
				#endif
				return 0;
			}

			// Inherited via ASTVisitor
			virtual void visit(ast::Program&) override;
			virtual void visit(ast::FunctionDeclaration&) override;
			virtual void visit(ast::SwitchCase&) override;
			virtual void visit(ast::Directive&) override;
			virtual void visit(ast::BlockStatement&) override;
			virtual void visit(ast::IfStatement&) override;
			virtual void visit(ast::WhileStatement&) override;
			virtual void visit(ast::ForStatement&) override;
			virtual void visit(ast::DoWhileStatement&) override;
			virtual void visit(ast::ReturnStatement&) override;
			virtual void visit(ast::BreakStatement&) override;
			virtual void visit(ast::WaitStatement&) override;
			virtual void visit(ast::WaitTillFrameEndStatement&) override;
			virtual void visit(ast::ExpressionStatement&) override;
			virtual void visit(ast::EmptyStatement&) override;
			virtual void visit(ast::ContinueStatement&) override;
			virtual void visit(ast::SwitchStatement&) override;
			virtual void visit(ast::LocalizedString&) override;
			virtual void visit(ast::Literal&) override;
			virtual void visit(ast::Identifier&) override;
			virtual void visit(ast::FunctionPointer&) override;
			virtual void visit(ast::BinaryExpression&) override;
			virtual void visit(ast::AssignmentExpression&) override;
			void handle_waittill(ast::CallExpression& n);
			virtual void visit(ast::CallExpression&) override;
			virtual void visit(ast::ConditionalExpression&) override;
			virtual void visit(ast::MemberExpression&) override;
			virtual void visit(ast::UnaryExpression&) override;
			virtual void visit(ast::VectorExpression&) override;
			virtual void visit(ast::ArrayExpression&) override;
		};
	};
};