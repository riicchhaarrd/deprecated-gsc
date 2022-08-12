#pragma once
#include <core/filesystem/api.h>
#include <script/compiler/compiler.h>
#include <script/vm/types.h>
#include <script/vm/virtual_machine.h>

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
		std::unordered_map<std::string, StockFunction> m_registeredfunctions;
	  public:
		ScriptEngine(filesystem_api&);
		~ScriptEngine();
		bool load(const std::string);
		void execute_thread(vm::VariantPtr, const std::string, const std::string, size_t);
		void register_function(const std::string name, StockFunction sf);
		void clear_functions()
		{
			m_registeredfunctions.clear();
		}
		void run();

		template <typename T> vm::VariantPtr create_object()
		{
			return std::make_shared<vm::Variant>(std::make_shared<T>());
		}
	};
};