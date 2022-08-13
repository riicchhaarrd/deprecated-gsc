#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace script
{
	struct VMContext;

	namespace vm
	{
		struct Vector
		{
			float x = 0.f;
			float y = 0.f;
			float z = 0.f;

			float dot(const Vector& o)
			{
				return x * o.x + y * o.y + z * o.z;
			}

			float length()
			{
				return sqrtf(dot(*this));
			}

			float distance(const Vector& o)
			{
				Vector v = (*this) - o;
				return v.length();
			}

			Vector operator-(const Vector& o)
			{
				Vector v;
				v.x = x - o.x;
				v.y = y - o.y;
				v.z = z - o.z;
				return v;
			}
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
			std::string reference;
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
		using VariantPtr = std::shared_ptr<vm::Variant>;
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

		struct ObjectMethod
		{
			virtual int execute(VMContext&, void*) = 0;
		};

		struct Object
		{
			virtual ~Object()
			{
			}
			std::unordered_map<std::string, std::shared_ptr<Variant>> fields;
			virtual bool get_method(const std::string n, ObjectMethod** ptr, void **obj)
			{
				*ptr = nullptr;
				*obj = nullptr;
				return false;
			}
			virtual std::shared_ptr<Variant> get_field(const std::string n)
			{
				if (n == "size")
				{
					int i = fields.size();
					return std::make_shared<Variant>(i);
				}
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