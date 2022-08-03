#pragma once
#include <script/ast/recursive_visitor.h>

namespace script
{
	namespace compiler
	{
		class ImportedFunctionsVisitor : public ast::RecursiveASTVisitor
		{
			std::set<std::string> m_imports;

		  public:
			std::set<std::string>& imports()
			{
				return m_imports;
			}

			virtual bool pre_visit(ast::Identifier& n) override
			{
				if (!n.file_reference.empty())
				{
					std::string ref = n.file_reference;
					std::replace(ref.begin(), ref.end(), '\\', '/');
					m_imports.insert(ref);
				}
				return false;
			}
		};
	};
};