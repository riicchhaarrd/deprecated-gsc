#pragma once
#include <vector>
#include <string>
#include "token.h"
#include <format>
#include <sstream>
#include <common/logger.h>

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
		std::string message_;
		explicit parse_error(const std::string& message, token* t = nullptr)
		{
			if (t)
				message_ = std::format("[{}:{}] {}", t->source_file(), t->line_number(), message);
			else
				message_ = message;
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

		int m_tokenindex_saved = -1;

	  public:
		token_parser(const token_list& t) : m_tokens(t), m_tokenindex(0)
		{
		}
		token_parser(const token_list& t, parse_opts opts) : m_tokens(t), m_tokenindex(0), m_opts(opts)
		{
		}

		void save()
		{
			if (m_tokenindex_saved != -1)
				LOG_ERROR("can't save parser");
			m_tokenindex_saved = m_tokenindex;
		}

		void restore()
		{
			m_tokenindex = m_tokenindex_saved;
			m_tokenindex_saved = -1;
		}

		void unread_token()
		{
			if (m_tokenindex > 0)
				--m_tokenindex;
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

		bool eof() const
		{
			return m_tokenindex + 1 > m_tokens.size();
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

		template <typename T> void expect_token(T token_type)
		{
			parse::token t;
			if (!accept_token(t, token_type))
			{
				token tok(token_type);
				throw parse_error(
					std::format("expected token type {}, got {} instead", tok.type_as_string(), t.type_as_string()),
					&t);
			}
		}

		template <typename T> void expect_token(parse::token& t, T token_type)
		{
			if (!accept_token(t, token_type))
			{
				token tok(token_type);
				throw parse_error(
					std::format("expected token type {}, got {} instead", tok.type_as_string(), t.type_as_string()),
					&t);
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
			if (t.type == parse::token_type::hexadecimal)
			{
				int i;
				std::stringstream ss;
				ss << std::hex << t.to_string();
				ss >> i;
				return neg ? -i : i;
			}
			else if (t.type == parse::token_type::integer)
			{
				return neg ? -std::stoi(t.to_string()) : std::stoi(t.to_string());
			}
			throw parse_error("expected integer", &t);
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
				throw parse_error("expected number", &t);
			return neg ? -std::stof(t.to_string()) : std::stof(t.to_string());
		}

		std::string read_string()
		{
			auto t = read_token();
			if (t.type != parse::token_type::string)
				throw parse_error("expected string", &t);
			return t.to_string();
		}

		std::string read_identifier()
		{
			auto t = read_token();
			if (t.type != parse::token_type::identifier)
				throw parse_error("expected identifier got " + t.type_as_string() + ", " + t.to_string(), &t);
			return t.to_string();
		}

		std::string read_identifier_or_string()
		{
			auto t = read_token();
			if (t.type != parse::token_type::identifier && t.type != parse::token_type::string)
				throw parse_error("expected identifier or string got " + t.type_as_string() + ", " + t.to_string(), &t);
			return t.to_string();
		}
	};
} // namespace parse;