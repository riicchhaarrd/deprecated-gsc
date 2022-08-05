#include "interpreter.h"
#include <script/ast/nodes.h>
#include <common/stringutil.h>
#include <parse/token.h>
#include <script/compiler/visitors/function.h>

namespace script
{

	namespace vm
	{
		namespace control
		{
			struct Return
			{
				Variant value;
			};
			struct Break
			{
			};
			struct Continue
			{
			};
		}; // namespace control
	};

	std::string variant_to_string(const vm::Variant v)
	{
		vm::Type index = (vm::Type)v.index();
		switch (index)
		{
		case vm::Type::kInteger:
			return std::to_string(std::get<vm::Integer>(v));
		case vm::Type::kFloat:
			return std::to_string(std::get<vm::Number>(v));
		case vm::Type::kString:
			return std::get<vm::String>(v);
		}
		throw vm::Exception("cannot convert {} to string", vm::kVariantNames[v.index()]);
		return "";
	}

	struct InterpreterVMContext : VMContext
	{
		class Interpreter& m_interpreter;
		InterpreterVMContext(Interpreter& interpreter) : m_interpreter(interpreter)
		{
		}

		virtual void add_int(const int i)
		{
			m_interpreter.push(i);
		}
		virtual void add_float(const float f)
		{
			m_interpreter.push(f);
		}
		virtual void add_string(const std::string s)
		{
			m_interpreter.push(s);
		}
		virtual vm::Variant get_variant(size_t index)
		{
			return m_interpreter.top(index);
		}
		virtual std::string get_string(size_t index)
		{
			auto v = m_interpreter.top(index);
			return variant_to_string(v);
		}
		virtual int get_int(size_t index)
		{
			auto v = m_interpreter.top(index);
			try
			{
				return std::get<vm::Integer>(v);
			}
			catch (std::bad_variant_access& e)
			{
				throw vm::Exception("cannot convert index {} from {} to integer", index, vm::kVariantNames[v.index()]);
			}
			return 0;
		}
		virtual float get_float(size_t index)
		{
			auto v = m_interpreter.top(index);
			if (v.index() == vm::type_index<vm::Number>())
				return std::get<vm::Number>(v);
			else if (v.index() == vm::type_index<vm::Integer>())
				return (float)std::get<vm::Integer>(v);
			throw vm::Exception("cannot convert index {} from {} to float", index, vm::kVariantNames[v.index()]);
			return 0.f;
		}
	};
	
	float variant_to_number(const vm::Variant& v)
	{
		vm::Type index = (vm::Type)v.index();
		if (index == vm::Type::kFloat)
			return std::get<vm::Number>(v);
		else if (index == vm::Type::kInteger)
			return (float)std::get<vm::Integer>(v);
		throw vm::Exception("cannot convert {} to float", vm::kVariantNames[v.index()]);
		return 0.f;
	}

	template <typename T> vm::Variant handle_binary_op(const T& a, const T& b, int op)
	{
		switch (op)
		{
		case '-':
			return a - b;
		case '+':
			return a + b;
		case '*':
			return a * b;
		case '/':
			return a / b;
		case '%':
			return a % b;
		case parse::TokenType_kEq:
			return a == b ? 1 : 0;
		case parse::TokenType_kNeq:
			return a == b ? 0 : 1;
		case parse::TokenType_kGeq:
			return a >= b ? 1 : 0;
		case parse::TokenType_kLeq:
			return a <= b ? 1 : 0;
		case parse::TokenType_kAndAnd:
			return a && b ? 1 : 0;
		case parse::TokenType_kOrOr:
			return a || b ? 1 : 0;
		}
		throw vm::Exception("invalid operator {}", op);
		return T();
	}
	template <> vm::Variant handle_binary_op(const float& a, const float& b, int op)
	{
		switch (op)
		{
		case '-':
			return a - b;
		case '+':
			return a + b;
		case '*':
			return a * b;
		case '/':
			return a / b;
		case parse::TokenType_kEq:
			return a == b ? 1 : 0;
		case parse::TokenType_kNeq:
			return a == b ? 0 : 1;
		case parse::TokenType_kGeq:
			return a >= b ? 1 : 0;
		case parse::TokenType_kLeq:
			return a <= b ? 1 : 0;
		case '%':
			return fmod(a, b);
		}
		throw vm::Exception("invalid operator {}", op);
		return 0.f;
	}

