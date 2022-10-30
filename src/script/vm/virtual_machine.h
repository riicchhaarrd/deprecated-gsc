#pragma once
#include <common/exception.h>
#include "instruction.h"
#include <script/vm/instructions/instructions.h>
#include "types.h"
#include "function.h"
#include <functional>
#include <script/compiler/compiler.h>
#include <parse/token.h>

namespace script
{
	struct VMContext
	{
		virtual size_t number_of_arguments() = 0;
		virtual void set_number_of_arguments(size_t) = 0;
		virtual std::string get_string(size_t) = 0;
		virtual int get_int(size_t) = 0;
		virtual vm::ObjectPtr get_object(size_t) = 0;
		virtual float get_float(size_t) = 0;
		virtual void get_vector(size_t, vm::Vector&) = 0;
		virtual vm::Variant get_variant(size_t) = 0;
		virtual std::string variant_to_string(vm::Variant) = 0;
		virtual float variant_to_number(vm::Variant) = 0;
		virtual void add_bool(const bool b)
		{
			add_int(b ? 1 : 0);
		}
		virtual void add_variant(vm::Variant) = 0;
		virtual void add_vector(vm::Vector) = 0;
		virtual void add_object(std::shared_ptr<vm::Object>) = 0;
		virtual void add_int(const int) = 0;
		virtual void add_undefined() = 0;
		virtual void add_float(const float) = 0;
		virtual void add_string(const std::string) = 0;
	};

	using StockFunction = std::function<int(VMContext&)>;

	namespace vm
	{
		namespace flags
		{
			enum
			{
				kNone = 0,
				kZF = 1,
				kVerbose = 2
			};
		}; // namespace flags

		struct NotifyEvent
		{
			std::string event_string;
			vm::ObjectPtr object;
			std::vector<vm::Variant> arguments;
		};

		struct ThreadLock
		{
			virtual bool locked() = 0;
			virtual void notify(NotifyEvent&) = 0;
			virtual ~ThreadLock()
			{
			}
		};

		struct ExceptionData
		{
		};
		using Exception = common::TypedDataMessageException<ExceptionData>;
		struct FunctionContext
		{
			std::string file_name;
			std::string function_name;
			vm::ObjectPtr self_object;
			std::unordered_map<std::string, vm::Variant> variables;
			std::unordered_map<size_t, size_t> labels;
			Variant& get_variable(const std::string var)
			{
				auto fnd = variables.find(var);
				if (fnd == variables.end())
				{
					variables[var] = vm::Undefined();
				}
				return variables[var];
			}
			size_t instruction_index = 0;
			compiler::CompiledFunction* function = nullptr;
		};
		struct ThreadContext
		{
			std::vector<vm::Variant> m_stack;
			std::vector<vm::Variant*> m_referencestack;
			std::stack<FunctionContext> m_callstack;
			std::vector<std::unique_ptr<ThreadLock>> m_locks;
			std::unique_ptr<VMContext> m_context;
			std::unique_ptr<VMContext>& context()
			{
				return m_context;
			}
			FunctionContext& function_context()
			{
				if (m_callstack.empty())
					throw vm::Exception("callstack empty");
				return m_callstack.top();
			}
			bool marked_for_deletion = false;
			void ret();

			const std::string& current_file()
			{
				return function_context().file_name;
			}

			void jump(size_t i)
			{
				auto& fc = function_context();
				auto fnd = fc.labels.find(i);
				if (fnd == fc.labels.end())
				{
					for (auto& it : fc.labels)
					{
						printf("label: %d\n", it.first);
					}
					throw vm::Exception("cannot jump to non existing label {}", i);
				}
				fc.instruction_index = fnd->second;
			}
			void push(Variant v)
			{
				m_stack.push_back(v);
			}
			void push_reference(Variant* v)
			{
				m_referencestack.push_back(v);
			}
			Variant* pop_reference()
			{
				if (m_referencestack.empty())
					throw vm::Exception("empty refstack");
				auto* v = m_referencestack[m_referencestack.size() - 1];
				m_referencestack.pop_back();
				return v;
			}
			Variant& top(int offset = 0)
			{
				auto& stack = m_stack;
				if (stack.empty())
					throw vm::Exception("stack empty");
				size_t offs = stack.size() - 1 - offset;
				if (offs >= stack.size())
					throw vm::Exception("out of bounds");
				return stack[offs];
			}
			Variant pop(size_t n = 1)
			{
				Variant v;
				for (size_t i = 0; i < n; ++i)
				{
					if (m_stack.empty())
						throw vm::Exception("empty stack");
					v = m_stack[m_stack.size() - 1];
					m_stack.pop_back();
				}
				return v;
			}
			Variant* pop_reference_value()
			{
				auto v = pop();
				if (v.index() != (int)vm::Type::kReference)
				{
					throw vm::Exception("not a reference");
				}
				return pop_reference();
			}
		};
		class VirtualMachine
		{
			int m_flags = flags::kNone;
			compiler::CompiledFiles& m_compiledfiles;
			size_t frame_number = 0;

