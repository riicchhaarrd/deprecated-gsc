#pragma once
#include <string>
#include <memory>

namespace script
{
	namespace vm
	{
		class VirtualMachine;
		struct ThreadContext;

#define DEFINE_INSTRUCTION(x)                                                                                          \
	virtual std::string to_string() override                                                                           \
	{                                                                                                                  \
		return #x;                                                                                                     \
	}

		//TODO: serialize/deserialize put instructions into a map with assigning them a ID
		//e.g instructions[some_number++] = new InstructionType();
		struct Instruction
		{
			size_t m_id;

			void set_id(size_t id)
			{
				m_id = id;
			}
			virtual ~Instruction()
			{
			}

			virtual std::string to_string() = 0;

			virtual void execute(VirtualMachine&, ThreadContext*) = 0;
		};
		using InstructionPtr = std::unique_ptr<Instruction>;
	}; // namespace vm
};	   // namespace script