#pragma once
#include <script/vm/instruction.h>
#include <common/format.h>

namespace script
{
	namespace vm
	{
		struct PushInteger : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(PushInteger)
			virtual std::string to_string();
			int value;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Pop : Instruction
		{
			DEFINE_INSTRUCTION(Pop)
			int value;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushNumber : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(PushNumber)
			virtual std::string to_string()
			{
				return common::format("PushNumber {}", value);
			}
			float value;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushVector : Instruction
		{
			DEFINE_INSTRUCTION(PushVector)
			size_t nelements = 0;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushArray : Instruction
		{
			DEFINE_INSTRUCTION(PushArray)
			size_t nelements = 0;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushString : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(PushString)
			virtual std::string to_string()
			{
				return common::format("PushString {}", value);
			}
			std::string value;
			size_t length;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushLocalizedString : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(PushLocalizedString)
			virtual std::string to_string()
			{
				return common::format("PushLocalizedString {}", value);
			}
			std::string value;
			size_t length;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushFunctionPointer : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(PushFunctionPointer)
			virtual std::string to_string()
			{
				return common::format("PushFunctionPointer {}::{}", file, function);
			}
			std::string file;
			std::string function;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		#if 0
		struct LoadSelfRef : Instruction
		{
			virtual void execute(VirtualMachine& vm, ThreadContext *) override
			{
			}
		};
		struct LoadLevelRef : Instruction
		{
			virtual void execute(VirtualMachine& vm, ThreadContext *) override
			{
			}
		};
		struct LoadGameRef : Instruction
		{
			virtual void execute(VirtualMachine& vm, ThreadContext *) override
			{
			}
		};
		#endif
		struct PushAnimationString : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(PushAnimationString)
			virtual std::string to_string()
			{
				return common::format("PushAnimationString %{}", value);
			}
			std::string value;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct PushUndefined : Instruction
		{
			DEFINE_INSTRUCTION(PushUndefined)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Nop : Instruction
		{
			DEFINE_INSTRUCTION(Nop)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct LoadRef : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(LoadRef)
			virtual std::string to_string()
			{
				return common::format("LoadRef {}", variable_name);
			}
			std::string variable_name;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct LoadValue : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(LoadValue)
			virtual std::string to_string()
			{
				return common::format("LoadValue {}", variable_name);
			}
			std::string variable_name;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct StoreRef : Instruction
		{
			DEFINE_INSTRUCTION_ONLY_KIND(StoreRef)
			virtual std::string to_string()
			{
				return common::format("StoreRef");
			}
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct LoadObjectFieldRef : Instruction
		{
			DEFINE_INSTRUCTION(LoadObjectFieldRef)
			int op;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct LoadObjectFieldValue : Instruction
		{
			DEFINE_INSTRUCTION(LoadObjectFieldValue)
			int op;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Not : Instruction
		{
			DEFINE_INSTRUCTION(Not)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct LogicalNot : Instruction
		{
			DEFINE_INSTRUCTION(LogicalNot)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Ret : Instruction
		{
			DEFINE_INSTRUCTION(Ret)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Wait : Instruction
		{
			DEFINE_INSTRUCTION(Wait)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct WaitTillFrameEnd : Instruction
		{
			DEFINE_INSTRUCTION(WaitTillFrameEnd)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		//got "lazy" and just wanted to get things working so i'm kinda mapping ast nodes 1:1 to instructions now
		struct BinOp : Instruction
		{
			DEFINE_INSTRUCTION(BinOp)
			int op;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};

		struct Label : Instruction
		{
			size_t label_index = 0;
			DEFINE_INSTRUCTION(Label)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};

		struct Test : Instruction
		{
			DEFINE_INSTRUCTION(Test)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Jump : Instruction
		{
			DEFINE_INSTRUCTION(Jump)
			std::weak_ptr<Label> dest;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct JumpZero : Instruction
		{
			DEFINE_INSTRUCTION(JumpZero)
			std::weak_ptr<Label> dest;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Constant0 : Instruction
		{
			DEFINE_INSTRUCTION(Constant0)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct Constant1 : Instruction
		{
			DEFINE_INSTRUCTION(Constant1)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct JumpNotZero : Instruction
		{
			DEFINE_INSTRUCTION(JumpNotZero)
			std::weak_ptr<Label> dest;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};

		struct Call : Instruction
		{
			DEFINE_INSTRUCTION(Call)
			bool is_method_call = false;
			bool is_threaded = false;
			size_t numargs = 0;
			virtual void execute(VirtualMachine& vm, ThreadContext *) = 0;
		};
		struct WaitTill : Instruction
		{
			DEFINE_INSTRUCTION(WaitTill)
			bool is_method_call = false;
			size_t numargs = 0;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};

		struct CallFunction : Call
		{
			DEFINE_INSTRUCTION_ONLY_KIND(CallFunction)
			virtual std::string to_string()
			{
				return common::format("CallFunction {}", function);
			}
			std::string function;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
		struct CallFunctionFile : Call
		{
			DEFINE_INSTRUCTION_ONLY_KIND(CallFunctionFile)
			std::string file;
			std::string function;
			virtual void execute(VirtualMachine& vm, ThreadContext *);
			virtual std::string to_string()
			{
				return common::format("CallFunctionFile {}::{}", file, function);
			}
		};
		struct CallFunctionPointer : Call
		{
			DEFINE_INSTRUCTION(CallFunctionPointer)
			virtual void execute(VirtualMachine& vm, ThreadContext *);
		};
	}; // namespace vm
};	   // namespace script
