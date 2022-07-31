#pragma once

namespace compiler
{
	class FindNodeVisitor : public RecursiveASTVisitor
	{
		ast::Node& node;
		std::vector<ast::Node*> found;
		std::function<bool(ast::Node&)> check_node;

	  public:
		FindNodeVisitor(ast::Node& n, std::function<bool(ast::Node&)> fn) : node(n), check_node(fn)
		{
		}

		std::vector<ast::Node*>& results()
		{
			node.accept(*this);
			return found;
		}

		virtual bool pre_visit(ast::Program& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::FunctionDeclaration& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::BlockStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::IfStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::WhileStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::ForStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::DoWhileStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::ReturnStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::BreakStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::WaitStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::WaitTillFrameEndStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::ExpressionStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::EmptyStatement& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::LocalizedString& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::Literal& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::Identifier& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::FunctionPointer& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::BinaryExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::AssignmentExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::CallExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::ConditionalExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::MemberExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::UnaryExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::VectorExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
		virtual bool pre_visit(ast::ArrayExpression& n)
		{
			if (check_node(n))
				found.push_back(&n);
			return true;
		}
	};
};