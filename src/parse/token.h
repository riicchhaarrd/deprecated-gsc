#pragma once
#include <string>
#include "source.h"

namespace parse
{
	enum class token_type
	{
		// 0 through 255 reserved for ascii
		string = 256,
		identifier,
		literal,
		integer,
		number,
		comment,
		eq,
		neq,
		leq,
		geq,
		lsht,
		rsht,
		eof,
		invalid
	};

	static const char* token_type_strings[] = {"string",  "identifier", "literal", "integer", "number",
											   "comment", "==",			"!=",	   "<=",	  ">=",
											   "<<",	  ">>",			"eof",	   "invalid", NULL};

	struct token
	{
		const source* m_source;
		token_type type;
		int pos, sz;

		token() : pos(-1), sz(0), type(token_type::invalid), m_source(NULL)
		{
		}
		token(token_type t) : type(t), m_source(NULL), pos(-1), sz(0)
		{
		}
		token(const source* src, token_type t) : m_source(src), type(t), pos(-1), sz(0)
		{
		}
		template <typename T>
		token(const source* src, T t, int _pos, int _sz) : m_source(src), type((token_type)t), pos(_pos), sz(_sz)
		{
		}

		const int line_number() const
		{
			if (!m_source)
				return -1;
			return m_source->line_number(pos);
		}

		std::string type_as_string() const
		{
			std::string type_str;
			if (type_as_int() < 255)
			{
				type_str.push_back(type_as_int() & 0xff);
				return type_str;
			}
			return token_type_strings[type_as_int() - (int)token_type::string];
		}

		std::string to_string() const
		{
			return m_source->extract_string(pos, sz);
		}

		int to_int() const
		{
			return std::stoi(to_string());
		}

		int type_as_int() const
		{
			return (int)type;
		}
	};
}; // namespace parse