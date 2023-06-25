#pragma once
#include <memory>
#include <string>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>
#include <common/stringutil.h>
#include <common/type_id.h>
#include <math.h>

enum
{
	k_EScriptObjectTypeArray = -1,
	k_EScriptObjectTypeDefault = 0
};

namespace script
{
	struct VMContext;

	struct IMethod
	{
		virtual int execute(VMContext&, void*) = 0;
	};

	template <typename T> struct Method : IMethod
	{
		virtual int execute(VMContext& ctx, void* ptr) override
		{
			return execute_impl(ctx, *(T*)ptr);
		}
		virtual int execute_impl(VMContext&, T&) = 0;
	};

	template <typename T> class MethodEntry
	{
	  public:
		using Entries = std::unordered_map<std::string, Method<T>*>;

		static Entries& get_methods()
		{
			static Entries methods;
			return methods;
		}

		MethodEntry(const char* name, Method<T>* entry)
		{
			get_methods()[name] = entry;
		}
	};

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

		struct Undefined
		{
		};

		struct Reference
		{
			std::optional<std::string> field;
		};

		using Variant = std::variant<Undefined, Vector, String, Integer, Number, ObjectPtr, LocalizedString, FunctionPointer, Animation, Reference>;
		using VariantPtr = std::shared_ptr<vm::Variant>;
		static const char* kVariantNames[] = {"Undefined", "Vector",	"String",		   "Integer",
											  "Number",	   "ObjectPtr", "LocalizedString", "FunctionPointer",
											  "Animation", "Reference"};

		enum class Type
		{
			kUndefined,
			kVector,
			kString,
			kInteger,
			kFloat,
			kObject,
			kLocalizedString,
			kFunctionPointer,
			kAnimation,
			kReference
		};

		template<typename T> inline size_t type_index()
		{
			return Variant(T()).index();
		}

		struct ObjectLookupTable
		{
			virtual bool call(const std::string&, VMContext&, int*) = 0;
			virtual void set(const std::string&, vm::Variant&) = 0;
			virtual vm::Variant get(const std::string&) = 0;
		};

		class Object
		{
			friend class VirtualMachine;

			std::string m_tag;
			std::unordered_map<std::string, vm::Variant> m_fields;

		  public:
			Object(const std::string& tag) : m_tag(tag)
			{
			}
			virtual ~Object()
			{
			}

			const std::unordered_map<std::string, vm::Variant>& fields() const
			{
				return m_fields;
			}

			virtual int type_id()
			{
				return k_EScriptObjectTypeDefault;
			}

			const size_t size() const
			{
				return m_fields.size();
			}

			void set_field(const std::string& key, vm::Variant& value)
			{
				if (key == "size")
				{
					throw std::runtime_error("Cannot set field 'size' for Object");
				}
				m_fields[key] = value;
			}
			vm::Variant* get_field(const std::string n, bool create)
			{
				if (m_fields.find(n) == m_fields.end())
				{
					if (create)
						m_fields[n] = vm::Undefined();
					else
						return NULL;
				}
				return &m_fields[n];
			}
		};
		/*
		struct Array
		{
			std::vector<std::shared_ptr<Variant>> elements;
		};
		*/
		class Array : public Object
		{
		  public:
			using Object::Object;
			virtual int type_id()
			{
				return k_EScriptObjectTypeArray;
			}
		};

		using Constants = std::vector<Variant>;
	}; // namespace vm
};	   // namespace script
