#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <common/format.h>
#include <core/filesystem/api.h>
#include <parse/token_parser.h>
#include "nodes.h"

namespace script
{
	namespace ast
	{
		struct ASTException : std::exception
		{
			std::string message_;
			template <typename... Ts> ASTException(std::string_view fmt, Ts&&... ts)
			{
				message_ = common::format(fmt, ts...);
				//message_ = std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...));
			}
			const char* what() const noexcept override
			{
				return message_.c_str();
			}
		};

		class ASTGenerator
		{
			std::string using_animtree_value;
			parse::token_list tokens;
			parse::token token;
			std::unique_ptr<parse::token_parser> m_token_parser;
			void program();
			std::unique_ptr<Program> tree;

			DebugInfo debug;

			template <typename T, typename... Ts> std::unique_ptr<T> node(Ts... ts)
			{
				// printf("node(%s)\n", typeid(T).name());
				auto n = std::move(std::make_unique<T>(ts...));
				n->debug = debug;
				return n;
			}
			std::unique_ptr<Identifier> identifier();
			std::unique_ptr<BlockStatement> block_statement();
			void function_declaration(Program&);
			bool accept(int token_type);
			void expect(int token_type);
			std::unique_ptr<Statement> statement();
			std::unique_ptr<CallExpression> call_expression(std::unique_ptr<Expression>, bool threaded = false);
			ExpressionPtr expression();
			ExpressionPtr factor_integer();
			ExpressionPtr factor_number();
			ExpressionPtr factor_string();
			ExpressionPtr factor_unary_expression();
			ExpressionPtr factor_identifier();
			ExpressionPtr factor_parentheses();
			ExpressionPtr factor_pound();
			ExpressionPtr factor_percent_symbol();
			void factor(ExpressionPtr&);
			std::unique_ptr<CallExpression> function_pointer_call(bool threaded = false);
			std::unique_ptr<CallExpression> regular_function_pointer_call();
			void assignment_expression(ExpressionPtr& expr);
			bool accept_assignment_operator();
			void ternary_expression(ExpressionPtr& expr);
			void bitwise_or(ExpressionPtr& expr);
			void bitwise_xor(ExpressionPtr& expr);
			void bitwise_and(ExpressionPtr& expr);
			void relational(ExpressionPtr& expr);
			void bitwise_shift(ExpressionPtr& expr);
			void add_and_subtract(ExpressionPtr& expr);
			void member_expression(ExpressionPtr& expr);
			void term(ExpressionPtr& expr);
			void postfix(ExpressionPtr& expr);
			ExpressionPtr binary_expression(int, ExpressionPtr&, ExpressionPtr&);
			std::unique_ptr<AssignmentExpression> assignment_node(int op, ExpressionPtr& lhs);
			StatementPtr if_statement();
			StatementPtr return_statement();
			StatementPtr for_statement();
			StatementPtr while_statement();
			StatementPtr break_statement();
			StatementPtr continue_statement();
			StatementPtr do_while_statement();
			StatementPtr wait_statement();
			StatementPtr empty_statement();
			StatementPtr switch_statement();
			std::unique_ptr<Directive> directive();
			StatementPtr waittillframeend_statement();
			bool accept_identifier_string(const std::string string);
			ExpressionPtr factor_array_expression();
			ExpressionPtr factor_function_pointer();
			ExpressionPtr factor_localized_string();
			void expect_identifier_string(const std::string string);
			parse::token peek();
			void logical_and(ExpressionPtr& expr);
			void logical_or(ExpressionPtr& expr);
			bool accept_token_string(const std::string str);

		  public:
			std::unique_ptr<Program>& root()
			{
				return tree;
			}
			ASTGenerator();
			~ASTGenerator();
			bool generate(filesystem_api& fs, const std::string base_path, const std::string path);
		};
	}; // namespace ast
}; // namespace script
