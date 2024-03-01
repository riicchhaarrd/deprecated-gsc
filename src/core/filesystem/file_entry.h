#pragma once
#include <memory>
#include <string>

#include "file_path.h"

namespace filesystem
{
	class file_entry
	{
		std::string m_path;
		std::shared_ptr<filepath> m_archive;
		enum class file_type
		{
			none,
			regular,
			directory,
			device,
			link
		};
		size_t m_size;
		file_type m_type;
		int m_flags;
		int m_index;
		friend class directory_impl_zip;
		friend class directory_impl_fs;

	  public:
		explicit file_entry(const std::string& path_) noexcept
			: m_path(path_), m_flags(0), m_type(file_type::regular), m_index(-1), m_size(0)
		{
		}
		file_entry() noexcept : m_type(file_type::none), m_flags(0), m_index(-1), m_size(0)
		{
		}

		const filepath& archive_path() const
		{
			return *m_archive.get();
		}

		bool is_inside_archive() const
		{
			return m_archive != nullptr;
		}

		bool is_directory() const
		{
			return m_type == file_type::directory;
		}

		bool is_regular_file() const
		{
			return m_type == file_type::regular;
		}

		const std::string& path() const
		{
			return m_path;
		}

		int index() const
		{
			return m_index;
		}

		size_t size() const
		{
			return m_size;
		}
	};
}; // namespace filesystem