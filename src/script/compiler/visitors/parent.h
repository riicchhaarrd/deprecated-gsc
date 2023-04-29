#pragma once
#include <script/ast/recursive_visitor.h>
#include <set>
#include <string>
#include <stack>
#include <vector>
#include <unordered_map>

namespace script
{
	namespace compiler
	{
		class ParentVisitor : public ast::RecursiveASTVisitor
		{
			std::unordered_map<ast::Node*, ast::Node*> m_parent_map;
			ast::Node* m_parent = nullptr;
		  public:

			ast::Node* get_parent(ast::Node* n)
			{
				if (m_parent_map.find(n) == m_parent_map.end())
					return nullptr;
				return m_parent_map[n];
			}
			template<typename T>
			T* get_parent_by_type(ast::Node* n, std::vector<ast::Node*>& visited_list)
			{
				auto* current = n;
				
				do
				{
					auto* parent = get_parent(current);
					if (!parent)
						break;
					//auto* dc = dynamic_cast<T*>(parent);
					auto* dc = parent->cast<T>();
					if (dc)
						return dc;
					visited_list.push_back(parent);
					current = parent;
				} while (current != nullptr);
				return nullptr;
			}
			
			template<typename T>
			T* get_parent_by_type(ast::Node* n)
			{
				auto* current = n;
				
				do
				{
					auto* parent = get_parent(current);
					if (!parent)
						break;
					//auto* dc = dynamic_cast<T*>(parent);
					auto* dc = parent->cast<T>();
					if (dc)
						return dc;
					current = parent;
				} while (current != nullptr);
				return nullptr;
			}

			bool visited(ast::Node& n)
			{
				m_parent_map[&n] = m_parent;
				m_parent = &n;
				return true;
			}

			virtual bool pre_visit(ast::SwitchCase& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::Directive& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::SwitchStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::ContinueStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::Program& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::FunctionDeclaration& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::BlockStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::IfStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::WhileStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::ForStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::DoWhileStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::ReturnStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::BreakStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::WaitStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::WaitTillFrameEndStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::ExpressionStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::EmptyStatement& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::LocalizedString& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::Literal& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::Identifier& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::FunctionPointer& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::BinaryExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::AssignmentExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::CallExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::ConditionalExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::MemberExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::UnaryExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::VectorExpression& n)
			{
				return visited(n);
			}
			virtual bool pre_visit(ast::ArrayExpression& n)
			{
				return visited(n);
			}
		};
	};
};