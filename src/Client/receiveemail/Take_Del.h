#pragma once
#include "stdafx.h"

extern std::string dir;

void setup_console();

std::wstring stringToWstring(const std::string& str);

std::string extractFileName(const std::string& filePath);

std::string wstringToString(const std::wstring& wstr);

void receiveFile(SOCKET serverSocket, std::string& dir);

void trim(std::wstring& str);
