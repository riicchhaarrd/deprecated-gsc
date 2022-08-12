#pragma once
#include <core/filesystem/api.h>
#include <script/compiler/compiler.h>
#include <script/vm/types.h>

namespace script
{
	namespace vm
	{
		class VirtualMachine;
	};

	class ScriptEngine
	{
		filesystem_api& m_fs;
		std::unique_ptr<script::vm::VirtualMachine> m_vm;
		script::compiler::CompiledFiles m_compiledfiles;
	  public:
		ScriptEngine(filesystem_api&);
		~ScriptEngine();
		bool load(const std::string);
		void execute_thread(vm::VariantPtr, const std::string, const std::string, size_t);
		void run();
	};
};