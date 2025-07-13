#pragma once
#include "lib.h"

void ListExecutableAppsRecursive(const std::string& directoryPath, std::vector<std::string>& appList);

void List_App();

void List_Services();

std::string change(char* buffer);

void LaunchApp(const std::string& appName);

std::wstring ConvertToWideString(const std::string& input);

bool ManageService(const std::string& serviceName, const std::string& action);

void stop_app(const std::string& processName);

void send_txt(SOCKET clientSocket, std::string a);