#pragma once
#include <common/exception.h>

namespace script
{
	namespace compiler
	{
		struct CompileData
		{
		};
		using CompileException = common::TypedDataMessageException<CompileData>;
	};
};