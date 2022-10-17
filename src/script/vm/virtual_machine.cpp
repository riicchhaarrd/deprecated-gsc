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
				return common::format("({}, {}, {})", vec.x, vec.y, vec.z);
			}
			break;
			}
			return common::format("unhandled variant {}", v->index());
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
			case vm::Type::kFunctionPointer:
			{
				auto fp = std::get<vm::FunctionPointer>(v);
				return common::format("{}::{}", fp.file, fp.name);
			} break;
			case vm::Type::kVector:
			{
				auto vec = std::get<vm::Vector>(v);
				return common::format("({}, {}, {})", vec.x, vec.y, vec.z);
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
			ThreadContext* thread; 
			size_t nargs = 0;

			VMContextImpl(VirtualMachine& vm_, ThreadContext* thread_) : vm(vm_), thread(thread_)
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

			virtual void add_object(std::shared_ptr<vm::Object> o)
			{
				thread->push(std::move(o));
			}
			virtual void add_vector(vm::Vector v)
			{
				thread->push(v);
			}
			virtual void add_int(const int i)
			{
				thread->push(i);
			}
			virtual void add_undefined()
			{
				thread->push(vm::Undefined());
			}
			virtual void add_float(const float f)
			{
				thread->push(f);
			}
			virtual void add_string(const std::string s)
			{
				thread->push(s);
			}
			virtual Variant get_variant(size_t index)
			{
				return thread->top(index);
			}
			virtual vm::ObjectPtr get_object(size_t index)
			{
				auto& v = thread->top(index);
				//if (v->index() == (int)vm::Type::kUndefined)
					//*v = std::make_shared<Object>();
				if (v.index() != (int)vm::Type::kObject)
					throw vm::Exception("expected object got {}", v.index());
				return std::get<vm::ObjectPtr>(v);
			}
			virtual void get_vector(size_t index, vm::Vector& vec)
			{
				auto& v = thread->top(index);
				if (v.index() != (int)vm::Type::kVector)
					throw vm::Exception("expected vector got {}", v.index());
				vec = std::get<vm::Vector>(v);
			}
			virtual std::string get_string(size_t index)
			{
				auto& v = thread->top(index);
				return vm.variant_to_string(v);
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
				auto& v = thread->top(index);
				try
				{
					return std::get<vm::Integer>(v);
				}
				catch (std::bad_variant_access& e)
				{
					throw vm::Exception("cannot convert index {} from {} to integer", index,
										vm::kVariantNames[v.index()]);
				}
				return 0;
			}
			virtual float get_float(size_t index)
			{
				auto& v = thread->top(index);
				if (v.index() == vm::type_index<vm::Number>())
					return std::get<vm::Number>(v);
				else if (v.index() == vm::type_index<vm::Integer>())
					return (float)std::get<vm::Integer>(v);
				throw vm::Exception("cannot convert index {} from {} to float", index, vm::kVariantNames[v.index()]);
				return 0.f;
			}
		};

		VirtualMachine::VirtualMachine(compiler::CompiledFiles& cf_) : m_compiledfiles(cf_)
		{
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

		vm::Variant VirtualMachine::exec_thread(ThreadContext* current_thread, vm::ObjectPtr obj, const std::string file,
												const std::string function,
										 size_t numargs, bool is_method)
		{
			if (!obj)
				throw vm::Exception("no object");
			auto* fn = find_function_in_file(file, function);
			if (!fn)
				throw vm::Exception("can't find {}::{}", file, function);
			m_newthreads.push_back(std::make_unique<ThreadContext>());
			auto* thr = m_newthreads[m_newthreads.size() - 1].get();
			thr->m_context = std::make_unique<VMContextImpl>(*this, thr);
			//TODO: FIXME there's no guarantee in which order the thread runs, atm it runs after the thread that made a new thread
			//but we could run the thread first till we hit a wait then return control to the former thread
			call_impl(current_thread, thr, obj, fn, numargs);
			if (run_thread(thr))
			{
				//the thread didn't yield/stall so we can just get the return value and the thread will end
				return thr->pop();
			}

			return vm::Undefined();
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
			#if 0
			auto obj = std::get<vm::ObjectPtr>(*ptr);
			for (auto& it : obj->fields)
			{
				for (int i = 0; i < indent; ++i)
					putchar('\t');
				printf("%s.%s = %s;\n", name.c_str(), it.first.c_str(), variant_to_string_for_dump(it.second).c_str());
				if (it.second->index() == (int)vm::Type::kObject)
					dump_object(it.first, it.second, indent + 1);
			}
			#endif
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

		void ThreadContext::ret()
		{
			if (m_callstack.empty())
				throw vm::Exception("empty callstack");
			m_callstack.pop();
			if (m_callstack.empty())
			{
				marked_for_deletion = true;
			}
		}

		void VirtualMachine::call_impl(ThreadContext *caller_thread, ThreadContext* callee_thread, vm::ObjectPtr obj, script::compiler::CompiledFunction* fn, size_t numargs)
		{
			callee_thread->m_callstack.push(FunctionContext());
			auto& fc = callee_thread->function_context();

			for (size_t i = 0; i < numargs; ++i)
			{
				auto arg = caller_thread->pop(); // we're still calling m_thread pop because we want to pop the arguments
											   // off that thread
				if (i >= fn->parameters.size())
					continue;
				auto &parm = fn->parameters[i];
				//printf("setting parameter %s to %s\n", parm.c_str(), variant_to_string_for_dump(arg).c_str());
				fc.variables[util::string::to_lower(parm)] = arg;
			}

			fc.instruction_index = 0;
			fc.file_name = fn->file;
			fc.function_name = fn->name;
			fc.function = fn;
			fc.self_object = obj;
			fc.variables["self"] = fc.self_object;

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

		void VirtualMachine::notify(ThreadContext* thread, vm::ObjectPtr obj, size_t numargs)
		{
			std::string evstr = thread->context()->get_string(0);
			thread->pop();

			std::vector<vm::Variant> args;
			for (size_t i = 0; i < numargs - 1; ++i)
			{
				auto vp = thread->pop();
				args.push_back(vp);
			}
			notify_event_string(obj, evstr, &args);
			thread->push(vm::Undefined());
		}
		void VirtualMachine::waittill(ThreadContext* thread, vm::ObjectPtr obj, const std::string event_str,
									  std::vector<std::string>& vars)
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
							auto &var = fc->get_variable(parameters[i]);
							var = ne.arguments[i];
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
			l->fc = &thread->function_context();
			l->vm = this;
			l->object = obj;
			l->string = event_str;
			thread->m_locks.push_back(std::move(l));
			thread->push(vm::Undefined());
		}
		void VirtualMachine::endon(ThreadContext* thread, vm::ObjectPtr obj, size_t numargs)
		{
			thread->pop(numargs);
			thread->push(vm::Undefined());
		}

		void VirtualMachine::call_builtin_method(ThreadContext* thread, vm::ObjectPtr obj, const std::string function,
												 size_t numargs)
		{
			int num_pushed = 0;
			thread->m_context->set_number_of_arguments(numargs);
			if (!obj->call_method(util::string::to_lower(function), *thread->m_context.get(), &num_pushed))
			{
				throw vm::Exception("no method {} found for object {}", function, obj->m_tag);
				return;
			}
			if (num_pushed == 0)
			{
				thread->pop(numargs);
				thread->push(vm::Undefined());
			}
			else
			{
				auto tmp = thread->pop();
				thread->pop(numargs);
				thread->push(tmp);
			}
		}
		void VirtualMachine::call_builtin(ThreadContext* thread, const std::string function, size_t numargs)
		{
			auto fnd = m_stockfunctions.find(util::string::to_lower(function));
			if (fnd == m_stockfunctions.end())
				throw vm::Exception("no function named {}", function);
			thread->m_context->set_number_of_arguments(numargs);
			int num_pushed = fnd->second(*thread->m_context.get());

			if (num_pushed == 0)
			{
				thread->pop(numargs);
				thread->push(vm::Undefined());
			}
			else
			{
				auto tmp = thread->pop();
				thread->pop(numargs);
				thread->push(tmp);
			}
		}
		
		void VirtualMachine::call_function(ThreadContext* thread, vm::ObjectPtr obj, const std::string& file,
										   const std::string& function, size_t numargs, bool is_method)
		{
			auto& fc = thread->function_context();
			auto* fn = find_function_in_file(file, function);
			bool function_exists = function_exists = fn != nullptr;

			if (is_method)
			{
				if (function == "endon")
				{
					endon(thread, obj, numargs);
					return;
				}
				else if (function == "notify")
				{
					notify(thread, obj, numargs);
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
				call_impl(thread, thread, obj, fn, numargs);
				return;
			}
			if (!is_method)
				call_builtin(thread, function, numargs);
			else
				call_builtin_method(thread, obj, function, numargs);
		}

		std::shared_ptr<vm::Instruction> VirtualMachine::fetch(ThreadContext* tc)
		{
			auto& fc = tc->function_context();
			if (fc.instruction_index >= fc.function->instructions.size())
				return nullptr;
			return fc.function->instructions[fc.instruction_index++];
		}

		Variant VirtualMachine::get_variable(ThreadContext* thread, const std::string var)
		{
			auto& fc = thread->function_context();
			if (var == "level")
				return level_object;
			else if (var == "game")
			{
				return game_object;
			}
			/*
			* //should add a new local variable with "self" so we can overwrite it
			* //sometimes we wanna do that i guess
			else if (var == "self")
			{
				return fc.self_object;
			}
			*/
			return fc.get_variable(var);
		}
		Variant* VirtualMachine::get_variable_reference(ThreadContext* thread, const std::string var)
		{
			auto& fc = thread->function_context();
			if (var == "level")
				return &level_object;
			else if (var == "game")
			{
				return &game_object;
			}
			return &fc.get_variable(var);
		}

		bool VirtualMachine::run_thread(ThreadContext *tc)
		{
			while (1)
			{
				for (auto lock_iterator = tc->m_locks.begin(); lock_iterator != tc->m_locks.end();)
				{
					if ((*lock_iterator)->locked())
					{
						return false;
					}
					lock_iterator = tc->m_locks.erase(lock_iterator);
				}
				if (tc->marked_for_deletion)
					break;
				auto instr = fetch(tc);
				if (!instr)
					throw vm::Exception("shouldn't be nullptr");
				auto& fc = tc->function_context();
				if (m_flags & flags::kVerbose)
				{
					printf("\t\t-->%s (%d)\t%s::%s\n", instr->to_string().c_str(), tc->m_stack.size(),
						   fc.file_name.c_str(), fc.function_name.c_str());
				}
				instr->execute(*this, tc);
			}
			return true;
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
					run_thread(thread.get());
				}
				//Sleep(1000 / 20);
			}
			++frame_number;
		}
	}; // namespace vm
};	   // namespace script