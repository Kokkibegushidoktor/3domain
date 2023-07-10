#include "ShellWrapper.h"
#include <Windows.h>
#include <shellapi.h>

void ShellWrapper::OpenFile(std::string filePath)
{
	ShellExecuteW(NULL, L"open", Utf8ToUtf16(filePath).c_str(), NULL, NULL, SW_SHOWNORMAL);
}
std::wstring ShellWrapper::Utf8ToUtf16(const std::string& utf8)
{
	std::wstring utf16;
	if (utf8.empty())
	{
		return utf16;
	}
	constexpr DWORD kFlags = MB_ERR_INVALID_CHARS;
	const int utf8Length = static_cast<int>(utf8.length());
	const int utf16Length = ::MultiByteToWideChar(
		CP_UTF8,       // Source string is in UTF-8
		kFlags,        // Conversion flags
		utf8.data(),   // Source UTF-8 string pointer
		utf8Length,    // Length of the source UTF-8 string, in chars
		nullptr,       // Unused - no conversion done in this step
		0              // Request size of destination buffer, in wchar_ts
	);

	utf16.resize(utf16Length);
	int result = ::MultiByteToWideChar(
		CP_UTF8,       // Source string is in UTF-8
		kFlags,        // Conversion flags
		utf8.data(),   // Source UTF-8 string pointer
		utf8Length,    // Length of source UTF-8 string, in chars
		&utf16[0],     // Pointer to destination buffer
		utf16Length    // Size of destination buffer, in wchar_ts          
	);

	return utf16;
}