	template <> vm::Variant handle_binary_op(const std::string& a, const std::string& b, int op)
	{
		switch (op)
		{
		case '+':
			return a + b;
		case parse::TokenType_kEq:
			return a == b ? 1 : 0;
		case parse::TokenType_kNeq:
			return a == b ? 0 : 1;
		}
		throw vm::Exception("invalid operator {}", op);
		return "";
	}

	vm::Variant binop(const vm::Variant& a, const vm::Variant& b, int op)
	{
		vm::Type a_index = (vm::Type)a.index();
		vm::Type b_index = (vm::Type)b.index();
		vm::Variant result;
		if (a_index == vm::Type::kString || b_index == vm::Type::kString)
		{
			if (a_index == vm::Type::kString)
				result = handle_binary_op(std::get<vm::String>(a), variant_to_string(b), op);
			else
				result = handle_binary_op(variant_to_string(a), std::get<vm::String>(b), op);
		}
		else
		{
			if (a_index == vm::Type::kInteger &&
				b_index == vm::Type::kInteger) // if they're both integers then give a integer result back
			{
				result = handle_binary_op(std::get<vm::Integer>(a), std::get<vm::Integer>(b), op);
			}
			else
				result = handle_binary_op(variant_to_number(a), variant_to_number(b), op);
		}
		return result;
	}

	Interpreter::Interpreter(script::ReferenceMap& refmap) : m_refmap(refmap)
	{
		m_context = std::make_unique<InterpreterVMContext>(*this);
		level_object = std::make_shared<vm::Object>();
		game_object = std::make_shared<vm::Object>();
	}
	vm::Variant Interpreter::call_builtin_function(const std::string name, FunctionArguments& args)
	{
		auto fnd = m_stockfunctions.find(util::string::to_lower(name));
		if (fnd == m_stockfunctions.end())
			throw vm::Exception("no function named {}", name);
		for (auto it = args.begin(); it != args.end(); ++it)
			push_pointer(*it);
		int num_pushed = fnd->second(*m_context.get(), nullptr);
		vm::Variant rt = vm::Undefined();
		// TODO: put checks in place to check whether stack has shifted
		if (num_pushed != 0)
		{
			rt = pop();
		}
		for (auto it = args.begin(); it != args.end(); ++it)
			pop();
		return rt;
	}
	vm::Variant Interpreter::call_function(const std::string filepath_, const std::string name, FunctionArguments& args)
	{
		std::string ref = filepath_;
		std::replace(ref.begin(), ref.end(), '\\', '/');
		auto fnd = m_refmap.find(ref);
		assert_cond(fnd != m_refmap.end(), "file {} is not loaded", ref);
		m_programstack.push(m_program);
		m_program = &fnd->second;
		auto rv = call_function(name, args);
		m_program = m_programstack.top();
		m_programstack.pop();
		return rv;
	}
	vm::Variant Interpreter::call_function(const std::string name, FunctionArguments& args)
	{
		FunctionContext fc;
		size_t i = 0;
		size_t ss = m_stack.size();
		vm::Variant return_value = vm::Undefined();
		auto fnd = m_program->function_map.find(util::string::to_lower(name));
		if (fnd == m_program->function_map.end())
		{
			return_value = call_builtin_function(name, args);
			goto end;
		}
		m_callstack.push(&fc);
		for (auto& parm : fnd->second->parameters)
			fc.variables[parm] = i >= args.size() ? std::make_shared<vm::Variant>(vm::Undefined()) : args[i];

		try
		{
			fnd->second->body->accept(*this);
		}
		catch (vm::control::Return& ret)
		{
			return_value = ret.value;
		}
		if (m_callstack.empty())
			throw vm::Exception("callstack empty");
		m_callstack.pop();
	end:
		assert_cond(m_stack.size() == ss, "corrupt stack");
		printf("return value for %s = %s\n", name.c_str(), vm::kVariantNames[return_value.index()]);
		return return_value;
	}
	void Interpreter::visit(ast::Program& n)
	{
		throw vm::Exception("invalid {}", __LINE__);
	}

	void Interpreter::visit(ast::FunctionDeclaration& n)
	{
		throw vm::Exception("invalid {}", __LINE__);
	}

