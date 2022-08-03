#pragma once
#include "recursive_visitor.h"

namespace script
{
	namespace ast
	{
		class NodeTraverser
		{
			std::vector<RecursiveASTVisitor*> m_visitors;

		  public:
			NodeTraverser()
			{
			}

			void add_visitor(RecursiveASTVisitor* visitor)
			{
				m_visitors.push_back(visitor);
			}

			// TODO: FIXME we don't really "support" returning a new set of nodes
			void traverse(ast::Node& n)
			{
				for (auto* visitor : m_visitors)
				{
					visitor->visit(n);
				}
			}
		};
	}; // namespace ast
}; // namespace compiler