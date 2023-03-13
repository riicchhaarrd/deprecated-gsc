#pragma once
#include <functional>
#include <variant>
#include <parse/token_parser.h>
#include <common/overload.h>

namespace parse
{
	using Expr = std::variant<std::string, float, int>;
	
	static std::string expression_type(const Expr& expr)
	{
		auto _ = common::Overload
		{
			[](std::string) { return "string"; },
			[](int) { return "int"; },
			[](float) { return "float"; }
		};
		return std::visit(_, expr);
	}

	static float expression_number(const Expr &expr)
	{
		auto _ = common::Overload
		{
			[](std::string) -> float
			{
				throw parse_error("not a number");
			},
			[](int i) -> float
			{
				return (float)i;
			},
			[](float f) -> float
			{
				return f;
			}
		};
		return std::visit(_, expr);
	}

	static std::string expression_string(const Expr& expr)
	{
		auto _ = common::Overload
		{
			[](std::string s) -> std::string
			{
				return s;
			},
			[](int i) -> std::string
			{
				return std::to_string(i);
			},
			[](float f) -> std::string
			{
				return std::to_string(f);
			}
		};
		return std::visit(_, expr);
	}

	struct FuncArgs
	{
		token_parser& parser;
		const std::string &function_name;
		std::vector<Expr> args;
		Expr& operator[](const size_t i)
		{
			if (i >= args.size())
				throw parse_error(common::format("index {} out of bounds in function {}", i, function_name));
			return args[i];
		}
	};
	using Func = std::function<Expr(FuncArgs&)>;
	using FindFuncFn = std::function<Func(const std::string&)>;