	void Interpreter::visit(ast::SwitchCase& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::Directive& n)
	{
		dump_node(n);
		// throw vm::Exception("unimplemented {}", __LINE__);
		printf("directive unimplemented, skipping\n");
	}

	void Interpreter::visit(ast::BlockStatement& n)
	{
		dump_node(n);
		printf("begin BlockStatement\n");
		for (auto& it : n.body)
			it->accept(*this);
		printf("end BlockStatement\n");
	}

	void Interpreter::dump()
	{
		printf("%d variables\n", function()->variables.size());
		for (auto it : function()->variables)
		{
			if (!it.second)
				continue;
			printf("%s %s\n", it.first, variant_to_string(*it.second.get()).c_str());
		}
		printf("level %d fields\n", level_object->fields.size());
		for (auto it : level_object->fields)
		{
			if (!it.second)
				continue;
			printf("field: %s %s\n", it.first.c_str(), variant_to_string(*it.second.get()).c_str());
		}
	}

	void Interpreter::visit(ast::IfStatement& n)
	{
		dump_node(n);
		printf("begin IfStatement\n");
		n.test->accept(*this);
		int cond = m_context->get_int(0);
		pop();
		printf("cond=%d\n", cond);
		printf("end IfStatement\n");
	}

	void Interpreter::visit(ast::WhileStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::ForStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::DoWhileStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::ReturnStatement& n)
	{
		dump_node(n);
		if (n.argument)
		{
			n.argument->accept(*this);
			throw vm::control::Return{.value = pop()};
		}
		throw vm::control::Return{.value = vm::Undefined()};
	}

	void Interpreter::visit(ast::BreakStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::WaitStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::WaitTillFrameEndStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::ExpressionStatement& n)
	{
		dump_node(n);
		n.expression->accept(*this);
		pop();
	}

	void Interpreter::visit(ast::EmptyStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::ContinueStatement& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::SwitchStatement& n)
	{
		dump_node(n);
		n.discriminant->accept(*this);
		auto d = variant_to_string(pop());
		for (auto& c : n.cases)
		{
			c->test->accept(*this);
			auto str = variant_to_string(pop());
			if (str == d)
			{
				for (auto& stmt : c->consequent)
				{
					stmt->accept(*this);
				}
				break;
			}
		}
	}

	void Interpreter::visit(ast::LocalizedString& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::Literal& n)
	{
		dump_node(n);
		printf("begin Literal\n");
		switch (n.type)
		{
		case ast::Literal::Type::kInteger:
			push(atoi(n.value.c_str()));
			break;
		case ast::Literal::Type::kNumber:
			push((float)atof(n.value.c_str()));
			break;
		case ast::Literal::Type::kString:
			push(n.value);
			break;
		case ast::Literal::Type::kAnimation:
		{
			vm::Animation a;
			a.reference = n.value;
			push(a);
		} break;
		case ast::Literal::Type::kUndefined:
		{
			push(vm::Undefined());
		} break;
		default:
			throw vm::Exception("unimplemented {}", __LINE__);
			break;
		}
		printf("end Literal\n");
	}

	std::shared_ptr<vm::Object> create_object()
	{
		return std::make_shared<vm::Object>();
	}

	std::shared_ptr<vm::Variant> create_variant_object()
	{
		return std::make_shared<vm::Variant>(create_object());
	}
	std::shared_ptr<vm::Variant> create_variant_undefined()
	{
		return std::make_shared<vm::Variant>(vm::Undefined());
	}

	void Interpreter::visit(ast::Identifier& n)
	{
		dump_node(n);
		printf("begin Identifier\n");
		if(!n.file_reference.empty())
			throw vm::Exception("can't use file references {}::{}", n.name, n.file_reference);
		printf("variable %s\n", n.name.c_str());
		if (n.name == "level")
			push(level_object);
		else if (n.name == "game")
			push(game_object);
		else if (n.name == "self")
			throw vm::Exception("self not implemented");
		else
		{
			auto& vars = function()->variables;
			if (vars.find(n.name) == vars.end())
				vars[n.name] = create_variant_undefined();
			push_pointer(vars[n.name]);
		}

		printf("end Identifier\n");
	}

	void Interpreter::visit(ast::FunctionPointer& n)
	{
		dump_node(n);
		push(vm::FunctionPointer{.name = n.function_name});
	}