			std::unordered_map<std::string, StockFunction> m_stockfunctions;

			std::vector<std::unique_ptr<ThreadContext>> m_threads;
			std::vector<std::unique_ptr<ThreadContext>> m_newthreads;
			std::vector<NotifyEvent> notification_events;

			vm::Variant level_object;
			vm::Variant game_object;

			//TODO: FIXME, why is this here?
			//well because when we include something, the preprocessor has a include guard and then it won't get included in this particular file when
			//we're looking for a function
			//hackish solution, just make a large global list of all the functions and then if we can't find the function
			//just try to find it here
			std::unordered_map<std::string, compiler::CompiledFunction*> m_allcustomfunctions;
			void call_impl(ThreadContext *, ThreadContext*, vm::ObjectPtr obj, script::compiler::CompiledFunction*, size_t);

		  public:
			std::shared_ptr<vm::Instruction> fetch(ThreadContext*);
			size_t thread_count()
			{
				return m_threads.size();
			}

			size_t get_frame_number()
			{
				return frame_number;
			}

			void set_flags(int flags)
			{
				m_flags = flags;
			}

			vm::ObjectPtr get_level_object()
			{
				return std::get<vm::ObjectPtr>(level_object);
			}

			int get_flags()
			{
				return m_flags;
			}

			vm::Variant get_variable(ThreadContext*, const std::string var);
			vm::Variant* get_variable_reference(ThreadContext*, const std::string var);
			std::string variant_to_string_for_dump(VariantPtr v);
			void dump_object(const std::string, VariantPtr ptr, int indent);
			void dump(ThreadContext*);
			void notify_event_string(vm::ObjectPtr object, const std::string str, std::vector<vm::Variant>* arguments = nullptr)
			{
				if (!object)
					object = get_level_object();
				NotifyEvent ev;
				ev.object = object;
				ev.event_string = str;
				if (arguments)
				{
					for (auto it = arguments->begin(); it != arguments->end(); ++it)
						ev.arguments.push_back(*it);
				}
				notification_events.push_back(ev);
			}
			compiler::CompiledFunction* find_function_in_file(const std::string file, const std::string function);
			
			template <typename T> VariantPtr variant(T t)
			{
				return std::make_shared<Variant>(t);
			}
			void register_function(const std::string name, StockFunction sf)
			{
				m_stockfunctions[name] = sf;
			}
			VirtualMachine(compiler::CompiledFiles&);
			void run();
			void call_function(ThreadContext*, vm::ObjectPtr obj, const std::string&, const std::string&, size_t,
							   bool);
			void call_builtin(ThreadContext*, const std::string, size_t);
			void call_builtin_method(ThreadContext*, vm::ObjectPtr obj, const std::string, size_t);
			void notify(ThreadContext*, vm::ObjectPtr obj, size_t);
			void waittill(ThreadContext*, vm::ObjectPtr obj, const std::string, std::vector<std::string>&);
			void endon(ThreadContext*, vm::ObjectPtr obj, size_t);

			std::string variant_to_string(vm::Variant v);
			float variant_to_number(vm::Variant v);
			int variant_to_integer(vm::Variant v);
			vm::Variant exec_thread(ThreadContext*, vm::ObjectPtr obj, const std::string file,
									const std::string function, size_t numargs,
							 bool);

