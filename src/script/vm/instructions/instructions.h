#pragma once
#include <script/vm/instruction.h>

namespace script
{
	namespace vm
	{
		struct LoadInstruction : Instruction
		{
			size_t offset;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct AddInstruction : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct SubInstruction : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct MulInstruction : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct DivInstruction : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct ModInstruction : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct LabelInstruction : Instruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct JumpInstruction : Instruction
		{
			std::weak_ptr<LabelInstruction> dest;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};

		struct CallInstruction : Instruction
		{
			bool is_method_call = false;
			bool is_threaded = false;
			size_t numargs = 0;
			virtual void execute(VirtualMachine& vm) = 0;
		};

		struct CallFunctionInstruction : CallInstruction
		{
			size_t function_index;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct CallBuiltinInstruction : CallInstruction
		{
			size_t name;
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct CallFunctionPointerInstruction : CallInstruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
			}
		};
		struct HaltInstruction : CallInstruction
		{
			virtual void execute(VirtualMachine& vm) override
			{
				getchar();
				printf("Exiting");
				exit(0);
			}
		};
	};
};