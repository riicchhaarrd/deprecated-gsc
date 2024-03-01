#include "filesystem.h"
#include <fstream>

namespace filesystem
{
	file_entry_data read_file(const std::string& _path)
	{
		std::ifstream in(_path, std::ios::binary | std::ios::ate);
		if (!in.is_open())
			return nullptr;
		size_t size = in.tellg();
		in.seekg(0, std::ios::beg);
		std::shared_ptr<buffer> ptr = std::make_shared<buffer>();
		ptr->resize(size);
		in.read((char*)ptr->data(), size);
		in.close();
		return ptr;
	}
};