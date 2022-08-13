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
			if (vfp->index() != (int)vm::Type::kFunctionPointer)
				throw vm::Exception("{} is not a function pointer", vfp->index());
			auto fp = std::get<vm::FunctionPointer>(*vfp);
			std::string ref = fp.file;
			std::replace(ref.begin(), ref.end(), '\\', '/');
			if (is_threaded)
			{
				vm.exec_thread(obj, ref, fp.name, numargs);
				vm.push(vm.variant(vm::Undefined())); // thread doesn't return
			}
			else
				vm.call(vm.thread(), obj, ref, fp.name, this->numargs);
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
				vm.exec_thread(obj, ref, this->function, numargs);
				vm.push(vm.variant(vm::Undefined())); // thread doesn't return
			}
			else
				vm.call(vm.thread(), obj, ref, this->function, this->numargs);
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
				vm.exec_thread(obj, this->function, numargs);
				vm.push(vm.variant(vm::Undefined())); //thread doesn't return
			}
			else
				vm.call(obj, this->function, this->numargs);
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
			vm.push(vm.variant(1));
		}
		void Constant0::execute(VirtualMachine& vm)
		{
			vm.push(vm.variant(0));
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
			if (v->index() == (int)vm::Type::kInteger)
			{
				int intval = std::get<vm::Integer>(*v);
				if (intval == 0)
					vm.set_flags(vm.get_flags() | vm::flags::kZF);
				else
					vm.set_flags(vm.get_flags() & ~vm::flags::kZF);
			}
			else if (v->index() == (int)vm::Type::kUndefined)
			{
				vm.set_flags(vm.get_flags() | vm::flags::kZF);
			}
			else
				throw vm::Exception("unexpected {}", v->index());
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
			vm.push(vm.variant(vm.binop(*a, *b, op)));
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
				virtual void notify(vm::ObjectPtr, const std::string)
				{
				}
				virtual bool locked()
				{
					return vm.get_frame_number() == frame;
				}
			};
			auto l = std::make_unique<ThreadLockWaitFrame>(vm, vm.get_frame_number());
			vm.thread()->m_locks.push_back(std::move(l));
		}
		void Wait::execute(VirtualMachine& vm)
		{
			float duration = vm.context()->get_float(0);
			vm.pop();
			struct ThreadLockWaitDuration : vm::ThreadLock
			{
				uint32_t end_time = 0;
				virtual void notify(vm::ObjectPtr, const std::string)
				{
				}
				virtual bool locked()
				{
					return GetTickCount() < end_time;
				}
			};
			auto l = std::make_unique<ThreadLockWaitDuration>();
			l->end_time = GetTickCount() + duration * 1000.f;
			vm.thread()->m_locks.push_back(std::move(l));
		}
		void Ret::execute(VirtualMachine& vm)
		{
			vm.ret();
		}
		void Not::execute(VirtualMachine& vm)
		{
			auto i = vm.context()->get_int(0);
			vm.pop();
			vm.push(vm.variant(~i));
		}
		void LogicalNot::execute(VirtualMachine& vm)
		{
			auto v = vm.context()->get_variant(0);
			vm.pop();
			if (v->index() == (int)vm::Type::kInteger)
			{
				vm.push(vm.variant(!std::get<vm::Integer>(*v)));
			}
			else if (v->index() == (int)vm::Type::kUndefined)
			{
				vm.push(vm.variant(1));
			}
			else
				throw vm::Exception("unexpected {}", v->index());
		}
		void LoadObjectFieldValue::execute(VirtualMachine& vm)
		{
			auto obj = vm.context()->get_object(0);
			auto prop = vm.context()->get_string(1);
			vm.pop(2);
			vm.push(obj->get_field(prop));
		}
		void LoadObjectFieldRef::execute(VirtualMachine& vm)
		{
			auto obj = vm.context()->get_object(0);
			auto prop = vm.context()->get_string(1);
			vm.pop(2);
			vm.push(obj->get_field(prop));
		}
		void StoreRef::execute(VirtualMachine& vm)
		{
			auto v = vm.context()->get_variant(0);
			auto val = vm.context()->get_variant(1);
			*v = *val;
			vm.pop(2);
		}
		void LoadValue::execute(VirtualMachine& vm)
		{
			vm.push(vm.get_variable(this->variable_name));
		}
		void LoadRef::execute(VirtualMachine& vm)
		{
			vm.push(vm.get_variable(this->variable_name));
		}
		void Nop::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void PushUndefined::execute(VirtualMachine& vm)
		{
			vm.push(vm.variant(vm::Undefined()));
		}
		void PushAnimationString::execute(VirtualMachine& vm)
		{
			vm::Animation a;
			a.reference = value;
			vm.push(vm.variant(a));
		}
		void PushFunctionPointer::execute(VirtualMachine& vm)
		{
			vm::FunctionPointer fp;
			fp.file = this->file;
			fp.name = this->function;
			vm.push(vm.variant(fp));
		}
		void PushLocalizedString::execute(VirtualMachine& vm)
		{
			vm::LocalizedString s;
			s.reference = this->value;
			vm.push(vm.variant(s));
		}
		void PushString::execute(VirtualMachine& vm)
		{
			vm::String s = value;
			vm.push(vm.variant(s));
		}
		void PushArray::execute(VirtualMachine& vm)
		{
			//TODO: FIXME don't use object as array
			vm::ObjectPtr o = std::make_shared<vm::Object>();
			vm.push(vm.variant(o));
		}
		void PushVector::execute(VirtualMachine& vm)
		{
			vm::Vector v;
			v.x = vm.context()->get_float(0);
			v.y = vm.context()->get_float(1);
			v.z = vm.context()->get_float(2);
			vm.pop(3);
			vm.push(vm.variant(v));
		}
		void PushNumber::execute(VirtualMachine& vm)
		{
			vm::Number v = value;
			vm.push(vm.variant(v));
		}
		void PushInteger::execute(VirtualMachine& vm)
		{
			vm::Integer v = value;
			vm.push(vm.variant(v));
		}
		void Pop::execute(VirtualMachine& vm)
		{
			vm.pop();
		}
	}; // namespace vm
};	   // namespace script
