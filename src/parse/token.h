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
		hexadecimal,
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

	enum
	{
		// 0 through 255 reserved for ascii
		TokenType_kString = 256,
		TokenType_kIdentifier,
		TokenType_kLiteral,
		TokenType_kInteger,
		TokenType_kHexadecimal,
		TokenType_kNumber,
		TokenType_kComment,
		TokenType_kEq,
		TokenType_kNeq,
		TokenType_kLeq,
		TokenType_kGeq,
		TokenType_kLsht,
		TokenType_kRsht,
		TokenType_kEof,
		TokenType_kInvalid
	};

	static const char* token_type_strings[] = {"string",  "identifier", "literal", "integer", "hexadecimal", "number",
											   "comment", "==",			"!=",	   "<=",	  ">=",
											   "<<",	  ">>",			"eof",	   "invalid", NULL};

	struct token
	{
		//either stringvalue is set and it has no reference to any source anymore or the source is set and the string can be extracted from there
		std::string m_stringvalue;
		const source* m_source;
		token_type type;
		int pos, sz;
		int real_line_number;

		token() : pos(-1), sz(0), type(token_type::invalid), m_source(NULL), real_line_number(-1)
		{
		}
		token(token_type t) : type(t), m_source(NULL), pos(-1), sz(0), real_line_number(-1)
		{
		}
		token(int t) : type((token_type)t), m_source(NULL), pos(-1), sz(0), real_line_number(-1)
		{
		}
		token(const source* src, token_type t) : m_source(src), type(t), pos(-1), sz(0), real_line_number(-1)
		{
		}
		template <typename T>
		token(const source* src, T t, int _pos, int _sz, int ln)
			: m_source(src), type((token_type)t), pos(_pos), sz(_sz), real_line_number(ln)
		{
		}

		token copy() const
		{
			token t;
			t.m_stringvalue = m_source ? to_string() : m_stringvalue;
			t.pos = -1;
			t.sz = -1;
			t.type = type;
			t.real_line_number = -1;
			t.m_source = nullptr;
			return t;
		}

		std::string source_file() const
		{
			if (!m_source)
				return "memory buffer";
			return m_source->path();
		}

		const int line_number() const
		{
			return real_line_number;
			#if 0
			if (!m_source)
				return -1;
			return m_source->line_number(pos);
			#endif
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
			if(m_stringvalue.empty())
				return m_source->extract_string(pos, sz);
			return m_stringvalue;
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