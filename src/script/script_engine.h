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
		std::string m_library_path;
	  public:
		void set_library_path(const std::string& path)
		{
			m_library_path = path;
		}
		ScriptEngine(filesystem_api&);
		~ScriptEngine();
		bool load_file(const std::string);
		bool load_file(filesystem_api& fs, const std::string);
		void create_virtual_machine();
		void execute_thread(vm::ObjectPtr, const std::string, const std::string, size_t);
		void execute_thread(vm::ObjectPtr, const std::string, const std::string, std::vector<vm::Variant>& args);
		void register_function(const std::string name, StockFunction sf);
		void clear_functions()
		{
			m_registeredfunctions.clear();
		}
		std::unique_ptr<vm::VirtualMachine>& get_virtual_machine()
		{
			return m_vm;
		}
		void notify(vm::ObjectPtr, const std::string, std::vector<vm::Variant>* args = nullptr);
		void run();

		template <typename T, typename... Ts> vm::ObjectPtr create_object(Ts... ts)
		{
			return std::make_shared<T>(ts...);
		}
	};
};