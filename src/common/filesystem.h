#pragma once
#include <string>
#include <core/filesystem/file_entry.h>
#include <core/filesystem/api.h>

namespace filesystem
{
	file_entry_data read_file(const std::string& _path);
};