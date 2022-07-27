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

	std::unique_ptr<ast::CallExpression> ASTGenerator::call_expression()
	{
		expect(parse::TokenType_kIdentifier);
		return nullptr;
	}

	std::unique_ptr<ast::Statement> ASTGenerator::statement()
	{
		auto expr_stmt = node<ast::ExpressionStatement>();
		expr_stmt->expression = call_expression();
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
			opts.backslash_comments = true;
			if (proc.preprocess(fs, base_path, path, tokens, sources, definitions, opts))
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