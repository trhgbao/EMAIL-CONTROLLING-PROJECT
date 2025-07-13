#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include "stdafx.h"
void receive_txt(SOCKET serverSocket, const char* outputFilePath);
int client_run();
#endif