	void Interpreter::visit(ast::BinaryExpression& n)
	{
		dump_node(n);
		printf("begin BinaryExpression\n");
		n.left->accept(*this);
		auto lhs = m_context->get_variant(0);
		pop();
		n.right->accept(*this);
		auto rhs = m_context->get_variant(0);
		pop();
		vm::Variant result = binop(lhs, rhs, n.op);
		push(result);
		printf("end BinaryExpression\n");
	}

	void Interpreter::visit(ast::AssignmentExpression& n)
	{
		dump_node(n);
		printf("begin AssignmentExpression\n");
		//load lhs
		n.lhs->accept(*this);
		auto ptr = pop_pointer();
		vm::Variant pv = vm::Undefined();
		if (ptr)
			pv = *ptr.get();
		n.rhs->accept(*this);
		auto rhs = pop(); //might want to pop_pointer, but cba to support multiple assignments
		if (ptr)
			*(ptr.get()) = rhs;
		printf("end AssignmentExpression\n");
		push_pointer(ptr);
	}

	void Interpreter::visit(ast::CallExpression& n)
	{
		dump_node(n);
		printf("begin CallExpression\n");
		FunctionArguments args;
		for (auto it = n.arguments.rbegin(); it != n.arguments.rend(); ++it)
		{
			(*it)->accept(*this);
			args.push_back(pop_pointer());
		}
		vm::Variant return_value;
		auto *id = dynamic_cast<ast::Identifier*>(n.callee.get());
		if (id)
		{
			if (!id->file_reference.empty())
			{
				return_value = call_function(id->file_reference, id->name, args);
			}
			else
			{
				return_value = call_function(id->name, args);
			}
		}
		else
			throw vm::Exception("unhandled function pointer call");
		push(return_value);
		printf("end CallExpression\n");
	}

	void Interpreter::visit(ast::ConditionalExpression& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	bool get_property(ast::Expression& n, std::string& prop)
	{
		auto* id = dynamic_cast<ast::Identifier*>(&n);
		if (id)
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

	void Interpreter::visit(ast::MemberExpression& n)
	{
		dump_node(n);
		printf("begin MemberExpression\n");
		n.object->accept(*this);
		auto obj = pop_pointer();
		if (obj->index() != vm::type_index<vm::ObjectPtr>())
		{
			if (obj->index() == 0)
				*obj = create_object();
			else
				throw vm::Exception("not an object");
		}
		auto ptr = std::get<vm::ObjectPtr>(*obj);
		std::string field_name;
		if (!get_property(*n.prop.get(), field_name))
		{
			n.prop->accept(*this);
			field_name = m_context->get_string(0);
			pop();
		}
		printf("field: %s\n", field_name.c_str());
		push_pointer(ptr->get_field(field_name));
		//throw vm::Exception("unimplemented {}", __LINE__);
		printf("end MemberExpression\n");
	}

	void Interpreter::visit(ast::UnaryExpression& n)
	{
		dump_node(n);
		printf("begin UnaryExpression\n");
		n.argument->accept(*this);
		auto arg = pop();
		vm::Type idx = (vm::Type)arg.index();
		switch (n.op)
		{
		case '-':
			assert_cond(n, idx == vm::Type::kInteger || idx == vm::Type::kFloat, "not a number");
			if (idx == vm::Type::kFloat)
				push(-std::get<vm::Number>(arg));
			else if (idx == vm::Type::kFloat)
				push(-std::get<vm::Integer>(arg));
			break;
		case '!':
		{
			printf("! %s\n", vm::kVariantNames[arg.index()]);
			assert_cond(n, idx == vm::Type::kInteger, "not a integer");
			push(!std::get<vm::Integer>(arg));
		} break;
		case '~':
			printf("~ %d\n", arg.index());
			assert_cond(n, idx == vm::Type::kInteger, "not a integer");
			push(~std::get<vm::Integer>(arg));
			break;
		default:
			throw vm::Exception("unhandled operator {}", n.op);
			break;
		}
		printf("end UnaryExpression\n");
	}

	void Interpreter::visit(ast::VectorExpression& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}

	void Interpreter::visit(ast::ArrayExpression& n)
	{
		dump_node(n);
		throw vm::Exception("unimplemented {}", __LINE__);
	}
}; // namespace script