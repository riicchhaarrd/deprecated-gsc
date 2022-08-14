#include "virtual_machine.h"
#include <Windows.h>

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
			case vm::Type::kLocalizedString:
				return std::get<vm::LocalizedString>(v).reference;
			case vm::Type::kUndefined:
				return "undefined";
			case vm::Type::kObject:
				return "object";
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
			else if (index == vm::Type::kUndefined)
				return 0.f;
			else if (index == vm::Type::kString)
				return atof(std::get<vm::String>(v).c_str());
			throw vm::Exception("cannot convert {} to float", vm::kVariantNames[v.index()]);
			return 0.f;
		}

		int VirtualMachine::variant_to_integer(vm::Variant v)
		{
			vm::Type index = (vm::Type)v.index();
			if (index == vm::Type::kUndefined)
				return -1; //TODO: FIXME proper bool types, otherwise undefined (0) would be true to false
			else if (index == vm::Type::kInteger)
				return std::get<vm::Integer>(v);
			throw vm::Exception("cannot convert {} to integer", vm::kVariantNames[v.index()]);
			return 0;
		}

		struct VMContextImpl : VMContext
		{
			class VirtualMachine& vm;
			size_t nargs = 0;

			VMContextImpl(VirtualMachine& vm_) : vm(vm_)
			{
			}
			virtual size_t number_of_arguments()
			{
				return nargs;
			}
			
			virtual void set_number_of_arguments(size_t n)
			{
				nargs = n;
			}

			virtual void add_object(std::shared_ptr<vm::Object>& o)
			{
				vm.push(vm.variant(std::move(o)));
			}
			virtual void add_vector(vm::Vector v)
			{
				vm.push(vm.variant(v));
			}
			virtual void add_int(const int i)
			{
				vm.push(vm.variant(i));
			}
			virtual void add_undefined()
			{
				vm.push(vm.variant(vm::Undefined()));
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
					throw vm::Exception("expected object got null");
				//if (v->index() == (int)vm::Type::kUndefined)
					//*v = std::make_shared<Object>();
				if (v->index() != (int)vm::Type::kObject)
					throw vm::Exception("expected object got {}", v->index());
				return std::get<vm::ObjectPtr>(*v);
			}
			virtual void get_vector(size_t index, vm::Vector& vec)
			{
				auto v = vm.top(index);
				if (v->index() != (int)vm::Type::kVector)
					throw vm::Exception("expected vector got {}", v->index());
				vec = std::get<vm::Vector>(*v);
			}
			virtual std::string get_string(size_t index)
			{
				auto v = vm.top(index);
				return vm.variant_to_string(*v);
			}
			virtual std::string variant_to_string(vm::Variant v)
			{
				return vm.variant_to_string(v);
			}
			virtual float variant_to_number(vm::Variant v)
			{
				return vm.variant_to_number(v);
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
			level_object = std::make_shared<vm::Object>("level");
			game_object = std::make_shared<vm::Object>("game");
			for (auto& it : cf_)
			{
				for (auto& iter : it.second)
				{
					m_allcustomfunctions[iter.first] = &iter.second;
				}
			}
		}

		void VirtualMachine::exec_thread(vm::ObjectPtr obj, const std::string file, const std::string function,
										 size_t numargs, bool is_method)
		{
			if (!obj)
				throw vm::Exception("no object");
			auto* fn = find_function_in_file(file, function);
			if (!fn)
				throw vm::Exception("can't find {}::{}", file, function);
			m_newthreads.push_back(std::make_unique<ThreadContext>());
			auto* thr = m_newthreads[m_newthreads.size() - 1].get();
			//TODO: FIXME there's no guarantee in which order the thread runs, atm it runs after the thread that made a new thread
			//but we could run the thread first till we hit a wait then return control to the former thread
			call_impl(thr, obj, fn, numargs);
		}

		compiler::CompiledFunction* VirtualMachine::find_function_in_file(const std::string file,
																		  const std::string function)
		{
			auto fnd = m_compiledfiles.find(util::string::to_lower(file));
			if (fnd == m_compiledfiles.end())
				return nullptr;
			auto fnd2 = fnd->second.find(util::string::to_lower(function));
			if (fnd2 == fnd->second.end())
			{
				auto fnd3 = m_allcustomfunctions.find(util::string::to_lower(function));
				if (fnd3 == m_allcustomfunctions.end())
					return nullptr;
				return fnd3->second;
			}
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

		void VirtualMachine::dump(ThreadContext *tc)
		{
			#if 0
			auto& fc = tc->function_context();
			dump_object("level", level_object, 0);
			dump_object("game", game_object, 0);
			for (auto& it : fc.variables)
			{
				printf("%s = %s;\n", it.first.c_str(), variant_to_string_for_dump(it.second).c_str());
			}
			#endif
		}

		void VirtualMachine::ret()
		{
			if (m_thread->m_callstack.empty())
				throw vm::Exception("empty callstack");
			m_thread->m_callstack.pop();
			if (m_thread->m_callstack.empty())
			{
				m_thread->marked_for_deletion = true;
			}
		}

		void VirtualMachine::call_impl(ThreadContext* thread, vm::ObjectPtr obj, script::compiler::CompiledFunction* fn, size_t numargs)
		{
			thread->m_callstack.push(FunctionContext());
			auto& fc = thread->function_context();

			for (size_t i = 0; i < numargs; ++i)
			{
				auto arg = pop(); //we're still calling m_thread pop because we want to pop the arguments off that thread
				if (i >= fn->parameters.size())
					continue;
				auto &parm = fn->parameters[i];
				fc.variables[parm] = arg;
				//printf("setting parameter %s to %s\n", parm.c_str(), variant_to_string_for_dump(arg).c_str());
			}

			fc.instruction_index = 0;
			fc.file_name = fn->file;
			fc.function_name = fn->name;
			fc.function = fn;
			fc.self_object = obj;

			//TODO: FIXME
			//scan forward for labels
			for (size_t i = 0; i < fc.function->instructions.size(); ++i)
			{
				auto& instr = fc.function->instructions[i];
				auto *l = dynamic_cast<Label*>(instr.get());
				if (!l)
					continue;
				fc.labels[l->label_index] = i;
			}
			#if 0
			printf("============================================\n");
			for (auto& instr : fc.function->instructions)
			{
				printf("$%s\n", instr->to_string().c_str());
			}
			printf("============================================\n");
			#endif
		}

		void VirtualMachine::notify(vm::ObjectPtr obj, size_t numargs)
		{
			std::string evstr = context()->get_string(0);
			pop();

			std::vector<vm::Variant> args;
			for (size_t i = 0; i < numargs - 1; ++i)
			{
				auto vp = pop();
				args.push_back(*vp);
			}
			notify_event_string(obj, evstr, &args);
			push(variant(vm::Undefined()));
		}
		void VirtualMachine::waittill(vm::ObjectPtr obj, const std::string event_str, std::vector<std::string>& vars)
		{
			struct ThreadLockWaitForEventString : vm::ThreadLock
			{
				VirtualMachine* vm;
				std::vector<std::string> parameters;
				std::string string;
				vm::ObjectPtr object;
				bool notified = false;
				FunctionContext* fc; //should still exist right..

				virtual void notify(NotifyEvent& ne)
				{
					if (string == ne.event_string && ne.object == object)
					{
						for (size_t i = 0; i < parameters.size(); ++i)
						{
							//printf("setting parameter for notify i:%d, argsize:%d, parmsize:%d\n", i, ne.arguments.size(), parameters.size());
							if (i >= ne.arguments.size())
								continue;
							auto var = fc->get_variable(parameters[i]);
							*var = ne.arguments[i];
						}
						notified = true;
					}
					else
					{
						//printf("notify: %s, %s, %02X, %02X\n", ne.event_string.c_str(), string.c_str(), ne.object.get(), object.get());
					}
				}

				virtual bool locked()
				{
					return !notified;
				}
			};
			auto l = std::make_unique<ThreadLockWaitForEventString>();
			for (size_t i = 0; i < vars.size(); ++i)
			{
				l->parameters.push_back(vars[i]);
			}
			l->fc = &function_context();
			l->vm = this;
			l->object = obj;
			l->string = event_str;
			current_thread()->m_locks.push_back(std::move(l));
			push(variant(vm::Undefined()));
		}
		void VirtualMachine::endon(vm::ObjectPtr obj, size_t numargs)
		{
			pop(numargs);
			push(variant(vm::Undefined()));
		}

		void VirtualMachine::call_builtin_method(vm::ObjectPtr obj, const std::string function, size_t numargs)
		{
			ObjectMethod* m = nullptr;
			void* robj = nullptr;
			if (!obj->get_method(util::string::to_lower(function), &m, &robj) || !m)
			{
				throw vm::Exception("no method {} found for object {}", function, obj->m_tag);
				return;
			}
			int num_pushed = m->execute(*m_context.get(), robj);
			if (num_pushed == 0)
			{
				pop(numargs);
				push(variant(vm::Undefined()));
			}
			else
			{
				auto tmp = pop();
				pop(numargs);
				push(tmp);
			}
		}
		void VirtualMachine::call_builtin(const std::string function, size_t numargs)
		{
			auto fnd = m_stockfunctions.find(util::string::to_lower(function));
			if (fnd == m_stockfunctions.end())
				throw vm::Exception("no function named {}", function);
			m_context->set_number_of_arguments(numargs);
			int num_pushed = fnd->second(*m_context.get(), nullptr);

			if (num_pushed == 0)
			{
				pop(numargs);
				push(variant(vm::Undefined()));
			}
			else
			{
				auto tmp = pop();
				pop(numargs);
				push(tmp);
			}
		}
		
		void VirtualMachine::call_function(vm::ObjectPtr obj, const std::string& file, const std::string& function, size_t numargs, bool is_method)
		{
			auto& fc = function_context();
			auto* fn = find_function_in_file(fc.file_name, function);
			bool function_exists = function_exists = fn != nullptr;

			if (is_method)
			{
				if (function == "endon")
				{
					endon(obj, numargs);
					return;
				}
				else if (function == "notify")
				{
					notify(obj, numargs);
					return;
				}
				else if (function == "waittill")
				{
					throw vm::Exception("should be a instruction, not a function call");
					return;
				}
			}
			if (function_exists)
			{
				call_impl(current_thread(), obj, fn, numargs);
				return;
			}
			if (!is_method)
				call_builtin(function, numargs);
			else
				call_builtin_method(obj, function, numargs);
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
				return variant(level_object);
			else if (var == "game")
			{
				return variant(game_object);
			}
			else if (var == "self")
			{
				return variant(fc.self_object);
			}
			return fc.get_variable(var);
		}
		void VirtualMachine::run()
		{
			//while (1)
			{
				//remove deleted threads
				for (auto it = m_threads.begin(); it != m_threads.end();)
				{
					if ((*it)->marked_for_deletion)
						it = m_threads.erase(it);
					else
						++it;
				}

				for (auto it = m_newthreads.begin(); it != m_newthreads.end();)
				{
					m_threads.push_back(std::move(*it));
					it = m_newthreads.erase(it);
				}

				//may have to change the order depending on the behavior of endon/notify called right after eachother
				//e.g
				//level endon("test");
				//level notify("test")

				for (auto& ne : notification_events)
				{
					for(auto & thr : m_threads)
					{
						for (auto& l : thr->m_locks)
						{
							l->notify(ne);
						}
					}
				}
				notification_events.clear();

				if (m_threads.empty())
					return;
				//probably would be better if the instructions did accept(*this)
				//and then a InstructionRunner instantiated with a reference to the active thread with some other things
				for (auto& thread : m_threads)
				{
					m_thread = thread.get();
					while (1)
					{
						bool is_waiting = false;
						for (auto lock_iterator = thread->m_locks.begin(); lock_iterator != thread->m_locks.end();)
						{
							if ((*lock_iterator)->locked())
							{
								is_waiting = true;
								break;
							}
							lock_iterator = thread->m_locks.erase(lock_iterator);
						}
						if (is_waiting)
							break;
						if (thread->marked_for_deletion)
							break;
						auto instr = fetch(m_thread);
						if (!instr)
							throw vm::Exception("shouldn't be nullptr");
						auto& fc = m_thread->function_context();
						//printf("\t\t-->%s (%d)\t%s::%s\n", instr->to_string().c_str(), m_thread->m_stack.size(), fc.file_name.c_str(), fc.function_name.c_str());
						instr->execute(*this);
					}
				}
				//Sleep(1000 / 20);
			}
			++frame_number;
		}
	}; // namespace vm
};	   // namespace script