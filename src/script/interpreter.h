#pragma once
#include <script/ast/ast_generator.h>
#include <memory>
#include <script/ast/visitor.h>
#include <script/vm/instructions/instructions.h>
#include <script/vm/types.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <functional>

#include <script/vm/function.h>
#include <script/vm/virtual_machine.h>

#include <script/ast/gsc_writer.h>
#include <iostream>

namespace script
{
	struct LoadedProgramReference
	{
		std::unique_ptr<ast::Program> program;
		std::string name;
		std::unordered_map<std::string, ast::FunctionDeclaration*> function_map;
	};
	using FunctionArguments = std::vector<std::shared_ptr<vm::Variant>>;
	using ReferenceMap = std::unordered_map<std::string, LoadedProgramReference>;
	struct VMContext
	{
		virtual std::string get_string(size_t) = 0;
		virtual int get_int(size_t) = 0;
		virtual float get_float(size_t) = 0;
		virtual vm::Variant get_variant(size_t) = 0;
		virtual void add_bool(const bool b)
		{
			add_int(b ? 1 : 0);
		}
		virtual void add_int(const int) = 0;
		virtual void add_float(const float) = 0;
		virtual void add_string(const std::string) = 0;
	};

	using StockFunction = std::function<int(VMContext&, vm::Object*)>;

	class Interpreter : public ast::ASTVisitor
	{
		friend class InterpreterVMContext;
		struct FunctionContext
		{
			std::unordered_map<std::string, std::shared_ptr<vm::Variant>> variables;
		};
		vm::ObjectPtr level_object;
		vm::ObjectPtr game_object;
		std::vector<std::shared_ptr<vm::Variant>> m_stack;
		std::stack<FunctionContext*> m_callstack;
		std::stack<LoadedProgramReference*> m_programstack;
		std::unordered_map<std::string, StockFunction> m_stockfunctions;
		std::unique_ptr<VMContext> m_context;
		script::ReferenceMap& m_refmap;
		LoadedProgramReference* m_program = nullptr;
	  public:
		void dump_node(ast::Node &n)
		{
			std::cout << "\t{";
			GSCWriter wr(std::cout);
			wr.visit_node(n);
			std::cout << "}\n";
		}

		template <typename... Ts>
		void assert_cond(ast::Node& n, bool cond, std::string_view fmt, Ts&&... ts)
		{
			if (cond)
				return;
			GSCWriter wr(std::cout);
			wr.visit_node(n);
			throw vm::Exception(std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...)));
		}
		template <typename... Ts>
		void assert_cond(bool cond, std::string_view fmt, Ts&&... ts)
		{
			if (cond)
				return;
			throw vm::Exception(std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...)));
		}
		void register_function(const std::string name, StockFunction sf)
		{
			m_stockfunctions[name] = sf;
		}
		std::shared_ptr<vm::Variant> pop_pointer()
		{
			if (m_stack.empty())
			{
				__debugbreak();
				throw vm::Exception("stack empty");
			}
			auto v = m_stack[m_stack.size() - 1];
			if (v)
				printf("popping variant(%s) from stack\n", vm::kVariantNames[v->index()]);
			else
				printf("popping variant(nullptr) from stack\n");
			m_stack.pop_back();
			return v;
		}
		vm::Variant pop()
		{
			auto pp = pop_pointer();
			if (!pp)
				return vm::Undefined();
			return *pp.get();
		}
		FunctionContext* function()
		{
			if (m_callstack.empty())
				throw vm::Exception("no current function");
			return m_callstack.top();
		}
		void push_pointer(std::shared_ptr<vm::Variant> v)
		{
			if (v)
				printf("pushing variant(%s) to stack\n", vm::kVariantNames[v->index()]);
			else
				printf("pushing variant(nullptr) to stack\n");
			m_stack.push_back(v ? v : std::make_shared<vm::Variant>(vm::Undefined()));
		}
		void push(vm::Variant v)
		{
			printf("pushing variant(%s) to stack\n", vm::kVariantNames[v.index()]);
			m_stack.push_back(std::make_shared<vm::Variant>(v));
		}
		void dump();
		const vm::Variant& top(int offset = 0)
		{
			if (m_stack.empty())
				throw vm::Exception("stack empty");
			size_t offs = m_stack.size() - 1 - offset;
			if (offs >= m_stack.size())
				throw vm::Exception("out of bounds");
			return *m_stack[offs].get();
		}
		Interpreter(script::ReferenceMap&);
		vm::Variant call_builtin_function(const std::string name, FunctionArguments&);
		vm::Variant call_function(const std::string, FunctionArguments&);
		vm::Variant call_function(const std::string, const std::string, FunctionArguments&);

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
}; // namespace script