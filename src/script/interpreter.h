#pragma once
#include <script/ast/ast_generator.h>
#include <memory>
#include <script/ast/recall_visitor.h>
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
#include <script/compiler/visitors/parent.h>

namespace script
{
	struct LoadedProgramReference
	{
		std::unique_ptr<ast::Program> program;
		std::string name;
		std::unordered_map<std::string, ast::FunctionDeclaration*> function_map;
		script::compiler::ParentVisitor m_parent_visitor;
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

	class Interpreter : public ast::ASTRecallVisitor
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
		std::stack<vm::ObjectPtr> m_objectstack;
		std::stack<LoadedProgramReference*> m_programstack;
		std::unordered_map<std::string, StockFunction> m_stockfunctions;
		std::unique_ptr<VMContext> m_context;
		script::ReferenceMap& m_refmap;
		LoadedProgramReference* m_program = nullptr;
	  public:
		void dump2(ast::Node& n)
		{
			auto* f = m_program->m_parent_visitor.get_parent_by_type<ast::FunctionDeclaration>(&n);
			auto* p = m_program->m_parent_visitor.get_parent_by_type<ast::BlockStatement>(&n);
			printf("file: %s, function: %s\n", m_program->name.c_str(), f->function_name.c_str());
			if (p)
			{
				GSCWriter wr(std::cout);
				wr.visit_node(*p);
			}
		}

		vm::ObjectPtr get_function_object()
		{
			if (m_objectstack.empty())
				return level_object;
			return m_objectstack.top();
		}

		template <typename... Ts> void error(ast::Node& n, std::string_view fmt, Ts&&... ts)
		{
			GSCWriter wr(std::cout);
			wr.visit_node(n);
			throw vm::Exception(std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...)));
		}
		void dump_node(ast::Node &n)
		{
			return;
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
			#if 0
			if (v)
				printf("popping variant(%s) from stack\n", vm::kVariantNames[v->index()]);
			else
				printf("popping variant(nullptr) from stack\n");
			#endif
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
			#if 0
			if (v)
				printf("pushing variant(%s) to stack\n", vm::kVariantNames[v->index()]);
			else
				printf("pushing variant(nullptr) to stack\n");
			#endif
			m_stack.push_back(v ? v : std::make_shared<vm::Variant>(vm::Undefined()));
		}
		void push(vm::Variant v)
		{
			//printf("pushing variant(%s) to stack\n", vm::kVariantNames[v.index()]);
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
		void visit_impl(ast::Program&);
		void visit_impl(ast::FunctionDeclaration&);
		void visit_impl(ast::SwitchCase&);
		void visit_impl(ast::Directive&);
		void visit_impl(ast::BlockStatement&);
		void visit_impl(ast::IfStatement&);
		void visit_impl(ast::WhileStatement&);
		void visit_impl(ast::ForStatement&);
		void visit_impl(ast::DoWhileStatement&);
		void visit_impl(ast::ReturnStatement&);
		void visit_impl(ast::BreakStatement&);
		void visit_impl(ast::WaitStatement&);
		void visit_impl(ast::WaitTillFrameEndStatement&);
		void visit_impl(ast::ExpressionStatement&);
		void visit_impl(ast::EmptyStatement&);
		void visit_impl(ast::ContinueStatement&);
		void visit_impl(ast::SwitchStatement&);
		void visit_impl(ast::LocalizedString&);
		void visit_impl(ast::Literal&);
		void visit_impl(ast::Identifier&);
		void visit_impl(ast::FunctionPointer&);
		void visit_impl(ast::BinaryExpression&);
		void visit_impl(ast::AssignmentExpression&);
		void visit_impl(ast::CallExpression&);
		void visit_impl(ast::ConditionalExpression&);
		void visit_impl(ast::MemberExpression&);
		void visit_impl(ast::UnaryExpression&);
		void visit_impl(ast::VectorExpression&);
		void visit_impl(ast::ArrayExpression&);
	};
}; // namespace script