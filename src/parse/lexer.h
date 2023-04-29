#pragma once
#include "source.h"
#include "token.h"
#include <exception>
#include <string>
#include <vector>

namespace parse
{
	struct lexer_opts
	{
		bool tokenize_comments = false;
		bool tokenize_whitespace = false;
		bool tokenize_newlines = false;
		bool backslash_comments = false;
	};

	typedef struct
	{
		const char* chars;
		token_type type;
	} seqmap_t;

	static const seqmap_t sequence_map[] = {{"==", token_type::eq},
											{"!=", token_type::neq},
											{">=", token_type::geq},
											{"<=", token_type::leq},
											{"<<", token_type::lsht},
											{">>", token_type::rsht},
											{"+=", token_type::plus_assign},
											{"-=", token_type::minus_assign},
											{"*=", token_type::multiply_assign},
											{"/=", token_type::divide_assign},
											{"%=", token_type::mod_assign},
											{"^=", token_type::xor_assign},
											{"|=", token_type::or_assign},
											{"&=", token_type::and_assign},
											{"++", token_type::plus_plus},
											{"--", token_type::minus_minus},
											{"::", token_type::double_colon},
											{"&&", token_type::and_and},
											{"||", token_type::or_or},
											{"/#", token_type::slash_pound},
											{"#/", token_type::pound_slash},
											{"##", token_type::pound_pound}
	};

	struct lexer_error : std::exception
	{
		int start, end;
		int line1, line2;
		const char* message_;
		lexer_error(const char* message, int s, int e, int x, int y)
			: start(s), end(e), line1(x), line2(y), message_(message)
		{
		}

		const char* what() const noexcept override
		{
			return message_;
		}
	};

	class lexer
	{
		const source* m_source;
		size_t m_cursor, m_bufsz;
		int m_lineno;
		int space = 0;

		lexer_opts m_opts;

	  public:

		void seek(size_t pos)
		{
			m_cursor = pos;
		}

		void skip(int n)
		{
			m_cursor += n;
		}

		void advance()
		{
			++m_cursor;
		}
		
		int read_character(size_t *wb = nullptr)
		{
			if (wb)
				*wb = m_cursor;
			return (*m_source)[m_cursor];
		}

		int peek_next_character(int distance = 0, size_t *would_be_position = nullptr)
		{
			if (would_be_position)
				*would_be_position = m_cursor + (distance + 1);
			return (*m_source)[m_cursor + (distance + 1)];
		}

		lexer(const source* src) : m_cursor(0), m_source(src), m_bufsz(src->length()), m_lineno(-1)
		{
		}
		lexer(const source* src, lexer_opts opts)
			: m_opts(opts), m_cursor(0), m_source(src), m_bufsz(src->length()), m_lineno(-1)
		{
		}

		virtual ~lexer()
		{
		}

		bool is_space(int ch) const
		{
			if (m_opts.tokenize_newlines && ch == '\n')
				return false;
			return ch == ' ' || ch == '\t' || ch == '\r';
		}

		bool is_digit(int ch) const
		{
			return ch >= '0' && ch <= '9';
		}

