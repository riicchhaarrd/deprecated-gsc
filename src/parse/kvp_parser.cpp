#include "kvp_parser.h"
#include <math/math.h>
#include <graphics/color.h>

namespace parse
{
	template <> void KvpParser::parse_impl(parse::token_parser& parser, std::string& s)
	{
		s = parser.read_string();
	}
	template <> void KvpParser::parse_impl(parse::token_parser& parser, int& v)
	{
		v = parser.read_integer();
	}
	template <> void KvpParser::parse_impl(parse::token_parser& parser, float& v)
	{
		v = parser.read_number();
	}
	template <> void KvpParser::parse_impl(parse::token_parser& parser, glm::vec3& v)
	{
		v.x = parser.read_number();
		v.y = parser.read_number();
		v.z = parser.read_number();
	}
	template <> void KvpParser::parse_impl(parse::token_parser& parser, graphics::color& v)
	{
		v.x = parser.read_number();
		v.y = parser.read_number();
		v.z = parser.read_number();
		v.w = parser.read_number();
	}
};