#pragma once
#include <common/exception.h>
#include "instruction.h"
#include <script/vm/instructions/instructions.h>
#include "types.h"
#include "function.h"
#include <functional>

namespace script
{
	struct VMContext
	{
		virtual std::string get_string(size_t) = 0;
		virtual int get_int(size_t) = 0;
		virtual float get_float(size_t) = 0;
		virtual vm::Variant get_variant(size_t) = 0;
		virtual void add_bool(const bool b)
		{
			add_int(b ? 1 : 0);
		}
		virtual void add_int(const int) = 0;
		virtual void add_float(const float) = 0;
		virtual void add_string(const std::string) = 0;
	};

	using StockFunction = std::function<int(VMContext&, vm::Object*)>;

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