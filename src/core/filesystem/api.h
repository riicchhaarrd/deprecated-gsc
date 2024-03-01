#pragma once
#include <core/filesystem/file_entry.h>
#include <vector>

namespace filesystem
{
	using buffer = std::vector<unsigned char>;
	using file_entry_data = std::shared_ptr<buffer>;
}; // namespace filesystem

class filesystem_api
{
  public:
	filesystem_api()
	{
	}
	virtual ~filesystem_api()
	{
	}
	virtual filesystem::file_entry_data read_entry(const std::string& s) = 0;
	virtual const filesystem::file_entry* get_entry(const std::string& s) const = 0;
	virtual bool file_exists(const std::string& s) const = 0;

	bool read_text_entry(const std::string& path, std::string& s);
};