	namespace op
	{
		struct Add
		{
			template <typename T, typename U> Expr operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_same_v<T, std::string> || std::is_same_v<U, std::string>)
				{
					return expression_string(lhs) + expression_string(rhs);
				}
				else
				{
					return lhs + rhs;
				}
			}
		};
		
		struct Zero
		{
			template <typename T>
			Expr operator()(const T& operand) const
			{
				if constexpr (std::is_arithmetic_v<T>)
				{
					return T{}*0;
				}
				else
				{
					throw parse_error("Not arithmetic type");
				}
			}
		};

		struct Negative
		{
			template <typename T>
			Expr operator()(const T& operand) const
			{
				if constexpr (std::is_arithmetic_v<T>)
				{
					return -operand;
				}
				else
				{
					throw parse_error("Not arithmetic type");
				}
			}
		};

		struct Sub
		{
			template <typename T, typename U> Expr operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs - rhs;
				}
				else
				{
					throw parse_error("Operator '+' not applicable to given types");
				}
			}
		};

		struct Mul
		{
			template <typename T, typename U> Expr operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs * rhs;
				}
				else
				{
					throw parse_error("Operator '*' not applicable to given types");
				}
			}
		};

		struct Leq
		{
			template <typename T, typename U>
			bool operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs <= rhs;
				}
				else
				{
					throw parse_error("Operator '<=' not applicable to given types");
				}
			}
		};
		struct Geq
		{
			template <typename T, typename U>
			bool operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs >= rhs;
				}
				else
				{
					throw parse_error("Operator '>=' not applicable to given types");
				}
			}
		};
		struct Eq
		{
			template <typename T, typename U>
			bool operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs == rhs;
				}
				else
				{
					throw parse_error("Operator '==' not applicable to given types");
				}
			}
		};
		struct Neq
		{
			template <typename T, typename U>
			bool operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs != rhs;
				}
				else
				{
					throw parse_error("Operator '!=' not applicable to given types");
				}
			}
		};
		struct Less
		{
			template <typename T, typename U>
			bool operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs < rhs;
				}
				else
				{
					throw parse_error("Operator '<' not applicable to given types");
				}
			}
		};
		struct Greater
		{
			template <typename T, typename U>
			bool operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					return lhs > rhs;
				}
				else
				{
					throw parse_error("Operator '>' not applicable to given types");
				}
			}
		};

		struct Div
		{
			template <typename T, typename U> Expr operator()(const T& lhs, const U& rhs) const
			{
				if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
				{
					if (rhs == 0)
					{
						throw parse_error("Division by zero");
					}
					return lhs / rhs;
				}
				else
				{
					throw parse_error("Operator '/' not applicable to given types");
				}
			}
		};
	}; // namespace op

	class ExpressionParser
	{
	  private:
		token_parser& parser;
		
		Expr expression()
		{
			return relational();
		}

		Expr relational()
		{
			Expr result = add_and_subtract();
			while (1)
			{
				token t = parser.read_token();
				int type = t.type_as_int();
				
				if (type != '>' && type != '<' && type != parse::TokenType_kEq && type != parse::TokenType_kLeq && 
					   type != parse::TokenType_kNeq && type != parse::TokenType_kGeq)
				{
					parser.unread_token();
					break;
				}
				Expr operand = add_and_subtract();
				bool b;
				switch (type)
				{
				case '>':
					b = std::visit(op::Geq{}, result, operand);
					break;
				case '<':
					b = std::visit(op::Less{}, result, operand);
					break;
				case parse::TokenType_kGeq:
					b = std::visit(op::Geq{}, result, operand);
					break;
				case parse::TokenType_kLeq:
					b = std::visit(op::Leq{}, result, operand);
					break;
				case parse::TokenType_kEq:
					b = std::visit(op::Eq{}, result, operand);
					break;
				case parse::TokenType_kNeq:
					b = std::visit(op::Neq{}, result, operand);
					break;
				}
				result = b ? 1 : 0;
			}
			return result;
		}

		Expr add_and_subtract()
		{
			Expr result = term();
			while (1)
			{
				token t = parser.read_token();
				int type = t.type_as_int();
				if (type != '-' && type != '+')
				{
					parser.unread_token();
					break;
				}
				Expr operand = term();
				switch (type)
				{
				case '-':
					result = std::visit(op::Sub{}, result, operand);
					break;
				case '+':
					result = std::visit(op::Add{}, result, operand);
					break;
				}
			}
			return result;
		}

		Expr term()
		{
			Expr result = factor();
			while (1)
			{
				token t = parser.read_token();
				int type = t.type_as_int();
				if (type != '*' && type != '/' && type != '%')
				{
					parser.unread_token();
					break;
				}
				Expr operand = factor();
				switch (type)
				{
				case '*':
					result = std::visit(op::Mul{}, result, operand);
					break;
				case '/':
					result = std::visit(op::Div{}, result, operand);
					break;
				case '%':
					result = fmod(parse::expression_number(result), parse::expression_number(operand));
					break;
				}
			}
			return result;
		}

		Expr factor()
		{
			Expr result{};
			token t = parser.read_token();
			if (t.type_as_int() == '(')
			{
				result = expression();
				parser.expect_token(')');
			}
			else if (t.type == token_type::integer)
			{
				result = t.to_int();
			} else if(t.type == token_type::number)
			{
				result = t.to_float();
			}
			else if (t.type_as_int() == '-')
			{
				// TODO: FIXME
				//uh... -(1) should work, did I do this correctly in other implementations?
				result = factor();
				result = std::visit(op::Negative{}, result);
			}
			else if (t.type == token_type::string)
			{
				result = t.to_string();
			}
			else if (t.type == token_type::identifier)
			{
				parser.expect_token('(');
				std::string function_name = t.to_string();
				FuncArgs func_args{.parser = parser, .function_name = function_name};
				do
				{
					t = parser.read_token();
					if (t.type == token_type::eof)
						throw parse_error("unexpected eof");
					if (t.type_as_int() == ')')
						goto skip_paren;
					parser.unread_token();
					Expr arg = expression();
					func_args.args.push_back(arg);
				} while (parser.accept_token(t, ','));
				parser.expect_token(')');
				skip_paren:
				Func f;
				if(function_find_fn)
					f = function_find_fn(function_name);
				if (!f)
					throw parse_error(common::format("can't find function {}", function_name), &t);
				result = f(func_args);
			}
			else
			{
				throw parse_error(common::format("unexpected token type {}", t.type_as_string()), &t);
			}
			return result;
		}
		FindFuncFn function_find_fn;

	  public:
		ExpressionParser(token_parser& parser_, FindFuncFn fn = {}) : parser(parser_), function_find_fn(fn)
		{
		}

		Expr evaluate()
		{
			//TODO: check if expression was fully parsed, otherwise it's a "invalid expression"
			return expression();
		}
	};
}; // namespace parse