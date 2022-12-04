#pragma once
#include <parse/token.h>
#include "ostream_indent.h"
#include <script/ast/visitor.h>
#include <script/ast/nodes.h>
#include <exception>

namespace script
{
	class GSCWriter : public ast::ASTVisitor
	{
		ostream_indent os;

	  public:
		GSCWriter(std::ostream& out) : os(out)
		{
		}
		virtual void visit(ast::FunctionDeclaration& n)
		{
#if 0
			//gather all variables used
			FindNodeVisitor find(*n.body.get(),
								 [](ast::Node& n) -> bool
								 {
									 //auto* ptr = dynamic_cast<ast::AssignmentExpression*>(&n);
									 if (!ptr)
										 return false;
									 return true;
								 });
			os << "/*" << std::endl << "variables set in this function" << std::endl;
			auto assignments = find.results();
			for (auto& assignment : assignments)
			{
				//auto* ptr = dynamic_cast<ast::AssignmentExpression*>(assignment);
				//auto* identifier = dynamic_cast<ast::Identifier*>(ptr->lhs.get());
				if (!identifier)
					continue;
				identifier->accept(*this);
				os << ", ";
			}
			os << "*/" << std::endl;
#endif

			os << "" << n.function_name << "(";
			size_t i = 0;
			for (auto& parm : n.parameters)
			{
				os << parm;
				//parm->accept(*this);
				if (i != n.parameters.size() - 1)
					os << ", ";
				++i;
			}
			os << ")" << std::endl;
			n.body->accept(*this);
		}

		// Inherited via ASTVisitor
		virtual void visit(ast::Program& n) override
		{
			for (auto& it : n.body)
				it->accept(*this);
		}
		virtual void visit(ast::BlockStatement& n) override
		{
			os >> "{" << std::endl;
			os.indent(1);
			for (auto& stmt : n.body)
				stmt->accept(*this);
			os.indent(-1);
			os >> "}" << std::endl;
		}

		void visit_statement(ast::StatementPtr& ptr)
		{
			//if (dynamic_cast<ast::BlockStatement*>(ptr.get()))
			if (ptr->cast<ast::BlockStatement>())
			{
				ptr->accept(*this);
				return;
			}
			os.indent(1);
			ptr->accept(*this);
			os.indent(-1);
		}

