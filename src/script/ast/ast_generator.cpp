#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <parse/preprocessor.h>
#include <core/filesystem/api.h>

#include "ast_generator.h"

namespace compiler
{
	ASTGenerator::ASTGenerator()
	{
	}
	ASTGenerator::~ASTGenerator()
	{
	}

	bool ASTGenerator::accept(int token_type)
	{
		return m_token_parser->accept_token(token, token_type);
	}

	void ASTGenerator::expect(int token_type)
	{
		if (!accept(token_type))
		{
			parse::token tok(token_type);
			throw ASTException("Expected token type '{}', got '{}' instead.", tok.type_as_string(), token.type_as_string());
		}
	}

	std::unique_ptr<ast::Identifier> ASTGenerator::identifier()
	{
		expect(parse::TokenType_kIdentifier);
		return std::move(node<ast::Identifier>(token.to_string()));
	}

	std::unique_ptr<ast::Expression> ASTGenerator::factor_unary_expression()
	{
		throw ASTException("factor unary expression unhandled");
		return nullptr;
	}
	std::unique_ptr<ast::Expression> ASTGenerator::factor_identifier()
	{
		throw ASTException("factor identifier unhandled");
		return nullptr;
	}
	std::unique_ptr<ast::Expression> ASTGenerator::factor_parentheses()
	{
		throw ASTException("factor parentheses unhandled");
		return nullptr;
	}
	std::unique_ptr<ast::Expression> ASTGenerator::factor_integer()
	{
		auto n = node<ast::Literal>();
		n->type = ast::Literal::Type::kInteger;
		n->value = token.to_string();
		return n;
	}
	std::unique_ptr<ast::Expression> ASTGenerator::factor_number()
	{
		auto n = node<ast::Literal>();
		n->type = ast::Literal::Type::kNumber;
		n->value = token.to_string();
		return n;
	}
	std::unique_ptr<ast::Expression> ASTGenerator::factor_string()
	{
		auto n = node<ast::Literal>();
		n->type = ast::Literal::Type::kString;
		n->value = token.to_string();
		return n;
	}

	std::unique_ptr<ast::Expression> ASTGenerator::factor()
	{
		token = m_token_parser->read_token();
		using FactorFunction = std::function<std::unique_ptr<ast::Expression>(ASTGenerator&)>;
		std::unordered_map<int, FactorFunction> factors = {
			{parse::TokenType_kIdentifier, &ASTGenerator::factor_identifier},
			{'(', &ASTGenerator::factor_parentheses},
			{parse::TokenType_kInteger, &ASTGenerator::factor_integer},
			{parse::TokenType_kNumber, &ASTGenerator::factor_number},
			{parse::TokenType_kString, &ASTGenerator::factor_string},
			{'-', &ASTGenerator::factor_unary_expression},
			{'!', &ASTGenerator::factor_unary_expression},
			{'~', &ASTGenerator::factor_unary_expression},
			{'&', &ASTGenerator::factor_unary_expression}
		};
		auto fnd = factors.find(token.type_as_int());
		if (fnd == factors.end())
			throw ASTException("Invalid factor '{}'", token.type_as_string());
		return fnd->second(*this);
	}

	std::unique_ptr<ast::Expression> ASTGenerator::expression()
	{
		return factor();
	}

	std::unique_ptr<ast::CallExpression> ASTGenerator::call_expression()
	{
		auto call = node<ast::CallExpression>();
		call->callee = identifier();

		if (accept(':') && accept(':'))
		{
			auto c = node<ast::CallExternalExpression>();
			c->file_reference = std::move(call->callee);
			c->callee = identifier();
			call = std::move(c);
		}
		expect('(');
		while (1)
		{
			if (accept(')'))
				return call;
			call->arguments.push_back(expression());
			if (!accept(','))
				break;
		}
		expect(')');
		return call;
	}

	std::unique_ptr<ast::Statement> ASTGenerator::statement()
	{
		//read the token type, then based off of that choose which statement is being processed
		auto expr_stmt = node<ast::ExpressionStatement>();
		expr_stmt->expression = call_expression();
		expect(';');
		//std::move implicitly applied to local return values i guess
		return expr_stmt;
	}

	std::unique_ptr<ast::BlockStatement> ASTGenerator::block_statement()
	{
		auto block = node<ast::BlockStatement>();
		while (1)
		{
			if (accept('}'))
				break;
			block->body.push_back(statement());
		}
	}

	void ASTGenerator::function_declaration(ast::Program &program)
	{
		auto decl = node<ast::FunctionDeclaration>();
		decl->id = identifier();
		expect('(');
		while (1)
		{
			if (accept(')'))
				goto skip_rparen;
			decl->parameters.push_back(identifier());
			if (!accept(','))
				break;
		}
		expect(')');
		skip_rparen:
		expect('{');
		decl->body = block_statement();
		//kinda ugly, but for now it'll do, i dont wanna add add_node methods on every node struct
		program.body.push_back(std::move(decl));
	}

	void ASTGenerator::program()
	{
		while (1)
		{
			if (accept(parse::TokenType_kEof))
				break;
			printf("token type = %s %s\n", token.to_string().c_str(), token.type_as_string().c_str());
			function_declaration(tree);
		}
	}

	bool ASTGenerator::generate(filesystem_api& fs, const std::string base_path, const std::string path)
	{
		try
		{
			parse::source_map sources;
			parse::definition_map definitions;
			parse::preprocessor proc;
			parse::lexer_opts opts;

			class custom_lexer : public parse::lexer
			{
			  public:
				custom_lexer(const parse::source* src) : lexer(src)
				{
				}
				custom_lexer(const parse::source* src, parse::lexer_opts opts) : lexer(src, opts)
				{
				}

				virtual bool is_identifier_character(int ch) override
				{
					return parse::lexer::is_identifier_character(ch) || ch == '\\';
				}
			};

			opts.backslash_comments = true;
			if (proc.preprocess_with_typed_lexer<custom_lexer>(fs, base_path, path, tokens, sources, definitions, opts))
			{
				m_token_parser = std::make_unique<parse::token_parser>(tokens);
				program();
			}
		}
		catch (std::exception& e)
		{
			//throw ASTException("something went wrong generating syntax tree {}", e.what());
			throw ASTException("{}", e.what());
		}
	}
}; // namespace script