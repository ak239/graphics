#include "stdafx.h"
#include "Common.h"

#include <fstream>

char* Common::GetFileContent(const std::string& fileName, GLint* outSize)
{
	*outSize = 0;
	std::ifstream file;
	file.open(fileName, std::ios::in|std::ios::binary|std::ios::ate);
	if (file.is_open())
	{
		GLint size = (GLint)file.tellg();
		char* memblock = new char [size + 1];
		file.seekg (0, std::ios::beg);
		file.read (memblock, size);
		file.close();
		if (size == 0)
			return 0;

		memblock[size] = '\0';
		*outSize = size;

		return memblock;
	}
	return 0;
}

