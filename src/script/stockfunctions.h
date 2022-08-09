#pragma once

namespace script
{
	namespace vm
	{
		class VirtualMachine;
	};
	void register_stockfunctions(vm::VirtualMachine&);
};