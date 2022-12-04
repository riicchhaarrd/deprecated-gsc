#pragma once
#include <script/ast/recursive_visitor.h>
#include <script/compiler/exception.h>
#include <script/ast/type_visitor.h>

namespace script
{
	namespace compiler
	{
		class LocalVariablesVisitor : public ast::RecursiveASTVisitor
		{
			std::unordered_map<std::string, size_t> m_functions;

		  public:
			std::unordered_map<std::string, size_t>& functions()
			{
				return m_functions;
			}

			bool is_local_identifier(const std::string n)
			{
				return n != "level" && n != "game" && n != "self";
			}

			bool get_variable_name(ast::Node* n, std::string& s)
			{
				//auto* ptr = dynamic_cast<ast::Identifier*>(n);
				//auto* ptr2 = dynamic_cast<ast::MemberExpression*>(n);
				auto* ptr = n->cast<ast::Identifier>();
				auto* ptr2 = n->cast<ast::MemberExpression>();
				if (ptr)
				{
					if (!ptr->file_reference.empty())
						throw CompileException("file_reference is not empty for variable, unsupported");
					s = ptr->name;
					return true;
				}
				else if (ptr2)
				{
					return get_variable_name(ptr2->object.get(), s);
				}
				throw CompileException("invalid node type");
				return false;
			}

			virtual bool pre_visit(ast::FunctionDeclaration& n) override
			{
				// gather how many local variables we have and reserve the space
				{
					ast::NodeTypeVisitor<ast::AssignmentExpression> nf;
					auto& assignments = nf.find(n.body.get());
					std::set<std::string> localvars;
					for (auto& assignment : assignments)
					{
						std::string varname;
						get_variable_name(assignment->lhs.get(), varname);
						// printf("varname=%s %d\n", varname.c_str(), is_local_identifier(varname));
						if (is_local_identifier(varname))
							localvars.insert(varname);
					}
					// printf("%d assignments, %d localvars\n", assignments.size(), localvars.size());
					m_functions[n.function_name] = localvars.size();
				}
				return false;
			}
		};
	};
};