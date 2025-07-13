#include "stdafx.h"

#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")

extern std::string smtpServer;
extern int port;
extern std::string username;
extern std::string password;
extern std::string fromEmail;
extern std::string subject;
extern std::string message;
extern std::string imagePath;
extern std::string list_app_Path;
extern std::string list_service_Path;


DWORD _getCurrentPath(LPTSTR lpPath, DWORD nSize);

std::string base64_encode(const std::string& in);

std::string readFile(const std::string& filePath);

size_t payload_source_(void* ptr, size_t size, size_t nmemb, void* userp);

bool checkaddress(std::string a);

bool sendEmail();

int run(int argc, _TCHAR* argv[]);
