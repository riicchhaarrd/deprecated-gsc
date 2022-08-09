#include "virtual_machine.h"

namespace script
{
	namespace vm
	{

		std::string VirtualMachine::variant_to_string_for_dump(VariantPtr v)
		{
			vm::Type index = (vm::Type)v->index();
			switch (index)
			{
			case vm::Type::kInteger:
				return std::to_string(std::get<vm::Integer>(*v));
			case vm::Type::kFloat:
				return std::to_string(std::get<vm::Number>(*v));
			case vm::Type::kString:
				return std::get<vm::String>(*v);
			case vm::Type::kUndefined:
				return "undefined";
			case vm::Type::kObject:
				return "object";
			case vm::Type::kVector:
			{
				auto vec = std::get<vm::Vector>(*v);
				return std::format("({}, {}, {})", vec.x, vec.y, vec.z);
			}
			break;
			}
			return std::format("unhandled variant {}", v->index());
		}

		std::string VirtualMachine::variant_to_string(vm::Variant v)
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
			case vm::Type::kUndefined:
				return "undefined";
			case vm::Type::kVector:
			{
				auto vec = std::get<vm::Vector>(v);
				return std::format("({}, {}, {})", vec.x, vec.y, vec.z);
			}
			break;
			}
			throw vm::Exception("cannot convert {} to string", vm::kVariantNames[v.index()]);
			return "";
		}

		float VirtualMachine::variant_to_number(vm::Variant v)
		{
			vm::Type index = (vm::Type)v.index();
			if (index == vm::Type::kFloat)
				return std::get<vm::Number>(v);
			else if (index == vm::Type::kInteger)
				return (float)std::get<vm::Integer>(v);
			throw vm::Exception("cannot convert {} to float", vm::kVariantNames[v.index()]);
			return 0.f;
		}

		struct VMContextImpl : VMContext
		{
			class VirtualMachine& vm;
			VMContextImpl(VirtualMachine& vm_) : vm(vm_)
			{
			}

			virtual void add_int(const int i)
			{
				vm.push(vm.variant(i));
			}
			virtual void add_float(const float f)
			{
				vm.push(vm.variant(f));
			}
			virtual void add_string(const std::string s)
			{
				vm.push(vm.variant(s));
			}
			virtual VariantPtr get_variant(size_t index)
			{
				return vm.top(index);
			}
			virtual vm::ObjectPtr get_object(size_t index)
			{
				auto v = vm.top(index);
				if (!v)
					throw vm::Exception("expected object got {}", v->index());
				if (v->index() == (int)vm::Type::kUndefined)
					*v = std::make_shared<Object>();
				if (v->index() != (int)vm::Type::kObject)
					throw vm::Exception("expected object got {}", v->index());
				return std::get<vm::ObjectPtr>(*v);
			}
			virtual std::string get_string(size_t index)
			{
				auto v = vm.top(index);
				return vm.variant_to_string(*v);
			}
			virtual int get_int(size_t index)
			{
				auto v = vm.top(index);
				try
				{
					return std::get<vm::Integer>(*v);
				}
				catch (std::bad_variant_access& e)
				{
					throw vm::Exception("cannot convert index {} from {} to integer", index,
										vm::kVariantNames[v->index()]);
				}
				return 0;
			}
			virtual float get_float(size_t index)
			{
				auto v = vm.top(index);
				if (v->index() == vm::type_index<vm::Number>())
					return std::get<vm::Number>(*v);
				else if (v->index() == vm::type_index<vm::Integer>())
					return (float)std::get<vm::Integer>(*v);
				throw vm::Exception("cannot convert index {} from {} to float", index, vm::kVariantNames[v->index()]);
				return 0.f;
			}
		};

		VirtualMachine::VirtualMachine(compiler::CompiledFiles& cf_) : m_compiledfiles(cf_)
		{
			m_context = std::make_unique<VMContextImpl>(*this);
			level_object = std::make_shared<vm::Variant>(std::make_shared<vm::Object>());
			game_object = std::make_shared<vm::Variant>(std::make_shared<vm::Object>());
		}

		void VirtualMachine::exec_thread(const std::string file, const std::string function)
		{
			m_threads.push_back(ThreadContext());
			m_thread = &m_threads[m_threads.size() - 1];

			call(file, function, 0);
		}

		compiler::CompiledFunction* VirtualMachine::find_function_in_file(const std::string file,
																		  const std::string function)
		{
			auto fnd = m_compiledfiles.find(file);
			if (fnd == m_compiledfiles.end())
				return nullptr;
			auto fnd2 = fnd->second.find(function);
			if (fnd2 == fnd->second.end())
				return nullptr;
			return &fnd2->second;
		}

		void VirtualMachine::dump_object(const std::string name, VariantPtr ptr, int indent)
		{
			auto obj = std::get<vm::ObjectPtr>(*ptr);
			for (auto& it : obj->fields)
			{
				for (int i = 0; i < indent; ++i)
					putchar('\t');
				printf("%s.%s = %s;\n", name.c_str(), it.first.c_str(), variant_to_string_for_dump(it.second).c_str());
				if (it.second->index() == (int)vm::Type::kObject)
					dump_object(it.first, it.second, indent + 1);
			}
		}

		void VirtualMachine::dump()
		{
			auto& fc = function_context();
			dump_object("level", level_object, 0);
			dump_object("game", game_object, 0);
			for (auto& it : fc.variables)
			{
				printf("%s = %s;\n", it.first.c_str(), variant_to_string_for_dump(it.second).c_str());
			}
		}

		void VirtualMachine::ret()
		{
			if (m_thread->m_callstack.empty())
				throw vm::Exception("empty callstack"); //remove current thread from active threads?
			m_thread->m_callstack.pop();
		}

		void VirtualMachine::call(const std::string file, const std::string function, size_t numargs)
		{
			auto* fn = find_function_in_file(file, function);
			if (!fn)
				throw vm::Exception("can't find {}::{}", file, function);

			m_thread->m_callstack.push(FunctionContext());
			auto& fc = function_context();

			for (size_t i = 0; i < numargs; ++i)
			{
				auto arg = pop();
				if (i >= fn->parameters.size())
					continue;
				auto &parm = fn->parameters[i];
				fc.variables[parm] = arg;
				printf("setting parameter %s to %s\n", parm.c_str(), variant_to_string_for_dump(arg).c_str());
			}

			fc.instruction_index = 0;
			fc.file_name = file;
			fc.function_name = function;
			fc.function = fn;

			printf("============================================\n");
			for (auto& instr : fc.function->instructions)
			{
				printf("$%s\n", instr->to_string().c_str());
			}
			printf("============================================\n");

			dump();
		}

		void VirtualMachine::call_builtin(const std::string function, size_t numargs)
		{
			//dump();
			auto fnd = m_stockfunctions.find(util::string::to_lower(function));
			if (fnd == m_stockfunctions.end())
				throw vm::Exception("no function named {}", function);
			int num_pushed = fnd->second(*m_context.get(), nullptr);
			if (num_pushed == 0)
			{
				push(variant(vm::Undefined()));
			}
		}
		
		void VirtualMachine::call(const std::string function, size_t numargs)
		{
			auto fnd = m_stockfunctions.find(util::string::to_lower(function));
			if (fnd != m_stockfunctions.end())
			{
				call_builtin(function, numargs);
				return;
			}
			auto& fc = function_context();
			call(fc.file_name, function, numargs);
		}

		std::shared_ptr<vm::Instruction> VirtualMachine::fetch(ThreadContext* tc)
		{
			auto& fc = tc->function_context();
			if (fc.instruction_index >= fc.function->instructions.size())
				return nullptr;
			return fc.function->instructions[fc.instruction_index++];
		}

		VariantPtr VirtualMachine::get_variable(const std::string var)
		{
			auto& fc = function_context();
			if (var == "level")
				return level_object;
			else if (var == "game")
			{
				return game_object;
			}
			else if (var == "self")
				throw vm::Exception("self unhandled");
			return fc.get_variable(var);
		}
		void VirtualMachine::run()
		{
			while (1)
			{
				for (auto& thread : m_threads)
				{
					while (1)
					{
						auto instr = fetch(&thread);
						if (!instr)
							throw vm::Exception("shouldn't be nullptr");
						printf("\t\t-->%s\n", instr->to_string().c_str());
						instr->execute(*this);
					}
				}
			}
		}
	}; // namespace vm
};	   // namespace script