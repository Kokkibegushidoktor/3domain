#include <filesystem>
#include "Connection.h"

Connection::Connection()
{
	_isSet = false;
}
Connection::Connection(std::string filePath)
{
	auto s = std::filesystem::u8path(filePath);
	if (std::filesystem::exists(s))
	{
		_isSet = true;
		_filePath = filePath;
	}
	else
		_isSet = false;
}
bool Connection::IsSet()
{
	return _isSet;
}
bool Connection::SetConnection(std::string filePath)
{
	auto s = std::filesystem::u8path(filePath);
	if(std::filesystem::exists(s))
	{
		_filePath = filePath;
		_isSet = true;
		return true;
	}
	return false;
}
bool Connection::IsValid()
{
	if (!_isSet) return false;
	auto s = std::filesystem::u8path(_filePath);
	if (!std::filesystem::exists(s)) return false;
	return true;
}
std::string Connection::GetPath()
{
	return _filePath;
}
void Connection::ReSet()
{
	_filePath.clear();
	_isSet = false;
}