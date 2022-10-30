#include "stockfunctions.h"
#include <script/vm/virtual_machine.h>
#include <unordered_map>
#include <string>
#include <core/time.h>
#include <math.h>
#include <float.h>

namespace script
{
	namespace functions
	{
		int randomint(script::VMContext& ctx)
		{
			//ctx.add_int(rand());
			ctx.add_int(0);
			return 1;
		}
		int randomintrange(script::VMContext& ctx)
		{
			int min = ctx.get_int(0);
			int max = ctx.get_int(1);
			int r = rand() % (max - min) + min;
			ctx.add_int(r);
			return 1;
		}
		int getteamscore(script::VMContext& ctx)
		{
			ctx.add_int(0);
			return 1;
		}
		int positionwouldtelefrag(script::VMContext& ctx)
		{
			ctx.add_bool(0);
			return 1;
		}
		int randomfloat(script::VMContext& ctx)
		{
			ctx.add_float(0.f);
			//ctx.add_float((float)rand() / (float)0xffff);
			return 1;
		}
		int getchar_(script::VMContext& ctx)
		{
			getchar();
			return 0;
		}
		int unimplemented(script::VMContext& ctx)
		{
			return 0;
		}
		int tolower(script::VMContext& ctx)
		{
			ctx.add_string(util::string::to_lower(ctx.get_string(0)));
			return 1;
		}
		int is_defined(script::VMContext& ctx)
		{
			auto v = ctx.get_variant(0);
			ctx.add_bool(v.index() != 0);
			return 1;
		}
		int typeof(script::VMContext& ctx)
		{
			auto v = ctx.get_variant(0);
			ctx.add_string(script::vm::kVariantNames[v.index()]);
			return 1;
		}
		int vectornormalize(script::VMContext& ctx)
		{
			vm::Vector v;
			ctx.get_vector(0, v);
			float len = v.length();
			if (len <= FLT_EPSILON)
				len = 0.00001f; // throw vm::Exception("divide by zero");
			v.x /= len;
			v.y /= len;
			v.z /= len;
			ctx.add_vector(v);
			return 1;
		}
		int spawnstruct(script::VMContext& ctx)
		{
			auto o = std::make_shared<vm::Object>("spawnstruct");
			ctx.add_object(o);
			return 1;
		}
		int getaiarray(script::VMContext& ctx)
		{
			auto o = std::make_shared<vm::Object>("getaiarray");
			ctx.add_object(o);
			return 1;
		}
		int getspawnerarray(script::VMContext& ctx)
		{
			auto o = std::make_shared<vm::Object>("getspawnerarray");
			ctx.add_object(o);
			return 1;
		}
		int getvehiclenodearray(script::VMContext& ctx)
		{
			auto o = std::make_shared<vm::Object>("getvehiclenodearray");
			ctx.add_object(o);
			return 1;
		}
		int getallvehiclenodes(script::VMContext& ctx)
		{
			auto o = std::make_shared<vm::Object>("getallvehiclenodes");
			ctx.add_object(o);
			return 1;
		}
		int issplitscreen(script::VMContext& ctx)
		{
			ctx.add_bool(false);
			return 1;
		}
		int gettime(script::VMContext& ctx)
		{
			ctx.add_int(core::time_milliseconds());
			return 1;
		}
		int unimplemented_vector(script::VMContext& ctx)
		{
			ctx.add_vector(vm::Vector());
			return 1;
		}
		int loadfx(script::VMContext& ctx)
		{
			ctx.add_int(-1);
			return 1;
		}
		int print(script::VMContext& ctx)
		{
			for (size_t i = 0; i < ctx.number_of_arguments(); ++i)
				printf("%s ", ctx.get_string(i).c_str());
			printf("\n");
			return 0;
		}
		int distance(script::VMContext& ctx)
		{
			vm::Vector a, b;
			ctx.get_vector(0, a);
			ctx.get_vector(1, b);

			ctx.add_float(a.distance(b));
			return 1;
		}
		int getguid(script::VMContext& ctx)
		{
			ctx.add_int(0);
			return 1;
		}
		int getdifficulty(script::VMContext& ctx)
		{
			ctx.add_string("medium");
			return 1;
		}
		int set_exp_fog(script::VMContext& ctx)
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
			{"typeof", typeof},
			{"loadfx", loadfx},
			{"positionwouldtelefrag", positionwouldtelefrag},
			{"tolower", tolower},
			{"distance", distance},
			{"spawnstruct", spawnstruct},
			{"resettimeout", unimplemented},
			{"setteamscore", unimplemented},
			{"objective_add", unimplemented},
			{"objective_delete", unimplemented},
			{"objective_current", unimplemented},
			{"objective_icon", unimplemented},
			{"objective_onentity", unimplemented},
			{"objective_position", unimplemented},
			{"objective_ring", unimplemented},
			{"objective_state", unimplemented},
			{"objective_string", unimplemented},
			{"objective_string_nomessage", unimplemented},
			{"objective_team", unimplemented},
			{"objective_additionalcurrent", unimplemented},
			{"objective_additionalposition", unimplemented},
			{"setvalue", unimplemented},
			{"randomint", randomint},
			{"randomintrange", randomintrange},
			{"getteamscore", getteamscore},
			{"randomfloat", randomfloat},
			{"gettime", gettime},
			{"vectornormalize", vectornormalize},
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
			{"precacherumble", unimplemented},
			{"updatescores", unimplemented},
			{"precachestatusicon", unimplemented},
			{"setarchive", unimplemented},
			{"precachestring", unimplemented},
			{"setclientnamemode", unimplemented},
			{"precacheheadicon", unimplemented},
			{"makecvarserverinfo", unimplemented},
			{"setsavedcvar", unimplemented},
			{"assert", unimplemented},
			{"setnormalhealth", unimplemented},
			{"getdifficulty", getdifficulty},
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
