#include "instructions.h"
#include <script/vm/virtual_machine.h>
#include <Windows.h>

namespace script
{
	namespace vm
	{
		std::string PushInteger::to_string()
		{
			return std::format("PushInteger {}", value);
		}
		void CallFunctionPointer::execute(VirtualMachine& vm)
		{
			vm::ObjectPtr obj = vm.function_context().self_object;
			if (is_method_call)
			{
				obj = vm.context()->get_object(0);
				vm.pop();
			}
			auto vfp = vm.context()->get_variant(0);
			vm.pop();
			if (vfp.index() != (int)vm::Type::kFunctionPointer)
				throw vm::Exception("{} is not a function pointer", vfp.index());
			auto fp = std::get<vm::FunctionPointer>(vfp);
			std::string ref = fp.file;
			std::replace(ref.begin(), ref.end(), '\\', '/');
			if (is_threaded)
			{
				vm.exec_thread(obj, ref, fp.name, numargs, is_method_call);
				vm.push(vm::Undefined()); // thread doesn't return
			}
			else
				vm.call_function(obj, ref, fp.name, this->numargs, is_method_call);
		}
		void CallFunctionFile::execute(VirtualMachine& vm)
		{
			vm::ObjectPtr obj = vm.function_context().self_object;
			if (is_method_call)
			{
				obj = vm.context()->get_object(0);
				vm.pop();
			}
			std::string ref = file;
			std::replace(ref.begin(), ref.end(), '\\', '/');
			if (is_threaded)
			{
				vm.exec_thread(obj, ref, this->function, numargs, is_method_call);
				vm.push(vm::Undefined()); // thread doesn't return
			}
			else
				vm.call_function(obj, ref, this->function, this->numargs, is_method_call);
		}
		void CallFunction::execute(VirtualMachine& vm)
		{
			vm::ObjectPtr obj = vm.function_context().self_object;
			if (is_method_call)
			{
				obj = vm.context()->get_object(0);
				vm.pop();
			}
			if (is_threaded)
			{
				vm.exec_thread(obj, vm.current_file(), this->function, numargs, is_method_call);
				vm.push(vm::Undefined()); //thread doesn't return
			}
			else
				vm.call_function(obj, vm.current_file(), this->function, this->numargs, is_method_call);
		}
		void WaitTill::execute(VirtualMachine& vm)
		{
			vm::ObjectPtr obj;
			if (is_method_call)
			{
				obj = vm.context()->get_object(0);
				vm.pop();
			}
			if (!obj)
				throw vm::Exception("no obj");
			std::string evstr = vm.context()->get_string(0);
			vm.pop();
			std::vector<std::string> vars;
			for (size_t i = 0; i < this->numargs; ++i)
			{
				vars.push_back(vm.context()->get_string(0));
				vm.pop();
			}
			std::reverse(vars.begin(), vars.end());
			vm.waittill(obj, evstr, vars);
		}
		void JumpNotZero::execute(VirtualMachine& vm)
		{
			if ((vm.get_flags() & vm::flags::kZF) != vm::flags::kZF)
			{
				if (!this->dest.expired())
				{
					size_t idx = this->dest.lock()->label_index;
					vm.jump(idx);
				}
			}
		}
		void Constant1::execute(VirtualMachine& vm)
		{
			vm.push(1);
		}
		void Constant0::execute(VirtualMachine& vm)
		{
			vm.push(0);
		}
		void JumpZero::execute(VirtualMachine& vm)
		{
			if (vm.get_flags() & vm::flags::kZF)
			{
				if (!this->dest.expired())
				{
					size_t idx = this->dest.lock()->label_index;
					vm.jump(idx);
				}
			}
		}
		void Jump::execute(VirtualMachine& vm)
		{
			if (!this->dest.expired())
			{
				size_t idx = this->dest.lock()->label_index;
				vm.jump(idx);
			}
		}
		void Test::execute(VirtualMachine& vm)
		{
			auto v = vm.context()->get_variant(0);
			vm.pop();
			if (v.index() == (int)vm::Type::kInteger)
			{
				int intval = std::get<vm::Integer>(v);
				if (intval == 0)
					vm.set_flags(vm.get_flags() | vm::flags::kZF);
				else
					vm.set_flags(vm.get_flags() & ~vm::flags::kZF);
			}
			else if (v.index() == (int)vm::Type::kUndefined)
			{
				vm.set_flags(vm.get_flags() | vm::flags::kZF);
			}
			else
				throw vm::Exception("unexpected {}", v.index());
		}
		void Label::execute(VirtualMachine& vm)
		{
		}
		void BinOp::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.binop(a, b, op));
		}
		void WaitTillFrameEnd::execute(VirtualMachine& vm)
		{
			struct ThreadLockWaitFrame : vm::ThreadLock
			{
				VirtualMachine& vm;
				size_t frame;
				ThreadLockWaitFrame(VirtualMachine& vm_, size_t frame_) : frame(frame_), vm(vm_)
				{
				}
				virtual void notify(NotifyEvent&)
				{
				}
				virtual bool locked()
				{
					return vm.get_frame_number() == frame;
				}
			};
			auto l = std::make_unique<ThreadLockWaitFrame>(vm, vm.get_frame_number());
			vm.current_thread()->m_locks.push_back(std::move(l));
		}
		void Wait::execute(VirtualMachine& vm)
		{
			float duration = vm.context()->get_float(0);
			vm.pop();
			struct ThreadLockWaitDuration : vm::ThreadLock
			{
				uint32_t end_time = 0;
				virtual void notify(NotifyEvent&)
				{
				}
				virtual bool locked()
				{
					return GetTickCount() < end_time;
				}
			};
			auto l = std::make_unique<ThreadLockWaitDuration>();
			l->end_time = GetTickCount() + duration * 1000.f;
			vm.current_thread()->m_locks.push_back(std::move(l));
		}
		void Ret::execute(VirtualMachine& vm)
		{
			vm.ret();
		}
		void Not::execute(VirtualMachine& vm)
		{
			auto i = vm.context()->get_int(0);
			vm.pop();
			vm.push(~i);
		}
		void LogicalNot::execute(VirtualMachine& vm)
		{
			Variant v = vm.context()->get_variant(0);
			vm.pop();
			if (v.index() == (int)vm::Type::kInteger)
			{
				vm.push(!std::get<vm::Integer>(v));
			}
			else if (v.index() == (int)vm::Type::kUndefined)
			{
				vm.push(1);
			}
			else
				throw vm::Exception("unexpected {}", v.index());
		}
		void LoadObjectFieldValue::execute(VirtualMachine& vm)
		{
			//TODO: FIXME we can't actually load anything if ref is undefined...
			auto ref = vm.pop();
			//TODO: FIXME something is still undefined and won't load properly...
			//something with the spawnlogic, maybe it's just not getting any spawns from entities and just stays undefined idk
			#if 1
			if (ref.index() == (int)vm::Type::kUndefined)
			{
				ref = std::make_shared<Object>("object created from undefined");
			}
			#endif
			if (ref.index() != (int)vm::Type::kObject)
			{
				throw vm::Exception("expected object got {}", ref.index());
			}
			std::shared_ptr<vm::Object> obj = std::get<vm::ObjectPtr>(ref);
			auto prop = vm.context()->get_string(0);
			vm.pop(1);
			if (prop == "size")
			{
				vm.push(vm::Integer(obj->fields.size()));
			}
			else
			{
				try
				{
					auto fv = obj->get_field(util::string::to_lower(prop));
					vm.push(fv);
				}
				catch (...)
				{
					throw vm::Exception("failed getting field {}", prop);
				}
			}
		}
		void LoadObjectFieldRef::execute(VirtualMachine& vm)
		{
			auto *ref = vm.pop_reference_value();
			if (ref->index() == (int)vm::Type::kUndefined)
			{
				*ref = std::make_shared<Object>("object created from undefined");
			}
			if (ref->index() != (int)vm::Type::kObject)
			{
				throw vm::Exception("expected object got {}", ref->index());
			}
			std::shared_ptr<vm::Object> obj = std::get<vm::ObjectPtr>(*ref);
			auto prop = vm.context()->get_string(0);
			vm.pop(1);
			if (prop == "size")
				throw vm::Exception("size is read-only");
			vm.push(vm::Reference{.offset = 0, .field = util::string::to_lower(prop), .object = obj});
			try
			{
				vm.push_reference(&obj->get_field(util::string::to_lower(prop)));
			}
			catch (...)
			{
				throw vm::Exception("failed getting field {}", prop);
			}
		}
		void StoreRef::execute(VirtualMachine& vm)
		{
			auto v = vm.pop();
			if (v.index() != (int)vm::Type::kReference)
			{
				throw vm::Exception("not a reference");
			}
			auto rd = std::get<vm::Reference>(v);
			auto *r = vm.pop_reference();
			if (!rd.object)
			{
				*r = vm.context()->get_variant(0);
			}
			else
			{
				vm::Variant value = vm.context()->get_variant(0);
				try
				{
					rd.object->set_field(util::string::to_lower(rd.field), value);
				}
				catch (...)
				{
					throw vm::Exception("failed setting field {} to {}", rd.field, vm.variant_to_string(value));
				}
			}
			vm.pop(1);
		}
		void LoadValue::execute(VirtualMachine& vm)
		{
			vm.push(vm.get_variable(util::string::to_lower(variable_name)));
		}
		void LoadRef::execute(VirtualMachine& vm)
		{
			auto* v = vm.get_variable_reference(util::string::to_lower(variable_name));
			vm.push(vm::Reference());
			vm.push_reference(v);
		}
		void Nop::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void PushUndefined::execute(VirtualMachine& vm)
		{
			vm.push(vm::Undefined());
		}
		void PushAnimationString::execute(VirtualMachine& vm)
		{
			vm::Animation a;
			a.reference = value;
			vm.push(a);
		}
		void PushFunctionPointer::execute(VirtualMachine& vm)
		{
			vm::FunctionPointer fp;
			fp.file = this->file;
			fp.name = this->function;
			vm.push(fp);
		}
		void PushLocalizedString::execute(VirtualMachine& vm)
		{
			vm::LocalizedString s;
			s.reference = this->value;
			vm.push(s);
		}
		void PushString::execute(VirtualMachine& vm)
		{
			vm::String s = value;
			vm.push(s);
		}
		void PushArray::execute(VirtualMachine& vm)
		{
			//TODO: FIXME don't use object as array
			vm::ObjectPtr o = std::make_shared<vm::Object>("pusharray");
			vm.push(o);
		}
		void PushVector::execute(VirtualMachine& vm)
		{
			vm::Vector v;
			v.x = vm.context()->get_float(0);
			v.y = vm.context()->get_float(1);
			v.z = vm.context()->get_float(2);
			vm.pop(3);
			vm.push(v);
		}
		void PushNumber::execute(VirtualMachine& vm)
		{
			vm::Number v = value;
			vm.push(v);
		}
		void PushInteger::execute(VirtualMachine& vm)
		{
			vm::Integer v = value;
			vm.push(v);
		}
		void Pop::execute(VirtualMachine& vm)
		{
			vm.pop();
		}
	}; // namespace vm
};	   // namespace script
