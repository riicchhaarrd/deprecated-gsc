#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace script
{
	namespace vm
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
		using ObjectPtr = std::shared_ptr<Object>;
		using ArrayPtr = std::shared_ptr<Array>;

		struct LocalizedString
		{
			size_t index;
		};

		struct Animation
		{
			std::string reference;
		};

		struct FunctionPointer
		{
			std::string file;
			std::string name;
		};

		struct Identifier
		{
			size_t name;
			size_t reference;
		};

		struct Undefined
		{
		};

		using Variant = std::variant<Undefined, Vector, String, Integer, Number, ObjectPtr, ArrayPtr, LocalizedString,
									 FunctionPointer, Identifier, Animation>;
		static const char* kVariantNames[] = {"Undefined", "Vector", "String", "Integer", "Float",
											  "Object",	   "Array",	 "String Localized", "Function Pointer", "?",
											  "Animation", NULL};

		enum class Type
		{
			kUndefined,
			kVector,
			kString,
			kInteger,
			kFloat,
			kObject,
			kArray,
			kLocalizedString,
			kFunctionPointer,
			kIdentifier,
			kAnimation
		};

		template<typename T> inline size_t type_index()
		{
			return Variant(T()).index();
		}

		struct Object
		{
			std::unordered_map<std::string, std::shared_ptr<Variant>> fields;

			std::shared_ptr<Variant> get_field(const std::string n)
			{
				if (fields.find(n) == fields.end())
				{
					fields[n] = std::make_shared<vm::Variant>(vm::Undefined());
				}
				return fields[n];
			}
		};

		struct Array
		{
			std::vector<std::shared_ptr<Variant>> elements;
		};

		using Constants = std::vector<Variant>;
	}; // namespace vm
};	   // namespace script