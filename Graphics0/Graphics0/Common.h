#pragma once

#include <string>
#include <fstream>

namespace Common
{
	char* GetFileContent(const std::string& fileName, GLint* outSize);
};

