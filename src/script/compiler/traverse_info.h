#pragma once
#include <script/compiler/visitors/function_call_reference.h>
#include <script/compiler/visitors/local_variables.h>
#include <script/compiler/visitors/function.h>
namespace script
{
	namespace compiler
	{
		struct TraverseInfo
		{
			TraverseInfo(const std::string filename) : imported_functions_visitor(filename)
			{
			}
			LocalVariablesVisitor local_variables_visitor;
			FunctionCallReferenceVisitor imported_functions_visitor;
		};
	};
}; // namespace script