		token hex()
		{
			m_cursor += 2; // read "0x"
			int start = m_cursor;
			int ch = read_character();

			while (is_digit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
			{
				++m_cursor;
				ch = read_character();
			}
			return token(m_source, token_type::hexadecimal, start, m_cursor - start, m_lineno, space);
		}

		token number()
		{
			int start = m_cursor;
			int ch = read_character();
			int is_integer = true;
			int prev = -1;
			while (1)
			{
				if (ch == 'E' || ch == 'e')
				{
					int n = peek_next_character();
					if (n == '-' || n == '+')
					{
						int n2 = peek_next_character(1);
						if (!is_digit(n2))
						{
							// should throw exception...
							break;
						}
						else
						{
							++m_cursor;
						}
					} else if (!is_digit(n))
					{
						//should throw exception...
						break;
					}
					//could still be a integer though...
					is_integer = false;
				}
				else
				{
					if (!is_digit(ch) && ch != '.')
						break;
				}
				if (ch == '.')
					is_integer = false;
				++m_cursor;
				prev = ch;
				ch = read_character();
			}
			return token(m_source, is_integer ? token_type::integer : token_type::number, start, m_cursor - start,
						 m_lineno, space);
		}

		virtual bool is_identifier_preamble_character(int ch)
		{
			return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
		}

		virtual bool is_identifier_character(int ch)
		{
			return is_identifier_preamble_character(ch) || isdigit(ch); //allow digits after the first character
		}

		token identifier()
		{
			int start = m_cursor;
			while (is_identifier_character(read_character()) || is_digit(read_character()))
				++m_cursor;
			return token(m_source, token_type::identifier, start, m_cursor - start, m_lineno, space);
		}

		token string(int quote, token_type tt)
		{
			++m_cursor; //"
			int start = m_cursor;
			int ch;
			bool escaped = false;
			while (1)
			{
				rep:
				ch = read_character();
					if (ch == -1)
						break;
					//throw lexer_error("unexpected eof, this shouldn't happen halfway through a string.", start,
					//				  m_cursor, m_source->line_number(start), m_source->line_number(m_cursor));
				if(ch == '\\')
				{
					escaped = true;
					++m_cursor;
					goto rep;
				}
				if (!escaped)
				{
					if (ch == quote)
						break;
				}
				escaped = false;
				++m_cursor;
			}
			auto t = token(m_source, tt, start, m_cursor - start, m_lineno, space);
			++m_cursor;
			return t;
		}

		token read_token()
		{
		repeat:
			int c;
			space = 0;
			do
			{
				c = read_character();
				if (c == '\n')
					++m_lineno;
				if (!is_space(c))
					break;
				++space;
				++m_cursor;
			} while (is_space(c));
			//while (is_space(read_character()))
				//++m_cursor;
			int ch = read_character();
			if (ch == -1)
				return token(m_source, token_type::eof, space);
			if (is_identifier_preamble_character(ch))
				return identifier();
			if (ch == '0' && peek_next_character() == 'x')
				return hex();
			if (is_digit(ch) || (ch == '.' && is_digit(peek_next_character())))
				return number();
			if (ch == '"')
				return string(ch, token_type::string);
			if (ch == '\'')
				return string(ch, token_type::literal);
			if (m_opts.backslash_comments && ch == '\\')
			{
				if (peek_next_character() == '\\')
				{
					m_cursor += 2;
					// read till next line
					int start = m_cursor;
					do
					{
						ch = read_character();
						if (ch == -1)
						{
							if (!m_opts.tokenize_comments)
								goto repeat;
							return token(m_source, token_type::comment, start, m_cursor - start, m_lineno, space);
						}
						++m_cursor;
					} while (ch != '\n');
					--m_cursor; // incase we read the \n as token
					if (!m_opts.tokenize_comments)
						goto repeat;
					return token(m_source, token_type::comment, start, m_cursor - start, m_lineno, space);
				}
			}
			if (ch == '/')
			{
				int next_peek = peek_next_character();
				if (next_peek == '*' || next_peek == '#') //TODO: FIXME when developer_script is enabled don't comment this out
				{
					m_cursor += 2; // read /*
					int start = m_cursor;
					while (1)
					{
						ch = read_character();
						int ch2 = peek_next_character();
						if (ch == next_peek && ch2 == '/')
							break;
						if (ch == -1)
						{
							if (!m_opts.tokenize_comments)
								goto repeat;
							return token(m_source, token_type::comment, start, m_cursor - start, m_lineno, space);
						}
						++m_cursor;
					}
					m_cursor += 2;
					if (!m_opts.tokenize_comments)
						goto repeat;
					return token(m_source, token_type::comment, start, m_cursor - start, m_lineno, space);
				}
				else if (peek_next_character() == '/')
				{
					m_cursor += 2;
					// read till next line
					int start = m_cursor;
					do
					{
						ch = read_character();
						if (ch == -1)
						{
							if (!m_opts.tokenize_comments)
								goto repeat;
							return token(m_source, token_type::comment, start, m_cursor - start, m_lineno, space);
						}
						++m_cursor;
					} while (ch != '\n');
					--m_cursor; // incase we read the \n as token
					if (!m_opts.tokenize_comments)
						goto repeat;
					return token(m_source, token_type::comment, start, m_cursor - start, m_lineno, space);
				}
			}
			for (int i = 0; i < sizeof(sequence_map) / sizeof(sequence_map[0]); ++i)
			{
				if (sequence_map[i].chars[0] != ch)
					continue;
				// check second char if match
				++m_cursor;
				int ch2 = read_character();
				if (sequence_map[i].chars[1] != ch2)
				{
					--m_cursor;
					continue;
				}
				++m_cursor;
				return token(m_source, sequence_map[i].type, m_cursor - 2, 2, m_lineno, space);
			}
			return token(m_source, ch, m_cursor++, 1, m_lineno, space);
		}

		std::vector<token> tokenize()
		{
			std::vector<token> tokens;
			while (1)
			{
				auto t = read_token();
				if (t.type == token_type::eof)
					break;
				tokens.push_back(t);
			}
			return tokens;
		}
	};
}; // namespace parse