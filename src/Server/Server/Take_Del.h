#pragma once
#include "lib.h"

void setup_console();

std::wstring stringToWstring(const std::string& str);

std::string extractFileName(const std::string& filePath);

std::string wstringToString(const std::wstring& wstr);

int sendData(SOCKET socket, const char* buffer, int length);

void sendFile(SOCKET clientSocket, const std::string& filePath);

void trim(std::wstring& str);