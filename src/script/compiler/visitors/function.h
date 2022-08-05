#pragma once
#include <script/ast/recursive_visitor.h>
#include <set>
#include <string>
#include <unordered_map>

namespace script
{
	namespace compiler
	{
		template<typename T>
		class FunctionVisitor : public ast::RecursiveASTVisitor
		{
		  public:
			struct FunctionInfo
			{
				std::string name;
				size_t numparameters = 0;
				ast::FunctionDeclaration* node;
			};

		  private:
			std::unordered_map<std::string, FunctionInfo> m_function_info;

		  public:
			std::unordered_map<std::string, FunctionInfo>& function_info()
			{
				return m_function_info;
			}

			FunctionInfo *get_function_info(const std::string fn)
			{
				auto fnd = m_function_info.find(fn);
				if (fnd != m_function_info.end())
					return &fnd->second;
				return nullptr;
			}

			virtual bool pre_visit(ast::FunctionDeclaration& n) override
			{
				auto fnd = m_function_info.find(n.function_name);
				if (fnd != m_function_info.end())
					throw T("Function '%s' already defined");
				auto &fi = m_function_info[n.function_name];
				fi.name = n.function_name;
				fi.numparameters = n.parameters.size();
				fi.node = &n;
				return false;
			}
		};
	};
};