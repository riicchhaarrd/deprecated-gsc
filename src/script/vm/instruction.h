#pragma once
#include <string>
#include <memory>
#include <common/type_id.h>
#include <script/debug_info.h>

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
	}                                                                                                                  \
	virtual size_t kind()                                                                                              \
	{                                                                                                                  \
		return type_id<x>::id();                                                                                       \
	}

#define DEFINE_INSTRUCTION_ONLY_KIND(x)                                                                                \
	virtual size_t kind()                                                                                              \
	{                                                                                                                  \
		return type_id<x>::id();                                                                                       \
	}

		//TODO: serialize/deserialize put instructions into a map with assigning them a ID
		//e.g instructions[some_number++] = new InstructionType();
		struct Instruction
		{
			size_t m_id;
			DebugInfo debug;
			void set_id(size_t id)
			{
				m_id = id;
			}
			virtual ~Instruction()
			{
			}
			virtual size_t kind() = 0;
			template <typename T> T* cast()
			{
				constexpr auto k = type_id<T>::id();
				if (k != this->kind())
					return NULL;
				return (T*)this;
			}
			virtual std::string to_string() = 0;

			virtual void execute(VirtualMachine&, ThreadContext*) = 0;
		};
		using InstructionPtr = std::unique_ptr<Instruction>;
	}; // namespace vm
};	   // namespace script