			bool run_thread(ThreadContext*);

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
				case '&':
					return a & b;
				case '|':
					return a | b;
				case parse::TokenType_kLsht:
					return a << b;
				case parse::TokenType_kRsht:
					return a >> b;
				case parse::TokenType_kEq:
					return a == b ? 1 : 0;
				case parse::TokenType_kNeq:
					return a == b ? 0 : 1;
				case parse::TokenType_kGeq:
					return a >= b ? 1 : 0;
				case '>':
					return a > b ? 1 : 0;
				case '<':
					return a < b ? 1 : 0;
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
			vm::Variant handle_binary_op(const float& a, const float& b, int op)
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
				case '>':
					return a > b ? 1 : 0;
				case '<':
					return a < b ? 1 : 0;
				case '%':
					return fmod(a, b);
				}
				throw vm::Exception("invalid operator {}", op);
				return 0.f;
			}

			vm::Variant handle_binary_op(const vm::Vector& a, const vm::Vector& b, int op)
			{
				vm::Vector ret;
				ret.x = ret.y = ret.z = 0.f;
				switch (op)
				{
				case '-':
					ret.x = a.x - b.x;
					ret.y = a.y - b.y;
					ret.z = a.z - b.z;
					break;
				case '+':
					ret.x = a.x + b.x;
					ret.y = a.y + b.y;
					ret.z = a.z + b.z;
					break;
				case '*':
					ret.x = a.x * b.x;
					ret.y = a.y * b.y;
					ret.z = a.z * b.z;
					break;
				case '/':
					ret.x = a.x / b.x;
					ret.y = a.y / b.y;
					ret.z = a.z / b.z;
					break;
				default:
					throw vm::Exception("invalid operator {}", op);
					break;
				}
				return ret;
			}

			vm::Variant handle_binary_op(const vm::Vector& a, float b, int op)
			{
				vm::Vector ret;
				ret.x = ret.y = ret.z = 0.f;
				switch (op)
				{
				case '-':
					ret.x = a.x - b;
					ret.y = a.y - b;
					ret.z = a.z - b;
					break;
				case '+':
					ret.x = a.x + b;
					ret.y = a.y + b;
					ret.z = a.z + b;
					break;
				case '*':
					ret.x = a.x * b;
					ret.y = a.y * b;
					ret.z = a.z * b;
					break;
				case '/':
					ret.x = a.x / b;
					ret.y = a.y / b;
					ret.z = a.z / b;
					break;
				default:
					throw vm::Exception("invalid operator {}", op);
					break;
				}
				return ret;
			}

			vm::Variant handle_binary_op(const std::string& a, const std::string& b, int op)
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
					if (a_index == vm::Type::kObject || b_index == vm::Type::kObject)
					{
						if (op != parse::TokenType_kEq && op != parse::TokenType_kNeq)
						{
							throw vm::Exception("expected == or !=");
						}
						if (a_index == vm::Type::kUndefined|| b_index == vm::Type::kUndefined)
						{
							if (op == parse::TokenType_kEq)
							{
								result = 0;
							}
							else
							{
								result = 1;
							}
						}
						else
						{
							auto obj_a = std::get<vm::ObjectPtr>(a);
							auto obj_b = std::get<vm::ObjectPtr>(b);
							if (op == parse::TokenType_kEq)
							{
								result = (obj_a.get() == obj_b.get()) ? 1 : 0;
							}
							else
							{
								result = (obj_a.get() != obj_b.get()) ? 1 : 0;
							}
						}
					}
					else if (a_index == vm::Type::kVector && b_index == vm::Type::kVector)
					{
						result = handle_binary_op(std::get<vm::Vector>(a), std::get<vm::Vector>(b), op);
					}
					else if (a_index == vm::Type::kVector && b_index == vm::Type::kFloat)
					{
						result = handle_binary_op(std::get<vm::Vector>(a), std::get<vm::Number>(b), op);
					}
					else if (a_index == vm::Type::kFloat && b_index == vm::Type::kFloat)
					{
						result = handle_binary_op(variant_to_number(a), variant_to_number(b), op);
					}
					else if (a_index == vm::Type::kFloat && b_index == vm::Type::kInteger)
					{
						result = handle_binary_op(variant_to_number(a), variant_to_number(b), op);
					}
					else if (a_index == vm::Type::kInteger && b_index == vm::Type::kFloat)
					{
						result = handle_binary_op(variant_to_number(a), variant_to_number(b), op);
					}
					else
					{
						result = handle_binary_op(variant_to_integer(a), variant_to_integer(b), op);
					}
				}
				return result;
			}
		};
	};
};
