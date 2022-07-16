#pragma once
#include <parse/lexer.h>
#include <parse/token_parser.h>
#include <unordered_map>

//not sure how common this is, but eh most the values are either in this format or can be changed a bit with flags e.g vector needing , to seperate units
namespace parse
{
	struct KvpParser
	{
		std::string string;
		KvpParser(const std::string& str) : string(str)
		{
		}

		template <typename T> void parse_impl(parse::token_parser&, T&);
		template <typename T> bool parse(T& v)
		{
			parse::source src("", string);
			parse::lexer lexer(&src);
			try
			{
				auto tokens = lexer.tokenize();
				parse::token_parser parser(tokens);
				this->parse_impl(parser, v);
				return true;
			}
			catch (std::exception& err)
			{
			}
			return false;
		}
	};

	template<typename T> struct KvpParserMap
	{
		T* inst;
		const std::unordered_map<std::string, std::string>& map;
		KvpParserMap(const std::unordered_map<std::string, std::string>& map_, T* inst_) : map(map_), inst(inst_)
		{
		}
		// might be very specific, but eh..
		// could use ptr and offsetof, but now the member gets initialized
		template <typename M>
		void parse(M(T::*ptm), const std::string& key)
		{
			auto fnd = map.find(key);
			if (fnd == map.end())
				return;
			auto& mv = (inst->*ptm);
			KvpParser parser(fnd->second);
			parser.parse(mv);
		}
	};
};