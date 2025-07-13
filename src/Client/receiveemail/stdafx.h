#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define CURL_STATICLIB

#include <string>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include<map>
#include <sstream>
#include <vector>
#include "EAGetMailObj.tlh"  // Thư viện EAGetMail
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <algorithm>
#include "curl/curl.h"
#include <locale>
#include <codecvt>


extern SOCKET sock;

#pragma comment(lib, "ws2_32.lib")
#endif