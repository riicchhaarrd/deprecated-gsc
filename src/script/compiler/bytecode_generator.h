#pragma once
#include <script/ast/visitor.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <script/vm/instructions/instructions.h>
#include <script/vm/types.h>
#include "traverse_info.h"

#include <script/vm/function.h>

namespace script
{
	namespace compiler
	{
		class BytecodeGenerator : public ast::ASTVisitor
		{

		  public:
			BytecodeGenerator();

			template <typename T, typename... Ts> std::unique_ptr<T> instruction(Ts... ts)
			{
				// printf("node(%s)\n", typeid(T).name());
				return std::move(std::make_unique<T>(ts...));
			}

			template <typename T>
			void add(std::unique_ptr<T>& instr)
			{
				m_function->instructions.push_back(std::move(instr));
			}

			size_t register_string(const vm::String s)
			{
				auto fnd = strings.find(s);
				if (fnd == strings.end())
				{
					strings[s] = register_constant(s);
				}
				return strings[s];
			}
			template <typename T> size_t register_constant(T t)
			{
				vm::Variant u;
				u = t;
				m_constants.push_back(u);
				return m_constants.size() - 1;
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
			virtual void visit(ast::CallExpression&) override;
			virtual void visit(ast::ConditionalExpression&) override;
			virtual void visit(ast::MemberExpression&) override;
			virtual void visit(ast::UnaryExpression&) override;
			virtual void visit(ast::VectorExpression&) override;
			virtual void visit(ast::ArrayExpression&) override;
		};
	};
};