#include "stockfunctions.h"
#include <script/vm/virtual_machine.h>
#include <unordered_map>
#include <string>

namespace script
{
	namespace functions
	{
		int unimplemented(script::VMContext& ctx, script::vm::Object* obj)
		{
			return 0;
		}
		int is_defined(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto v = ctx.get_variant(0);
			printf("adding bool \n");
			ctx.add_bool(v->index() != 0);
			return 1;
		}
		int loadfx(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_int(-1);
			return 1;
		}
		int print(script::VMContext& ctx, script::vm::Object* obj)
		{
			printf("%s", ctx.get_string(0).c_str());
			return 0;
		}
		int get_cvar(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto var = ctx.get_string(0);
			printf("getcvar %s\n", var.c_str());
			ctx.add_string("");
			return 1;
		}
		int set_exp_fog(script::VMContext& ctx, script::vm::Object* obj)
		{
			printf("float: %f\n", ctx.get_float(0));
			printf("float: %f\n", ctx.get_float(1));
			printf("float: %f\n", ctx.get_float(2));
			printf("float: %f\n", ctx.get_float(3));
			printf("float: %f\n", ctx.get_float(4));
			return 0;
		}

		std::unordered_map<std::string, StockFunction> stockfunctions = {
			{"setexpfog", set_exp_fog},
			{"isdefined", is_defined},
			{"loadfx", loadfx},
			{"setsavedcvar", unimplemented},
			{"precachemodel", unimplemented},
			{"precacheshader", unimplemented},
			{"assertex", unimplemented},
			{"precacheturret", unimplemented},
			{"precachevehicle", unimplemented},
			{"print", print},
			{"getcvar", get_cvar}
		};
	}; // namespace functions

	void register_stockfunctions(vm::VirtualMachine& vm)
	{
		for (auto& it : functions::stockfunctions)
		{
			vm.register_function(it.first, it.second);
		}
		//vm.register_function("setExpFog", [](script::VMContext& context, script::vm::Object* obj) -> int { return 0; });
	}
}; // namespace script