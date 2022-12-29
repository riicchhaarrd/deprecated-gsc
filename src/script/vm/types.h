#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>
#include <common/stringutil.h>
#include <common/type_id.h>
#include <math.h>

namespace script
{
	struct VMContext;

	namespace vm
	{
		struct Vector
		{
			union
			{
				struct
				{
					float x;
					float y;
					float z;
				};
				float data[3];
			};

			Vector()
			{
				x = y = z = 0.f;
			}
			Vector(float a, float b, float c)
			{
				x = a;
				y = b;
				z = c;
			}

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

			float& operator[](const int index)
			{
				if (index < 0 || index >= 3)
					throw std::out_of_range("index < 0 || index >= 3");
				return data[index];
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
			std::string name;
		};

		struct Undefined
		{
		};
		
		using ReferenceType = std::variant<Identifier, ObjectPtr, Vector*>;
		enum class ReferenceTypeId
		{
			kIdentifier,
			kObjectPtr,
			kVector
		};
		struct Reference
		{
			size_t offset = 0;
			int index = -1;
			std::string field;
			ReferenceType object;
		};

		using Variant = std::variant<Undefined, Vector, String, Integer, Number, ObjectPtr, ArrayPtr, LocalizedString,
									 FunctionPointer, Identifier, Animation, Reference>;
		using VariantPtr = std::shared_ptr<vm::Variant>;
		static const char* kVariantNames[] = {"Undefined", "Vector", "String", "Integer", "Float",
											  "Object",	   "Array",	 "String Localized", "Function Pointer", "?",
											  "Animation", "Reference", NULL};

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
			kAnimation,
			kReference,
			kObjectFieldReference
		};

		template<typename T> inline size_t type_index()
		{
			return Variant(T()).index();
		}

		struct Object
		{
			std::string m_tag;
			vm::Variant size;
			Object(const std::string tag) : m_tag(tag), size(vm::Integer())
			{
			}

			template<typename T> T* cast()
			{
				constexpr auto k = type_id<T>::id();
				if (k != this->kind())
					return NULL;
				return (T*)this;
			}

			virtual size_t kind()
			{
				return type_id<Object>::id();
			}

			virtual ~Object()
			{
			}

			std::unordered_map<std::string, vm::Variant> fields;
			virtual bool call_method(const std::string n, VMContext&, int*)
			{
				return false;
			}
			virtual bool set_field(const std::string n, vm::Variant& value)
			{
				if (n == "size")
				{
					throw std::runtime_error("cannot set size for object");
				}
				fields[n] = value;
				return true;
			}
			virtual vm::Variant* get_field(const std::string n, bool create)
			{
				if (fields.find(n) == fields.end())
				{
					if (create)
						fields[n] = vm::Undefined();
					else
						return NULL;
				}
				return &fields[n];
			}
		};

		struct Array
		{
			std::vector<std::shared_ptr<Variant>> elements;
		};

		using Constants = std::vector<Variant>;
	}; // namespace vm
};	   // namespace script
