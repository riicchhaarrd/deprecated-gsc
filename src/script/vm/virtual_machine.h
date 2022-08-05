#pragma once
#include <common/exception.h>
#include "instruction.h"
#include <script/vm/instructions/instructions.h>
#include "types.h"
#include "function.h"

namespace script
{
	namespace vm
	{
		struct ExceptionData
		{
		};
		using Exception = common::TypedDataMessageException<ExceptionData>;
		class VirtualMachine
		{
			//eh... i could combine all the "opcodes" into one large set and just use a singular instruction pointer
			size_t function_index = 0;
			size_t instruction_index = 0;
			Constants &m_constants;
			std::vector<Function> &m_functions;
			Function* m_function;
		  public:
			VirtualMachine(Constants&, std::vector<Function>&);
			void run();
			bool reached_end();
			void set_function(size_t index);
			std::unique_ptr<Instruction>& fetch();
		};
	};
};