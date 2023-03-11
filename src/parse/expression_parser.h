#pragma once
#include <parse/token_parser.h>

namespace parse
{
	class ExpressionParser
	{
		token_parser& parser;

		float expression()
		{
			float result = term();
			while (1)
			{
				token t = parser.read_token();
				int type = t.type_as_int();
				if (type != '-' && type != '+')
				{
					parser.unread_token();
					break;
				}
				float operand = term();
				switch (type)
				{
				case '-':
					result -= operand;
					break;
				case '+':
					result += operand;
					break;
				}
			}
			return result;
		}

		float term()
		{
			float result = factor();
			while (1)
			{
				token t = parser.read_token();
				int type = t.type_as_int();
				if (type != '*' && type != '/')
				{
					parser.unread_token();
					break;
				}
				float operand = factor();
				switch (type)
				{
				case '*':
					result *= operand;
					break;
				case '/':
					result /= operand;
					break;
				}
			}
			return result;
		}

		float factor()
		{
			float result = 0.f;
			token t = parser.read_token();
			if (t.type_as_int() == '(')
			{
				result = expression();
				parser.expect_token(')');
			}
			else if (t.type == token_type::integer || t.type == token_type::number)
			{
				result = t.to_float();
			}
			else
			{
				throw parse_error(common::format("unexpected token type {}", t.type_as_string()), &t);
			}
			return result;
		}

	  public:
		ExpressionParser(token_parser& parser_) : parser(parser_)
		{
		}

		float parse()
		{
			return expression();
		}
	};
}; // namespace parse