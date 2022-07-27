#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <format>
#include <core/filesystem/api.h>
#include <parse/token_parser.h>
#include <script/ast/node/statement.h>

#include <script/ast/node/expression/assignment_expression.h>
#include <script/ast/node/expression/binary_expression.h>
#include <script/ast/node/statement/block_statement.h>
#include <script/ast/node/statement/break_statement.h>
#include <script/ast/node/expression/call_expression.h>
#include <script/ast/node/expression/conditional_expression.h>
#include <script/ast/node/statement/do_while_statement.h>
#include <script/ast/node/statement/expression_statement.h>
#include <script/ast/node/statement/for_statement.h>
#include <script/ast/node/function_declaration.h>
#include <script/ast/node/expression/identifier.h>
#include <script/ast/node/statement/if_statement.h>
#include <script/ast/node/expression/literal.h>
#include <script/ast/node/expression/member_expression.h>
#include <script/ast/node/program.h>
#include <script/ast/node/statement/return_statement.h>
#include <script/ast/node/expression/unary_expression.h>
#include <script/ast/node/statement/while_statement.h>
#include <script/ast/node/expression/call_external_expression.h>
#include <script/ast/node/expression.h>

namespace compiler
{
	struct ASTException : std::exception
	{
		std::string message_;
		template <typename... Ts> ASTException(std::string_view fmt, Ts&&... ts)
		{
			message_ = std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...));
		}
		const char* what() const noexcept override
		{
			return message_.c_str();
		}
	};

	class ASTGenerator
	{
		parse::token_list tokens;
		parse::token token;
		std::unique_ptr<parse::token_parser> m_token_parser;
		void program();
		ast::Program tree;

		template<typename T, typename... Ts>
		std::unique_ptr<T> node(Ts... ts)
		{
			return std::move(std::make_unique<T>(ts...));
		}
		std::unique_ptr<ast::Identifier> identifier();
		std::unique_ptr<ast::BlockStatement> block_statement();
		void function_declaration(ast::Program&);
		bool accept(int token_type);
		void expect(int token_type);
		std::unique_ptr<ast::Statement> statement();
		std::unique_ptr<ast::CallExpression> call_expression();
		std::unique_ptr<ast::Expression> expression();
		std::unique_ptr<ast::Expression> factor_integer();
		std::unique_ptr<ast::Expression> factor_number();
		std::unique_ptr<ast::Expression> factor_string();
		std::unique_ptr<ast::Expression> factor_unary_expression();
		std::unique_ptr<ast::Expression> factor_identifier();
		std::unique_ptr<ast::Expression> factor_parentheses();
		std::unique_ptr<ast::Expression> factor();
	  public:
		ASTGenerator();
		~ASTGenerator();
		bool generate(filesystem_api& fs, const std::string base_path, const std::string path);
	};
}; // namespace script