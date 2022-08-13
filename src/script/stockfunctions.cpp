#include "stockfunctions.h"
#include <script/vm/virtual_machine.h>
#include <unordered_map>
#include <string>
#include <Windows.h>

namespace script
{
	namespace functions
	{
		int randomint(script::VMContext& ctx, script::vm::Object* obj)
		{
			//ctx.add_int(rand());
			ctx.add_int(0);
			return 1;
		}
		int positionwouldtelefrag(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_bool(0);
			return 1;
		}
		int randomfloat(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_float(0.f);
			//ctx.add_float((float)rand() / (float)0xffff);
			return 1;
		}
		int getentitynumber(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_int(0);
			return 1;
		}
		int getchar_(script::VMContext& ctx, script::vm::Object* obj)
		{
			getchar();
			return 0;
		}
		int unimplemented(script::VMContext& ctx, script::vm::Object* obj)
		{
			return 0;
		}
		int tolower(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_string(util::string::to_lower(ctx.get_string(0)));
			return 1;
		}
		int is_defined(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto v = ctx.get_variant(0);
			ctx.add_bool(v->index() != 0);
			return 1;
		}
		int spawn(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int getent(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int spawnstruct(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int getaiarray(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int getspawnerarray(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int getvehiclenodearray(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int getallvehiclenodes(script::VMContext& ctx, script::vm::Object* obj)
		{
			auto o = std::make_shared<vm::Object>();
			ctx.add_object(o);
			return 1;
		}
		int issplitscreen(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_bool(false);
			return 1;
		}
		int gettime(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_int(GetTickCount());
			return 1;
		}
		int unimplemented_vector(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_vector({.x = 0.f, .y = 0.f, .z = 0.f});
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
		int distance(script::VMContext& ctx, script::vm::Object* obj)
		{
			vm::Vector a, b;
			ctx.get_vector(0, a);
			ctx.get_vector(1, b);

			ctx.add_float(a.distance(b));
			return 1;
		}
		int getguid(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_int(0);
			return 1;
		}
		int getdifficulty(script::VMContext& ctx, script::vm::Object* obj)
		{
			ctx.add_string("medium");
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
			{"spawn", spawn},
			{"positionwouldtelefrag", positionwouldtelefrag},
			{"tolower", tolower},
			{"distance", distance},
			{"spawnstruct", spawnstruct},
			{"newclienthudelem", spawnstruct},
			{"newhudelem", spawnstruct},
			{"settimer", unimplemented},
			{"setshader", unimplemented},
			{"resettimeout", unimplemented},
			{"stopshellshock", unimplemented},
			{"stoprumble", unimplemented},
			{"randomint", randomint},
			{"randomfloat", randomfloat},
			{"gettime", gettime},
			{"getent", getent},
			{"getchar", getchar_},
			{"getaiarray", getaiarray},
			{"issplitscreen", issplitscreen},
			{"getspawnerarray", getspawnerarray},
			{"getallvehiclenodes", getallvehiclenodes},
			{"getvehiclenodearray", getvehiclenodearray},
			{"vectortoangles", unimplemented_vector},
			{"anglestoforward", unimplemented_vector},
			{"vectorscale", unimplemented_vector},
			{"precacheitem", unimplemented},
			{"precacheshellshock", unimplemented},
			{"prof_begin", unimplemented},
			{"prof_end", unimplemented},
			{"placespawnpoint", unimplemented},
			{"precacherumble", unimplemented},
			{"updatescores", unimplemented},
			{"getentitynumber", getentitynumber},
			{"precachestatusicon", unimplemented},
			{"setarchive", unimplemented},
			{"precachestring", unimplemented},
			{"setclientnamemode", unimplemented},
			{"precacheheadicon", unimplemented},
			{"makecvarserverinfo", unimplemented},
			{"setsavedcvar", unimplemented},
			{"assert", unimplemented},
			{"giveweapon", unimplemented},
			{"setnormalhealth", unimplemented},
			{"takeallweapons", unimplemented},
			{"getdifficulty", getdifficulty},
			{"getguid", getguid},
			{"precachemodel", unimplemented},
			{"precacheshader", unimplemented},
			{"assertex", unimplemented},
			{"precacheturret", unimplemented},
			{"precachevehicle", unimplemented},
			{"logprint", print},
			{"print", print}
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