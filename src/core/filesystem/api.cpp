#include "api.h"

bool filesystem_api::read_text_entry(const std::string& path, std::string& s)
{
	auto entry = read_entry(path);
	if (!entry)
		return false;
	const char* data = (const char*)entry->data();
	// convert crlf to lf
	size_t sz = entry->size();
	for (size_t i = 0; i < sz; ++i)
	{
		int pk = (i + 1) < sz ? data[i + 1] : 0;
		if (data[i] == '\r' && pk == '\n')
		{
			continue;
		}
		s.push_back(data[i]);
	}
	// s = std::string(data, entry->size());
	return true;
}
