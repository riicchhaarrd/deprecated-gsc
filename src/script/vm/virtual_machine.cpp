#include "virtual_machine.h"

namespace script
{
	namespace vm
	{
		VirtualMachine::VirtualMachine(Constants& constants, std::vector<Function>& functions)
			: m_constants(constants), m_functions(functions)
		{
			Function* main_function = nullptr;
			//find main
			for (auto& it : m_functions)
			{
				if (it.name == "main")
				{
					main_function = &it;
					break;
				}
			}

			if (!main_function)
			{
				throw Exception("No main function found");
			}
			set_function(main_function->index);
		}
		void VirtualMachine::set_function(size_t index)
		{
			if (index >= m_functions.size())
				throw Exception("invalid function index");
			function_index = index;
			m_function = &m_functions[function_index];
		}

		bool VirtualMachine::reached_end()
		{
			return instruction_index >= m_function->instructions.size();
		}
		std::unique_ptr<Instruction>& VirtualMachine::fetch()
		{
			if (instruction_index >= m_function->instructions.size())
				throw Exception("invalid instruction index");
			return m_function->instructions[instruction_index++];
		}

		void VirtualMachine::run()
		{
			while (1)
			{
				printf("%s] %d/%d\n", m_function->name.c_str(), instruction_index, m_function->instructions.size());
				if (reached_end())
				{
					break;
				}
				auto &instr = fetch();
				instr->execute(*this);
			}
		}
	}; // namespace vm
};	   // namespace script