#include "instructions.h"
#include <script/vm/virtual_machine.h>

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
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void CallFunction::execute(VirtualMachine& vm)
		{
			if (is_threaded)
				throw vm::Exception("threaded unhandled {}", __LINE__);
			if (is_method_call)
				throw vm::Exception("method unhandled {}", __LINE__);
			vm.call(this->function, this->numargs);
		}
		void JumpNotZero::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Constant1::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Constant0::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void JumpZero::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Jump::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Test::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Compare::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void CaseEnd::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Label::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void BinOp::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void WaitTillFrameEnd::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Wait::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Ret::execute(VirtualMachine& vm)
		{
			vm.ret();
		}
		void Not::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
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
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Div::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Mul::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Sub::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Or::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void And::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
		}
		void Add::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
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
		void Switch::execute(VirtualMachine& vm)
		{
			throw vm::Exception("unhandled instruction {}", __LINE__);
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
