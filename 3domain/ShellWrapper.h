#pragma once
#include <string>
class ShellWrapper
{
	public:
		static void OpenFile(std::string filePath);
		static std::wstring Utf8ToUtf16(const std::string& string);
};

