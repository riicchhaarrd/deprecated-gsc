#pragma once
#include <script/ast/recursive_visitor.h>

namespace script
{
	namespace ast
	{
		template <typename T> class NodeTypeVisitor : public RecursiveASTVisitor
		{
			std::vector<T*> m_instances;
			using CondFunc = std::function<bool(T&)>;
			CondFunc m_cond;
			bool single = false;

		  public:
			virtual bool pre_visit(T& n) override
			{
				if (!m_cond || m_cond(n))
				{
					m_instances.push_back(&n);
					if (single)
						return false;
				}
				return true;
			}
			std::vector<T*>& find(ast::Node* root, CondFunc cond = nullptr)
			{
				single = false;
				m_instances.clear();
				m_cond = cond;
				root->accept(*this);
				return m_instances;
			}
			T* find_single(ast::Node* root, CondFunc cond = nullptr)
			{
				single = true;
				m_instances.clear();
				m_cond = cond;
				root->accept(*this);
				if (m_instances.empty())
					return nullptr;
				return m_instances[0];
			}
		};
		
		using NodeVisitor = NodeTypeVisitor<ast::Node>;
	}; // namespace ast
}; // namespace compiler