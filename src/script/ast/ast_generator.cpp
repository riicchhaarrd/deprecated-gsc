#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <parse/preprocessor.h>
#include <core/filesystem/api.h>

#include "ast_generator.h"
#include "printer.h"

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
#include <script/ast/node/expression.h>
#include <script/ast/node/expression/array_expression.h>
#include <script/ast/node/expression/vector_expression.h>
#include <script/ast/node/expression/function_pointer.h>
#include <script/ast/node/expression/localized_string.h>
#include <script/ast/node/statement/wait_statement.h>
#include <script/ast/node/statement/wait_till_frame_end_statement.h>
#include <script/ast/node/statement/empty_statement.h>

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
			m_token_parser->dump();
			//__debugbreak();
			throw ASTException("Expected token type '{}' {}, got '{}' {} instead. {}:{}", tok.type_as_string(),
							   tok.to_string(), token.type_as_string(), token.to_string(), token.source_file(),
							   token.line_number());
		}
	}

	std::unique_ptr<ast::Identifier> ASTGenerator::identifier()
	{
		expect(parse::TokenType_kIdentifier);
		std::string ident = token.to_string();
		if (accept(parse::TokenType_kDoubleColon))
		{
			expect(parse::TokenType_kIdentifier);
			std::string file_reference = ident;
			ident = token.to_string();
			return node<ast::Identifier>(ident, file_reference);
		}
		return node<ast::Identifier>(ident);
	}

	ExpressionPtr ASTGenerator::factor_unary_expression()
	{
		token = m_token_parser->read_token();
		int op = token.type_as_int();
		auto n = node<ast::UnaryExpression>();
		n->op = op;
		n->argument = expression();
		n->prefix = true;
		return n;
	}

	parse::token ASTGenerator::peek()
	{
		m_token_parser->save();
		parse::token t = m_token_parser->read_token();
		m_token_parser->restore();
		return t;
	}

	ExpressionPtr ASTGenerator::factor_identifier()
	{
		auto ident = identifier();
		if (accept('('))
		{
			return call_expression(std::move(ident));
		}
		return ident;
	}
	ExpressionPtr ASTGenerator::factor_parentheses()
	{
		expect('(');
		ExpressionPtr a = expression();

		//TODO: check whether it's only int/float and handle vector as a literal
		if (accept(','))
		{
			auto n = node<ast::VectorExpression>();
			n->elements.push_back(std::move(a));
			n->elements.push_back(expression());
			expect(',');
			n->elements.push_back(expression());
			expect(')');
			return n;
		}
		//otherwise just a general expression captured within parentheses
		expect(')');
		return a;
	}
	ExpressionPtr ASTGenerator::factor_localized_string()
	{
		expect('&');
		auto n = node<ast::LocalizedString>();
		expect(parse::TokenType_kString);
		n->reference = token.to_string();
		return n;
	}
	ExpressionPtr ASTGenerator::factor_function_pointer()
	{
		expect(parse::TokenType_kDoubleColon);
		auto n = node<ast::FunctionPointer>();
		n->identifier = identifier();
		return n;
	}
	std::unique_ptr<ast::CallExpression> ASTGenerator::function_pointer_call()
	{
		expect(parse::TokenType_kDoubleBracketLeft);
		// function pointer call
		auto ident = expression();
		expect(parse::TokenType_kDoubleBracketRight);
		return call_expression(std::move(ident));
	}
	ExpressionPtr ASTGenerator::factor_array_expression()
	{
		expect('[');

		auto n = node<ast::ArrayExpression>();
		while (1)
		{
			if (accept(']'))
				return n;
			n->elements.push_back(expression());
			if (!accept(','))
				break;
		}
		expect(']');
		return n;
	}
	ExpressionPtr ASTGenerator::factor_integer()
	{
		expect(parse::TokenType_kInteger);
		auto n = node<ast::Literal>();
		n->type = ast::Literal::Type::kInteger;
		n->value = token.to_string();
		return n;
	}
	ExpressionPtr ASTGenerator::factor_number()
	{
		expect(parse::TokenType_kNumber);
		auto n = node<ast::Literal>();
		n->type = ast::Literal::Type::kNumber;
		n->value = token.to_string();
		return n;
	}
	ExpressionPtr ASTGenerator::factor_string()
	{
		expect(parse::TokenType_kString);
		auto n = node<ast::Literal>();
		n->type = ast::Literal::Type::kString;
		n->value = token.to_string();
		return n;
	}

	void ASTGenerator::factor(ExpressionPtr& expr)
	{
		m_token_parser->save();
		token = m_token_parser->read_token();
		m_token_parser->restore();

		using FactorFunction = std::function<ExpressionPtr(ASTGenerator&)>;
		std::unordered_map<int, FactorFunction> factors = {
			{parse::TokenType_kIdentifier, &ASTGenerator::factor_identifier},
			{'(', &ASTGenerator::factor_parentheses},
			{parse::TokenType_kInteger, &ASTGenerator::factor_integer},
			{parse::TokenType_kNumber, &ASTGenerator::factor_number},
			{parse::TokenType_kString, &ASTGenerator::factor_string},
			{'-', &ASTGenerator::factor_unary_expression},
			{'!', &ASTGenerator::factor_unary_expression},
			{'~', &ASTGenerator::factor_unary_expression},
			{'&', &ASTGenerator::factor_localized_string},
			{'[', &ASTGenerator::factor_array_expression},
			{parse::TokenType_kDoubleBracketLeft, &ASTGenerator::function_pointer_call},
			{parse::TokenType_kDoubleColon, &ASTGenerator::factor_function_pointer}
		};
		auto fnd = factors.find(token.type_as_int());
		if (fnd == factors.end())
			throw ASTException("Invalid factor '{}'", token.type_as_string());
		expr = std::move(fnd->second(*this));
	}

	bool ASTGenerator::accept_assignment_operator()
	{
		static int operators[] = {'=',
								  parse::TokenType_kPlusAssign,
								  parse::TokenType_kMinusAssign,
								  parse::TokenType_kDivideAssign,
								  parse::TokenType_kMultiplyAssign,
								  parse::TokenType_kAndAssign,
								  parse::TokenType_kOrAssign,
								  parse::TokenType_kXorAssign,
								  parse::TokenType_kModAssign};
		for (size_t i = 0; i < COUNT_OF(operators); ++i)
		{
			if (accept(operators[i]))
				return true;
		}
		return false;
	}
	ExpressionPtr ASTGenerator::binary_expression(int op, ExpressionPtr& lhs, ExpressionPtr& rhs)
	{
		auto n = node<ast::BinaryExpression>();
		n->left = std::move(lhs);
		n->right = std::move(rhs);
		n->op = op;
		return n;
	}

	void ASTGenerator::postfix(ExpressionPtr& expr)
	{
		factor(expr);
		while (accept(parse::TokenType_kPlusPlus) || accept(parse::TokenType_kMinusMinus))
		{
			int op = token.type_as_int();
			auto n = node<ast::UnaryExpression>();
			n->op = op;
			n->prefix = false;
			n->argument = std::move(expr);
			expr = std::move(n);
		}
	}

	void ASTGenerator::member_expression(ExpressionPtr& expr)
	{
		postfix(expr);
		while (accept('[') || accept('.'))
		{
			int op = token.type_as_int();
			ExpressionPtr rhs;
			postfix(rhs);
			if (op == '[')
				expect(']');
			auto n = node<ast::MemberExpression>();
			n->object = std::move(expr);
			n->prop = std::move(rhs);
			expr = std::move(n);
		}
	}

	void ASTGenerator::method_call_expression(ExpressionPtr& expr)
	{
		member_expression(expr);
		bool threaded = accept_identifier_string("thread");
		auto t = peek();
		if (t.type_as_int() == parse::TokenType_kDoubleBracketLeft)
		{
			auto n = function_pointer_call();
			n->threaded = threaded;
			n->object = std::move(expr);
			expr = std::move(n);
		}
		else if (t.type_as_int() == parse::TokenType_kIdentifier)
		{
			auto ident = identifier();
			expect('(');
			auto n = call_expression(std::move(ident));
			n->threaded = threaded;
			n->object = std::move(expr);
			expr = std::move(n);
		}
	}
	
	void ASTGenerator::term(ExpressionPtr& expr)
	{
		method_call_expression(expr);
		while (accept('/') || accept('*') || accept('%'))
		{
			int op = token.type_as_int();
			ExpressionPtr rhs;
			method_call_expression(rhs);
			expr = binary_expression(op, expr, rhs);
		}
	}

	void ASTGenerator::add_and_subtract(ExpressionPtr& expr)
	{
		term(expr);
		while (accept('+') || accept('-'))
		{
			int op = token.type_as_int();
			ExpressionPtr rhs;
			term(rhs);
			expr = binary_expression(op, expr, rhs);
		}
	}

	void ASTGenerator::bitwise_shift(ExpressionPtr& expr)
	{
		add_and_subtract(expr);
		while (accept(parse::TokenType_kLsht) || accept(parse::TokenType_kRsht))
		{
			int op = token.type_as_int();
			ExpressionPtr rhs;
			add_and_subtract(rhs);
			expr = binary_expression(op, expr, rhs);
		}
	}

	void ASTGenerator::relational(ExpressionPtr& expr)
	{
		bitwise_shift(expr);
		while (accept('>') || accept('<') || accept(parse::TokenType_kEq) || accept(parse::TokenType_kLeq) ||
			   accept(parse::TokenType_kNeq) || accept(parse::TokenType_kGeq))
		{
			int op = token.type_as_int();
			ExpressionPtr rhs;
			bitwise_shift(rhs);
			expr = binary_expression(op, expr, rhs);
		}
	}

	void ASTGenerator::bitwise_and(ExpressionPtr& expr)
	{
		relational(expr);
		while (accept('&'))
		{
			ExpressionPtr rhs;
			relational(rhs);
			expr = binary_expression('&', expr, rhs);
		}
	}

	void ASTGenerator::bitwise_xor(ExpressionPtr& expr)
	{
		bitwise_and(expr);
		while (accept('^'))
		{
			ExpressionPtr rhs;
			bitwise_and(rhs);
			expr = binary_expression('^', expr, rhs);
		}
	}

	void ASTGenerator::bitwise_or(ExpressionPtr& expr)
	{
		bitwise_xor(expr);
		while (accept('|'))
		{
			ExpressionPtr rhs;
			bitwise_xor(rhs);
			expr = binary_expression('|', expr, rhs);
		}
	}

	void ASTGenerator::logical_and(ExpressionPtr& expr)
	{
		bitwise_or(expr);
		while (accept(parse::TokenType_kAndAnd))
		{
			ExpressionPtr rhs;
			bitwise_or(rhs);
			expr = binary_expression(parse::TokenType_kAndAnd, expr, rhs);
		}
	}

	void ASTGenerator::logical_or(ExpressionPtr& expr)
	{
		logical_and(expr);
		while (accept(parse::TokenType_kOrOr))
		{
			ExpressionPtr rhs;
			logical_and(rhs);
			expr = binary_expression(parse::TokenType_kOrOr, expr, rhs);
		}
	}

	void ASTGenerator::ternary_expression(ExpressionPtr& expr)
	{
		logical_or(expr);

		while (accept('?'))
		{
			ExpressionPtr consequent, alternative;
			logical_or(consequent);
			expect(':');
			logical_or(alternative);

			auto ternary = node<ast::ConditionalExpression>();
			ternary->condition = std::move(expr);
			ternary->consequent = std::move(consequent);
			ternary->alternative = std::move(alternative);
			expr = std::move(ternary);
		}
	}

	std::unique_ptr<ast::AssignmentExpression> ASTGenerator::assignment_node(int op, ExpressionPtr &lhs)
	{
		auto n = node<ast::AssignmentExpression>();
		n->op = op;
		n->lhs = std::move(lhs);
		n->rhs = nullptr;
		return n;
	}

	void ASTGenerator::assignment_expression(ExpressionPtr& expr)
	{
		ExpressionPtr lhs;
		
		ternary_expression(lhs);

		if (!accept_assignment_operator())
		{
			expr = std::move(lhs);
			return;
		}
		int op = token.type_as_int();
		expr = assignment_node(op, lhs);
		ExpressionPtr* n = &expr;
		while (1)
		{
			ExpressionPtr rhs;
			ternary_expression(rhs);
			if (!accept_assignment_operator())
			{
				dynamic_cast<ast::AssignmentExpression*>(n->get())->rhs = std::move(rhs);
				break;
			}
			op = token.type_as_int();
			auto* ptr = dynamic_cast<ast::AssignmentExpression*>(n->get());
			ptr->rhs = assignment_node(op, rhs);
			n = &ptr->rhs;
		}
	}

	ExpressionPtr ASTGenerator::expression()
	{
		ExpressionPtr expr;
		assignment_expression(expr);
		if (!expr)
			throw ASTException("should not happen");
		return expr;
	}

	std::unique_ptr<ast::CallExpression> ASTGenerator::call_expression(std::unique_ptr<ast::Expression> ident)
	{
		auto call = node<ast::CallExpression>();
		call->callee = std::move(ident);
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

	bool ASTGenerator::accept_identifier_string(const std::string string)
	{
		if (!accept(parse::TokenType_kIdentifier))
			return false;
		if (token.to_string() != string)
		{
			m_token_parser->unread_token();
			return false;
		}
		return true;
	}

	void ASTGenerator::expect_identifier_string(const std::string string)
	{
		expect(parse::TokenType_kIdentifier);
		if (token.to_string() != string)
			throw ASTException("expected identifier '{}'", string);
	}

	StatementPtr ASTGenerator::for_statement()
	{
		expect_identifier_string("for");
		expect('(');
		//init-statement, but we'll just use expression for now
		auto n = node<ast::ForStatement>();
		if (!accept(';'))
		{
			n->init = expression();
			expect(';');
		}
		if (!accept(';'))
		{
			n->test = expression();
			expect(';');
		}
		if (!accept(')'))
		{
			n->update = expression();
			expect(')');
		}
		n->body = statement();
		return n;
	}
	StatementPtr ASTGenerator::return_statement()
	{
		expect_identifier_string("return");
		auto stmt = node<ast::ReturnStatement>();
		if (!accept(';'))
		{
			stmt->argument = expression();
			expect(';');
		}
		return stmt;
	}
	StatementPtr ASTGenerator::while_statement()
	{
		expect_identifier_string("while");
		expect('(');
		auto n = node<ast::WhileStatement>();
		n->test = expression();
		expect(')');
		n->body = statement();
		return n;
	}
	StatementPtr ASTGenerator::do_while_statement()
	{
		expect_identifier_string("do");
		throw ASTException("unhandled statement do while");
	}
	StatementPtr ASTGenerator::waittillframeend_statement()
	{
		expect_identifier_string("waittillframeend");
		auto n = node<ast::WaitTillFrameEndStatement>();
		expect(';');
		return n;
	}
	StatementPtr ASTGenerator::wait_statement()
	{
		expect_identifier_string("wait");
		auto n = node<ast::WaitStatement>();
		n->duration = expression();
		expect(';');
		return n;
	}
	StatementPtr ASTGenerator::if_statement()
	{
		expect_identifier_string("if");
		expect('(');
		auto stmt = node<ast::IfStatement>();
		stmt->test = expression();
		expect(')');
		stmt->consequent = statement();
		if (accept_identifier_string("else"))
			stmt->alternative = statement();
		return stmt;
	}

	std::unique_ptr<ast::Statement> ASTGenerator::statement()
	{
		if (accept('{'))
			return block_statement();
		m_token_parser->save();
		token = m_token_parser->read_token();
		m_token_parser->restore(); // restore now otherwise we may call statement in a recursive way later again and
								   // bugs will happen.
		if (token.type_as_int() != parse::TokenType_kIdentifier)
		{
			throw ASTException("expected identifier or block statement got '{}' instead", token.type_as_string());
		}
		using StatementFunction = std::function<StatementPtr(ASTGenerator&)>;
		std::unordered_map<std::string, StatementFunction> statements = {
			{"for", &ASTGenerator::for_statement},
			{"while", &ASTGenerator::while_statement},
			{"wait", &ASTGenerator::wait_statement},
			{"waittillframeend", &ASTGenerator::waittillframeend_statement},
			{"do", &ASTGenerator::do_while_statement},
			{"if", &ASTGenerator::if_statement},
			{"return", &ASTGenerator::return_statement}
		};

		// read the token, then based off of that choose which statement is being processed
		auto fnd = statements.find(token.to_string());
		if (fnd != statements.end())
			return fnd->second(*this);

		auto expr_stmt = node<ast::ExpressionStatement>();
		expr_stmt->expression = expression();
		expect(';');
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
		return block;
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
		tree = node<ast::Program>();
		while (1)
		{
			if (accept(parse::TokenType_kEof))
				break;
			function_declaration(*tree.get());
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
				auto it = std::remove_if(tokens.begin(), tokens.end(),
							   [](const parse::token& t) { return t.type_as_int() == '\n'; });
				tokens.erase(it, tokens.end());
				m_token_parser = std::make_unique<parse::token_parser>(tokens);
				program();
				#if 0
				FILE* fp = fopen("F:\\export\\dm.txt", "w");
				BasicPrinter out(fp);
				tree.print(out);
				fclose(fp);
				#endif
			}
		}
		catch (std::exception& e)
		{
			//throw ASTException("something went wrong generating syntax tree {}", e.what());
			throw ASTException("{}", e.what());
		}
	}
}; // namespace script