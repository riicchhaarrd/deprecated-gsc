#pragma once
#include <memory>
#include <string>
#include <variant>
#include <unordered_map>
#include <vector>

namespace script
{
	namespace compiler
	{
		namespace type
		{
			struct Vector
			{
				float x = 0.f;
				float y = 0.f;
				float z = 0.f;
			};
			using String = std::string;
			using Integer = int;
			using Number = float;

			struct Object;
			struct Array;
			using ObjectPtr = std::weak_ptr<Object>;
			using ArrayPtr = std::weak_ptr<Array>;

			struct LocalizedString
			{
				size_t index;
			};

			struct FunctionPointer
			{
				size_t pointer;
			};

			struct Identifier
			{
				size_t name;
				size_t reference;
			};

			struct Undefined
			{
			};

			using Variant = std::variant<Vector, String, Integer, Number, ObjectPtr, ArrayPtr, LocalizedString,
										 FunctionPointer, Identifier, Undefined>;

			struct Object
			{
				std::unordered_map<std::string, Variant> fields;
			};

			struct Array
			{
				std::vector<Variant> elements;
			};
		}; // namespace type
	};
};