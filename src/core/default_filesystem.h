#pragma once

#include "../common/filesystem.h"
#include <core/filesystem/api.h>

class default_filesystem : public filesystem_api
{
  protected:
	std::unordered_map<std::string, filesystem::file_entry> m_entries;
	virtual void add_entry(const std::string& _path, const filesystem::file_entry& entry)
	{
		m_entries[_path] = entry;
	}

  public:
	default_filesystem()
	{
	}
	virtual ~default_filesystem()
	{
	}

	virtual filesystem::file_entry_data read_entry(const std::string& s) override
	{
		return filesystem::read_file(s);
	}

	virtual const filesystem::file_entry* get_entry(const std::string& s) const override
	{
		return NULL;
	}

	virtual bool file_exists(const std::string&) const override
	{
		fprintf(stderr, "use platform::file_exists\n");
		exit(1);
	}
};
