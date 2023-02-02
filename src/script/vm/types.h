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

		struct Object
		{
			std::string m_tag;
			vm::Variant size;
			Object(const std::string tag) : m_tag(tag), size(vm::Integer())
			{
			}

			virtual std::weak_ptr<void> get_proxy_object()
			{
				return {};
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
			bool set_field(const std::string n, vm::Variant& value)
			{
				if (n == "size")
				{
					throw std::runtime_error("cannot set size for object");
				}
				fields[n] = value;
				return true;
			}
			vm::Variant* get_field(const std::string n, bool create)
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

		template<typename T>
		struct ProxyObject : Object
		{
			std::weak_ptr<T> class_instance_ptr;
			ProxyObject(const std::string tag, std::weak_ptr<T> ptr) : Object(tag), class_instance_ptr(ptr)
			{
			}

			virtual std::weak_ptr<void> get_proxy_object() override
			{
				return class_instance_ptr;
			}

			std::shared_ptr<T> get_proxy_shared()
			{
				return std::static_pointer_cast<T>(get_proxy_object().lock());
			}

			virtual size_t kind() override
			{
				return type_id<ProxyObject<T>>::id();
			}
		};


		struct Array
		{
			std::vector<std::shared_ptr<Variant>> elements;
		};

		using Constants = std::vector<Variant>;
	}; // namespace vm
};	   // namespace script
