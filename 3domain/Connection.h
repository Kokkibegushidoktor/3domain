#pragma once
#include <string>
#include <raylib.h>

class Connection
{
	bool _isSet;
	std::string _filePath;
public:
	Connection();
	Connection(std::string filePath);
	bool IsSet();
	bool IsValid();
	std::string GetPath();
	bool SetConnection(std::string filePath);
	void ReSet();
};

