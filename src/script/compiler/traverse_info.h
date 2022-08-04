#pragma once
#include <script/compiler/visitors/imported_functions.h>
#include <script/compiler/visitors/local_variables.h>
#include <script/compiler/visitors/function.h>
namespace script
{
	namespace compiler
	{
		struct TraverseInfo
		{
			LocalVariablesVisitor local_variables_visitor;
			ImportedFunctionsVisitor imported_functions_visitor;
			FunctionVisitor function_visitor;
		};
	};
}; // namespace script