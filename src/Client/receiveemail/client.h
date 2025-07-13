#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include "stdafx.h"
std::wstring GetLineAtIndex();
void receive_txt(SOCKET serverSocket, const char* outputFilePath);
bool client_run();
#endif