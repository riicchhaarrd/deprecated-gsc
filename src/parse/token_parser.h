#pragma once
#include <vector>
#include <string>
#include "token.h"

namespace parse
{
	using token_list = std::vector<parse::token>;
	struct parse_opts
	{
		bool newlines = false;
		bool comments = false;
	};

	struct parse_error : std::exception
	{
		int line;
		std::string message_;
		explicit parse_error(const std::string& message, int lineno = -1) : message_(message), line(lineno)
		{
		}
		const char* what() const noexcept override
		{
			return message_.c_str();
		}
	};

	class token_parser
	{
		const token_list& m_tokens;
		int m_tokenindex = 0;
		parse_opts m_opts;

	  public:
		token_parser(const token_list& t) : m_tokens(t), m_tokenindex(0)
		{
		}
		token_parser(const token_list& t, parse_opts opts) : m_tokens(t), m_tokenindex(0), m_opts(opts)
		{
		}

		parse::token read_token()
		{
			if (m_tokenindex + 1 > m_tokens.size())
				return parse::token(parse::token_type::eof);
			auto t = m_tokens[m_tokenindex++];
			if (!m_opts.newlines && t.type_as_int() == '\n')
				return read_token();
			return t;
		}

		void read_tokens_till(std::vector<parse::token>& tokens, const char* tt)
		{
			while (1)
			{
				auto t = read_token();
				if (t.type == parse::token_type::eof)
					break;
				for (int i = 0; tt[i]; ++i)
				{
					if (t.type == (parse::token_type)tt[i])
					{
						--m_tokenindex;
						return;
					}
				}
				tokens.push_back(t);
			}
		}

		template <typename T> void read_tokens_till(std::vector<parse::token>& tokens, T tt)
		{
			while (1)
			{
				auto t = read_token();
				if (t.type == parse::token_type::eof)
					break;
				if (t.type == (parse::token_type)tt)
				{
					--m_tokenindex;
					break;
				}
				tokens.push_back(t);
			}
		}

		template <typename T> void expect_token(T tt)
		{
			parse::token t;
			if (!accept_token(t, tt))
			{
				std::string error_message = "expected token type" + t.type_as_string();
				throw parse_error(error_message, t.line_number());
			}
		}

		template <typename T> void expect_token(parse::token& t, T tt)
		{
			if (!accept_token(t, tt))
			{
				std::string error_message = "expected token type" + t.type_as_string();
				throw parse_error(error_message, t.line_number());
			}
		}

		template <typename T> bool accept_token(parse::token& t, T tt)
		{
			t = read_token();
			if (t.type == (parse::token_type)tt)
				return true;
			--m_tokenindex;
			return false;
		}

		int read_integer()
		{
			auto t = read_token();
			bool neg = false;
			if (t.type_as_int() == '-')
			{
				neg = true;
				t = read_token();
			}
			if (t.type != parse::token_type::integer)
				throw parse_error("expected integer", t.line_number());
			return neg ? -std::stoi(t.to_string()) : std::stoi(t.to_string());
		}

		float read_number()
		{
			auto t = read_token();
			bool neg = false;
			if (t.type_as_int() == '-')
			{
				neg = true;
				t = read_token();
			}
			if (t.type != parse::token_type::number && t.type != parse::token_type::integer)
				throw parse_error("expected number", t.line_number());
			return neg ? -std::stof(t.to_string()) : std::stof(t.to_string());
		}

		std::string read_string()
		{
			auto t = read_token();
			if (t.type != parse::token_type::string)
				throw parse_error("expected string", t.line_number());
			return t.to_string();
		}

		std::string read_identifier()
		{
			auto t = read_token();
			if (t.type != parse::token_type::identifier)
				throw parse_error("expected identifier got " + t.type_as_string() + ", " + t.to_string(), t.line_number());
			return t.to_string();
		}
	};
} // namespace parse;