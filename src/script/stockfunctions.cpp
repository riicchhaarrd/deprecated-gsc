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
		template<typename... Ts>
		void ctx_printf(script::VMContext& ctx, const char *fmt, Ts... ts)
		{
			auto& dbg = ctx.get_debug_info();
			printf("[%s:%s:%d] ", dbg.file.c_str(), dbg.function.c_str(), dbg.line);
			printf(fmt, ts...);
		}
		int randomint(script::VMContext& ctx)
		{
			int i = ctx.get_int(0);
			if (i == 0)
			{
				ctx_printf(ctx, "RandomInt parm: 0");
				ctx.add_int(0);
			}
			else
			{
				ctx.add_int(rand() % i);
			}
			//ctx.add_int(0);
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
			ctx.add_float((float)rand() / (float)0xffff);
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
		int dir(script::VMContext& ctx)
		{
			auto o = ctx.get_object(0);
			auto list = std::make_shared<vm::Object>("dir");
			size_t i = 0;
			for (auto& [key, value] : o->fields())
			{
				vm::Variant v = value;
				list->set_field(std::to_string(i++), v);
			}
			ctx.add_object(list);
			return 1;
		}
		int dump(script::VMContext& ctx)
		{
			auto o = ctx.get_object(0);
			auto kvp = ctx.get_object_keys_and_values(o);
			for (auto& it : kvp)
			{
				printf("%s = %s\n", it.first.c_str(), ctx.variant_to_string(it.second).c_str());
			}
			return 0;
		}
		int getarraykeys(script::VMContext& ctx)
		{
			auto o = ctx.get_object(0);
			auto kvp = ctx.get_object_keys_and_values(o);

			auto result = std::make_shared<vm::Object>("getarraykeys");
			size_t n = 0;
			for(auto & it : kvp)
			{
				vm::Variant key = it.first;
				result->set_field(std::to_string(n++), key);
			}
			ctx.add_object(result);
			return 1;
		}
		int tolower(script::VMContext& ctx)
		{
			ctx.add_string(util::string::to_lower(ctx.get_string(0)));
			return 1;
		}
		int is_defined(script::VMContext& ctx)
		{
			auto v = ctx.get_variant(0);
			ctx.add_int(v.index() == (int)vm::Type::kUndefined ? 0 : 1);
			return 1;
		}
		int typeof_(script::VMContext& ctx)
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
			auto& dbg = ctx.get_debug_info();
			printf("[%s:%d] ", dbg.file.c_str(), dbg.line);
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
		int sqrt(script::VMContext& ctx)
		{
			ctx.add_float(sqrtf(ctx.get_float(0)));
			return 1;
		}
		int abs(script::VMContext& ctx)
		{
			ctx.add_float(fabs(ctx.get_float(0)));
			return 1;
		}
		int cos(script::VMContext& ctx)
		{
			ctx.add_float(cosf(ctx.get_float(0)));
			return 1;
		}
		int sin(script::VMContext& ctx)
		{
			ctx.add_float(sinf(ctx.get_float(0)));
			return 1;
		}
		int pow(script::VMContext& ctx)
		{
			ctx.add_float(powf(ctx.get_float(0), ctx.get_float(1)));
			return 1;
		}
		int pi(script::VMContext& ctx)
		{
			ctx.add_float(3.1415927f);
			return 1;
		}
		int getguid(script::VMContext& ctx)
		{
			ctx.add_int(0);
			return 1;
		}
		int float_conv(script::VMContext& ctx)
		{
			ctx.add_float(ctx.get_float(0));
			return 1;
		}
		int getdifficulty(script::VMContext& ctx)
		{
			ctx.add_string("medium");
			return 1;
		}
		int scr_assertex(script::VMContext& ctx)
		{
			int i = ctx.get_int(0);
			auto msg = ctx.get_string(1);
			if (i == 0)
			{
				ctx_printf(ctx, "Assertion failed! %s", msg.c_str());
				getchar();
			}
			return 0;
		}
		int scr_assert(script::VMContext& ctx)
		{
			int i = ctx.get_int(0);
			if (i == 0)
			{
				ctx_printf(ctx, "Assertion failed!");
				getchar();
			}
			return 0;
		}
		int set_exp_fog(script::VMContext& ctx)
		{
			ctx_printf(ctx, "float: %f\n", ctx.get_float(0));
			ctx_printf(ctx, "float: %f\n", ctx.get_float(1));
			ctx_printf(ctx, "float: %f\n", ctx.get_float(2));
			ctx_printf(ctx, "float: %f\n", ctx.get_float(3));
			ctx_printf(ctx, "float: %f\n", ctx.get_float(4));
			return 0;
		}

		std::unordered_map<std::string, StockFunction> stockfunctions = {
			{"getarraykeys", getarraykeys},
			{"setexpfog", set_exp_fog},
			{"isdefined", is_defined},
			{"typeof", typeof_},
			{"loadfx", loadfx},
			{"positionwouldtelefrag", positionwouldtelefrag},
			{"tolower", tolower},
			{"distance", distance},
			{"pi", pi},
			{"cos", cos},
			{"sin", sin},
			{"pow", pow},
			{"abs", abs},
			{"dump", dump},
			{"sqrt", sqrt},
			{"float", float_conv},
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
			{"assert", scr_assert},
			{"setnormalhealth", unimplemented},
			{"getdifficulty", getdifficulty},
			{"precachemodel", unimplemented},
			{"precacheshader", unimplemented},
			{"assertex", scr_assertex},
			{"precacheturret", unimplemented},
			{"precachevehicle", unimplemented},
			{"logprint", print},
			{"dir", dir},
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
