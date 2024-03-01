#pragma once
#include <unordered_map>
#include <string>
#include <script/vm/types.h>

struct vec2
{
	union
	{
		struct
		{
			float x, y;
		};
		float components_[2];
	};
	float& operator[](const size_t index)
	{
		return components_[index];
	}
};
struct vec3
{
	union
	{
		struct
		{
			float x, y, z;
		};
		float components_[3];
	};
	float& operator[](const size_t index)
	{
		return components_[index];
	}
};
struct vec4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		float components_[4];
	};
	float& operator[](const size_t index)
	{
		return components_[index];
	}
};

namespace script
{
	class PropertyHandler
	{
		vm::Variant& m_value;

	  public:
		PropertyHandler(vm::Variant& val) : m_value(val)
		{
		}
		template <typename T> void get_value(const T& t)
		{
			m_value = t;
		}
		template <typename T> void set_value(T& t);
		void set_value(vm::ObjectPtr& ptr)
		{
			throw std::runtime_error("no object ptr");
		}

		void get_value(const vec4& v)
		{
			throw std::runtime_error("no vec4");
		}
		void set_value(vec4& v)
		{
			throw std::runtime_error("no vec4");
		}
		void get_value(const vec3& v)
		{
			m_value = vm::Vector(v.x, v.y, v.z);
		}
		void set_value(vec3& v)
		{
			auto tmp = std::get<vm::Vector>(m_value);
			v.x = tmp.x;
			v.y = tmp.y;
			v.z = tmp.z;
		}
		void get_value(const vec2& v)
		{
			throw std::runtime_error("no vec2");
		}
		void set_value(vec2& v)
		{
			throw std::runtime_error("no vec2");
		}

		void set_value(int& t)
		{
			t = std::get<vm::Integer>(m_value);
		}
		void set_value(std::string& t)
		{
			if (m_value.index() == (int)vm::Type::kString)
				t = std::get<vm::String>(m_value);
			else if (m_value.index() == (int)vm::Type::kLocalizedString)
				t = std::get<vm::LocalizedString>(m_value).reference;
			else
				throw std::runtime_error("expected string");
		}
		void set_value(bool& t)
		{
			t = std::get<vm::Integer>(m_value) > 0;
		}
		void set_value(float& t)
		{
			if (m_value.index() == (int)vm::Type::kInteger)
				t = (float)std::get<vm::Integer>(m_value);
			else
				t = std::get<vm::Number>(m_value);
		}
	};

	struct IProperty
	{
		virtual void set_value(PropertyHandler& handler, void* ptr) = 0;
		virtual void get_value(PropertyHandler& handler, void* ptr) = 0;
	};

	template <typename T> class Property : public IProperty
	{
		enum class Type
		{
			kString,
			kInt,
			kFloat,
			kVec2,
			kVec3,
			kVec4,
			kBool,
			kObject
		};
		Type type;
		union
		{
			std::string T::*string_value;
			int T::*int_value;
			float T::*float_value;
			vec4 T::*vec4_value;
			vec3 T::*vec3_value;
			vec2 T::*vec2_value;
			bool T::*bool_value;
			vm::ObjectPtr T::*object_value;
		};

	  public:
		Property(int T::*p) : type(Type::kInt)
		{
			int_value = p;
		}
		Property(vm::ObjectPtr T::*p) : type(Type::kObject)
		{
			object_value = p;
		}
		Property(bool T::*p) : type(Type::kBool)
		{
			bool_value = p;
		}
		Property(float T::*p) : type(Type::kFloat)
		{
			float_value = p;
		}
		Property(std::string T::*p) : type(Type::kString)
		{
			string_value = p;
		}
		Property(vec4 T::*p) : type(Type::kVec4)
		{
			vec4_value = p;
		}
		Property(vec3 T::*p) : type(Type::kVec3)
		{
			vec3_value = p;
		}
		Property(vec2 T::*p) : type(Type::kVec2)
		{
			vec2_value = p;
		}

		virtual void set_value(PropertyHandler& handler, void* ptr) override
		{
			set_value_impl(handler, static_cast<T*>(ptr));
		}
		virtual void get_value(PropertyHandler& handler, void* ptr) override
		{
			get_value_impl(handler, static_cast<T*>(ptr));
		}

		void set_value_impl(PropertyHandler& handler, T* subject)
		{
			switch (type)
			{
			case Type::kInt:
				handler.set_value(subject->*int_value);
				break;
			case Type::kBool:
				handler.set_value(subject->*bool_value);
				break;
			case Type::kFloat:
				handler.set_value(subject->*float_value);
				break;
			case Type::kString:
				handler.set_value(subject->*string_value);
				break;
			case Type::kVec4:
				handler.set_value(subject->*vec4_value);
				break;
			case Type::kVec3:
				handler.set_value(subject->*vec3_value);
				break;
			case Type::kVec2:
				handler.set_value(subject->*vec2_value);
				break;
			case Type::kObject:
				handler.set_value(subject->*object_value);
				break;
			}
		}
		void get_value_impl(PropertyHandler& handler, T* subject)
		{
			switch (type)
			{
			case Type::kInt:
				handler.get_value(subject->*int_value);
				break;
			case Type::kBool:
				handler.get_value(subject->*bool_value);
				break;
			case Type::kFloat:
				handler.get_value(subject->*float_value);
				break;
			case Type::kString:
				handler.get_value(subject->*string_value);
				break;
			case Type::kVec4:
				handler.get_value(subject->*vec4_value);
				break;
			case Type::kVec3:
				handler.get_value(subject->*vec3_value);
				break;
			case Type::kVec2:
				handler.get_value(subject->*vec2_value);
				break;
			case Type::kObject:
				handler.get_value(subject->*object_value);
				break;
			}
		}
	};

	template <typename T> struct NamedProperty
	{
		const char* name;
		Property<T> prop;
	};

	template<typename T>
	using NamedPropertyMap = std::unordered_map<std::string, Property<T>>;
}; // namespace ui