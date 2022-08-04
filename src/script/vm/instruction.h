#pragma once
#include <memory>

namespace script
{
	namespace vm
	{
		class VirtualMachine;

		//TODO: serialize/deserialize put instructions into a map with assigning them a ID
		//e.g instructions[some_number++] = new InstructionType();
		struct Instruction
		{
			virtual ~Instruction()
			{
			}

			virtual void execute(VirtualMachine&) = 0;
		};
		using InstructionPtr = std::unique_ptr<Instruction>;
	}; // namespace vm
};	   // namespace script