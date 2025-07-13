#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include "EAGetMailObj.tlh"  // Thư viện EAGetMail
#include <winsock2.h>
#include <Ws2tcpip.h>

extern SOCKET sock;

#pragma comment(lib, "ws2_32.lib")
#endif