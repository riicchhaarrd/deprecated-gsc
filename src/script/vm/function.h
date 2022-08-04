#pragma once
#include <script/vm/instruction.h>

namespace script
{
	namespace vm
	{
		struct Function
		{
			size_t index = 0;
			std::string name;
			size_t numparameters = 0;
			std::vector<std::unique_ptr<Instruction>> instructions;
		};
	}; // namespace vm
}; // namespace script