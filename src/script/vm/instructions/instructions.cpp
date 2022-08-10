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
		void Halt::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void CallFunctionPointer::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void CallFunctionFile::execute(VirtualMachine& vm)
		{
			VariantPtr obj = nullptr;
			if (is_method_call)
			{
				obj = vm.context()->get_variant(0);
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
			VariantPtr obj = nullptr;
			if (is_method_call)
			{
				obj = vm.context()->get_variant(0);
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
			int intval = vm.context()->get_int(0);
			vm.pop();
			if (intval != 0)
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
			int intval = vm.context()->get_int(0);
			vm.pop();
			if (intval == 0)
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
				vm.push(vm.variant(intval != 0 ? 1 : 0));
			}
			else if (v->index() == (int)vm::Type::kUndefined)
			{
				vm.push(vm.variant(0));
			}
			else
				throw vm::Exception("unexpected {}", v->index());
		}
		void Compare::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, parse::TokenType_kEq)));
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
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Wait::execute(VirtualMachine& vm)
		{
			float duration = vm.context()->get_float(0);
			vm.pop();
			struct ThreadLockWaitDuration : vm::ThreadLock
			{
				uint32_t end_time = 0;
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
		void Negate::execute(VirtualMachine& vm)
		{
			auto v = vm.context()->get_variant(0);
			vm.pop();
			vm::Variant zero(0);
			vm.push(vm.variant(vm.binop(zero, *v, '-')));
		}
		void Mod::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '%')));
		}
		void Div::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '/')));
		}
		void Mul::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '*')));
		}
		void Sub::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '-')));
		}
		void Or::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '|')));
		}
		void And::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '&')));
		}
		void Add::execute(VirtualMachine& vm)
		{
			auto a = vm.context()->get_variant(0);
			auto b = vm.context()->get_variant(1);
			vm.pop();
			vm.pop();
			vm.push(vm.variant(vm.binop(*a, *b, '+')));
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
			auto v = vm.context()->get_variant(1);
			auto val = vm.context()->get_variant(0);
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
