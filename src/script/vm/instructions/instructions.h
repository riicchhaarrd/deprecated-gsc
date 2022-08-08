#pragma once
#include <script/vm/instruction.h>
#include <format>

namespace script
{
	namespace vm
	{
		struct PushInteger : Instruction
		{
			virtual std::string to_string()
			{
				return std::format("PushInteger {}", value);
			}
			int value;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Pop : Instruction
		{
			DEFINE_INSTRUCTION(Pop)
			int value;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushNumber : Instruction
		{
			virtual std::string to_string()
			{
				return std::format("PushNumber {}", value);
			}
			float value;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushVector : Instruction
		{
			DEFINE_INSTRUCTION(PushVector)
			size_t nelements = 0;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushArray : Instruction
		{
			DEFINE_INSTRUCTION(PushArray)
			size_t nelements = 0;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushString : Instruction
		{
			virtual std::string to_string()
			{
				return std::format("PushString {}", value);
			}
			std::string value;
			size_t length;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushLocalizedString : Instruction
		{
			virtual std::string to_string()
			{
				return std::format("PushLocalizedString {}", value);
			}
			std::string value;
			size_t length;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushFunctionPointer : Instruction
		{
			virtual std::string to_string()
			{
				return std::format("PushFunctionPointer {}::{}", file, function);
			}
			std::string file;
			std::string function;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		#if 0
		struct LoadSelfRef : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct LoadLevelRef : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct LoadGameRef : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		#endif
		struct PushAnimationString : Instruction
		{
			virtual std::string to_string()
			{
				return std::format("PushAnimationString %{}", value);
			}
			std::string value;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct PushUndefined : Instruction
		{
			DEFINE_INSTRUCTION(PushUndefined)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Nop : Instruction
		{
			DEFINE_INSTRUCTION(Nop)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Switch : Instruction
		{
			DEFINE_INSTRUCTION(Switch)
			size_t numcases;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct LoadRef : Instruction
		{
			DEFINE_INSTRUCTION(LoadRef)
			std::string variable_name;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct LoadValue : Instruction
		{
			DEFINE_INSTRUCTION(LoadValue)
			std::string variable_name;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct StoreRef : Instruction
		{
			DEFINE_INSTRUCTION(StoreRef)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct LoadObjectFieldRef : Instruction
		{
			DEFINE_INSTRUCTION(LoadObjectFieldRef)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct LoadObjectFieldValue : Instruction
		{
			DEFINE_INSTRUCTION(LoadObjectFieldValue)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		//all binary expressions instructions take two operands from the stack and push one result back to the stack
		struct Add : Instruction
		{
			DEFINE_INSTRUCTION(Add)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct And : Instruction
		{
			DEFINE_INSTRUCTION(And)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Or : Instruction
		{
			DEFINE_INSTRUCTION(Or)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Sub : Instruction
		{
			DEFINE_INSTRUCTION(Sub)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Mul : Instruction
		{
			DEFINE_INSTRUCTION(Mul)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Div : Instruction
		{
			DEFINE_INSTRUCTION(Div)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Mod : Instruction
		{
			DEFINE_INSTRUCTION(Mod)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Negate : Instruction
		{
			DEFINE_INSTRUCTION(Negate)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Not : Instruction
		{
			DEFINE_INSTRUCTION(Not)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Ret : Instruction
		{
			DEFINE_INSTRUCTION(Ret)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Wait : Instruction
		{
			DEFINE_INSTRUCTION(Wait)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct WaitTillFrameEnd : Instruction
		{
			DEFINE_INSTRUCTION(WaitTillFrameEnd)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		//got "lazy" and just wanted to get things working so i'm kinda mapping ast nodes 1:1 to instructions now
		struct BinOp : Instruction
		{
			DEFINE_INSTRUCTION(BinOp)
			int op;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct Label : Instruction
		{
			DEFINE_INSTRUCTION(Label)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct CaseEnd : Label
		{
			DEFINE_INSTRUCTION(CaseEnd)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct Compare : Instruction
		{
			DEFINE_INSTRUCTION(Compare)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Test : Instruction
		{
			DEFINE_INSTRUCTION(Test)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Jump : Instruction
		{
			DEFINE_INSTRUCTION(Jump)
			std::weak_ptr<Label> dest;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct JumpZero : Instruction
		{
			DEFINE_INSTRUCTION(JumpZero)
			std::weak_ptr<Label> dest;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Constant0 : Instruction
		{
			DEFINE_INSTRUCTION(Constant0)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Constant1 : Instruction
		{
			DEFINE_INSTRUCTION(Constant1)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct JumpNotZero : Instruction
		{
			DEFINE_INSTRUCTION(JumpNotZero)
			std::weak_ptr<Label> dest;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct Call : Instruction
		{
			DEFINE_INSTRUCTION(Call)
			bool is_method_call = false;
			bool is_threaded = false;
			size_t numargs = 0;
			virtual void execute(VirtualMachine& vm) = 0;
		};

		struct CallFunction : Call
		{
			virtual std::string to_string()
			{
				return std::format("CallFunction {}", function);
			}
			std::string function;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct CallFunctionFile : Call
		{
			std::string file;
			std::string function;
			virtual void execute(VirtualMachine& vm) override
			{
			}
			virtual std::string to_string()
			{
				return std::format("CallFunctionFile {}::{}", file, function);
			}
		};
		struct CallFunctionPointer : Call
		{
			DEFINE_INSTRUCTION(CallFunctionPointer)
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct Halt : Instruction
		{
			DEFINE_INSTRUCTION(Halt)
			virtual void execute(VirtualMachine& vm) override
			{
				getchar();
				printf("Exiting");
				exit(0);
			}
		};
	}; // namespace vm
};	   // namespace script