		virtual void visit(ast::IfStatement& n) override
		{
			os >> "if(";
			n.test->accept(*this);
			os << ")" << std::endl;
			visit_statement(n.consequent);
			if (n.alternative)
			{
				os >> "else" << std::endl;
				visit_statement(n.alternative);
			}
		}
		virtual void visit(ast::WhileStatement& n) override
		{
			os >> "while(";
			n.test->accept(*this);
			os << ")" << std::endl;
			visit_statement(n.body);
		}
		virtual void visit(ast::ForStatement& n) override
		{
			os >> "for(";
			if (n.init)
				n.init->accept(*this);

			if (n.test)
				os << "; ";
			else
				os << ";";

			if (n.test)
				n.test->accept(*this);

			if (n.update)
				os << "; ";
			else
				os << ";";

			if (n.update)
				n.update->accept(*this);
			os << ")" << std::endl;
			visit_statement(n.body);
		}
		virtual void visit(ast::DoWhileStatement& n) override
		{
			os >> "do" << std::endl;
			visit_statement(n.body);
			os >> "while(";
			n.test->accept(*this);
			os << ");" << std::endl;
		}
		virtual void visit(ast::ReturnStatement& n) override
		{
			if (!n.argument)
			{
				os >> "return;" << std::endl;
				return;
			}
			os >> "return ";
			n.argument->accept(*this);
			os << ";" << std::endl;
		}
		virtual void visit(ast::BreakStatement& n) override
		{
			os >> "break;" << std::endl;
		}
		virtual void visit(ast::WaitStatement& n) override
		{
			os >> "wait ";
			n.duration->accept(*this);
			os << ";" << std::endl;
		}
		virtual void visit(ast::WaitTillFrameEndStatement& n) override
		{
			os >> "waittillframeend;" << std::endl;
		}
		virtual void visit(ast::ExpressionStatement& n) override
		{
			os >> "";
			n.expression->accept(*this);
			os << ";" << std::endl;
		}
		virtual void visit(ast::EmptyStatement& n) override
		{
			os >> ";";
		}
		virtual void visit(ast::LocalizedString& n) override
		{
			os << "&"
			   << "\"" << n.reference << "\"";
		}
		virtual void visit(ast::Literal& n) override
		{
			if (n.type == ast::Literal::Type::kString)
				os << "\"" << n.value << "\"";
			else if (n.type == ast::Literal::Type::kUndefined)
				os << "undefined";
			else if (n.type == ast::Literal::Type::kAnimation)
				os << "%" << n.value;
			else
				os << n.value;
		}
		virtual void visit(ast::Identifier& n) override
		{
			if (n.file_reference.empty())
				os << n.name;
			else
				os << n.file_reference << "::" << n.name;
		}
		virtual void visit(ast::FunctionPointer& n) override
		{
			os << "::";
			//n.identifier->accept(*this);
			os << n.function_name;
		}
		virtual void visit(ast::BinaryExpression& n) override
		{
			n.left->accept(*this);
			os << " ";
			switch (n.op)
			{
			case parse::TokenType_kAndAnd:
				os << "&&";
				break;
			case parse::TokenType_kOrOr:
				os << "||";
				break;
			case parse::TokenType_kEq:
				os << "==";
				break;
			case parse::TokenType_kNeq:
				os << "!=";
				break;
			case parse::TokenType_kGeq:
				os << ">=";
				break;
			case parse::TokenType_kLeq:
				os << "<=";
				break;
			case parse::TokenType_kRsht:
				os << ">>";
				break;
			case parse::TokenType_kLsht:
				os << "<<";
				break;
			default:
				if (n.op >= 32 && n.op < 127)
					os << (char)n.op;
				else
					throw std::runtime_error("unhandled operator");
					// throw std::exception("unhandled operator {}", n.op);
				break;
			}
			os << " ";
			n.right->accept(*this);
		}
		virtual void visit(ast::AssignmentExpression& n) override
		{
			n.lhs->accept(*this);
			os << " ";
			switch (n.op)
			{
			case '=':
				os << "=";
				break;
			case parse::TokenType_kPlusAssign:
				os << "+=";
				break;
			case parse::TokenType_kMinusAssign:
				os << "-=";
				break;
			case parse::TokenType_kDivideAssign:
				os << "/=";
				break;
			case parse::TokenType_kMultiplyAssign:
				os << "*=";
				break;
			case parse::TokenType_kAndAssign:
				os << "&=";
				break;
			case parse::TokenType_kOrAssign:
				os << "|=";
				break;
			case parse::TokenType_kXorAssign:
				os << "^=";
				break;
			case parse::TokenType_kModAssign:
				os << "%=";
				break;
			default:
				// throw std::exception("unhandled assignment operator {}", n.op);
				throw std::runtime_error("unhandled assignment operator");
				break;
			}
			os << " ";
			n.rhs->accept(*this);
		}
		virtual void visit(ast::CallExpression& n) override
		{
			if (n.object)
			{
				n.object->accept(*this);
				os << " ";
			}
			if (n.threaded)
			{
				os << "thread ";
			}
			if (n.pointer)
				os << "[[";
			n.callee->accept(*this);
			if (n.pointer)
				os << "]]";
			os << "(";
			int i = 0;
			for (auto& arg : n.arguments)
			{
				arg->accept(*this);
				if (i != n.arguments.size() - 1)
					os << ", ";
				++i;
			}
			os << ")";
		}
		virtual void visit(ast::ConditionalExpression& n) override
		{
			n.condition->accept(*this);
			os << " ? ";
			n.consequent->accept(*this);
			os << " : ";
			n.alternative->accept(*this);
		}
		virtual void visit(ast::MemberExpression& n) override
		{
			n.object->accept(*this);
			if (n.op == '.')
				os << ".";
			else
				os << "[";
			n.prop->accept(*this);
			if (n.op == '[')
				os << "]";
		}
		virtual void visit(ast::UnaryExpression& n) override
		{
			if (!n.prefix)
				n.argument->accept(*this);
			if (n.op == parse::TokenType_kPlusPlus)
				os << "++";
			else if (n.op == parse::TokenType_kMinusMinus)
				os << "--";
			else
				os << (char)n.op;
			if (n.prefix)
				n.argument->accept(*this);
		}
		virtual void visit(ast::VectorExpression& n) override
		{
			os << "(";
			int i = 0;
			for (auto& el : n.elements)
			{
				el->accept(*this);
				if (n.elements.size() - 1 != i)
					os << ", ";
				++i;
			}
			os << ")";
		}
		virtual void visit(ast::ArrayExpression& n) override
		{
			os << "[";
			int i = 0;
			for (auto& el : n.elements)
			{
				el->accept(*this);
				if (n.elements.size() - 1 != i)
					os << ", ";
				++i;
			}
			os << "]";
		}

		// Inherited via ASTVisitor
		virtual void visit(ast::SwitchCase&) override;
		virtual void visit(ast::ContinueStatement&) override;
		virtual void visit(ast::SwitchStatement&) override;
		virtual void visit(ast::Directive&) override;
	};
}; // namespace compiler
