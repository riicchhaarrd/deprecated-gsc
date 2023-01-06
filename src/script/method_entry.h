#pragma once
#include <memory>
#include <script/vm/types.h>
#include <string>
#include <unordered_map>

namespace script
{
	struct VMContext;

	struct IMethod
	{
		virtual int execute(VMContext&, std::shared_ptr<void>) = 0;
	};

	template <typename T> struct Method : IMethod
	{
		virtual int execute(VMContext& ctx, std::shared_ptr<void> ptr) override
		{
			return execute_impl(ctx, std::static_pointer_cast<T>(ptr));
		}
		virtual int execute_impl(VMContext&, std::shared_ptr<T>) = 0;
	};

	template <typename T> class MethodEntry
	{
	  public:
		using Entries = std::unordered_map<std::string, Method<T>*>;

		static Entries& get_methods()
		{
			static Entries methods;
			return methods;
		}

		MethodEntry(const char* name, Method<T>* entry)
		{
			get_methods()[name] = entry;
		}
	};
}; // namespace script