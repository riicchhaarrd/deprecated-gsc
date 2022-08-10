#pragma once
#include <script/ast/recursive_visitor.h>
#include <set>
#include <string>
#include <algorithm>
#include <common/stringutil.h>

namespace script
{
	namespace compiler
	{
		class FunctionCallReferenceVisitor : public ast::RecursiveASTVisitor
		{
			using References = std::unordered_map<std::string, std::set<std::string>>;
			References m_references;
			std::string m_file_name;
		  public:
			FunctionCallReferenceVisitor(const std::string file_name) : m_file_name(file_name)
			{
			}
			References& references()
			{
				return m_references;
			}
			virtual bool pre_visit(ast::FunctionPointer& n) override
			{
				std::string ref = m_file_name;
				std::replace(ref.begin(), ref.end(), '\\', '/');
				m_references[util::string::to_lower(ref)].insert(util::string::to_lower(n.function_name));
				return false;
			}
			virtual bool pre_visit(ast::CallExpression& n) override
			{
				auto* id = dynamic_cast<ast::Identifier*>(n.callee.get());
				if (!id)
					return true;
				std::string ref = m_file_name;
				if (!id->file_reference.empty())
					ref = id->file_reference;
				std::replace(ref.begin(), ref.end(), '\\', '/');
				m_references[util::string::to_lower(ref)].insert(util::string::to_lower(id->name));
				return true;
			}
			#if 0
			virtual bool pre_visit(ast::Identifier& n) override
			{
				if (n.file_reference.empty())
				{
					return false;
				}
				std::string ref = n.file_reference;
				std::replace(ref.begin(), ref.end(), '\\', '/');
				m_references[ref].insert(n.name);
				return false;
			}
			#endif
		};
	};
};