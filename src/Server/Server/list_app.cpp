#include "list_app.h"

using namespace std;

void ListExecutableAppsRecursive(const string& directoryPath, vector<string>& appList) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    // Tìm tất cả file và thư mục trong thư mục hiện tại
    string searchPath = directoryPath + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cerr << "Error: Unable to open directory: " << directoryPath << endl;
        return;
    }

    do {
        // Bỏ qua các thư mục đặc biệt "." và ".."
        string fileName = findFileData.cFileName;
        if (fileName == "." || fileName == "..") {
            continue;
        }

        string fullPath = directoryPath + "\\" + fileName;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Nếu là thư mục, duyệt đệ quy
            ListExecutableAppsRecursive(fullPath, appList);
        }
        else {
            // Nếu là file .exe, thêm vào danh sách
            if (fullPath.size() > 4 && fullPath.substr(fullPath.size() - 4) == ".exe") {
                appList.push_back(fullPath);
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void List_App() {
    ofstream outFile("list_app.txt");
    vector<string> apps;
    //ListExecutableAppsRecursive("C:\\Windows\\System32", apps);
    //ListExecutableAppsRecursive("C:\\Program Files", apps);
    //ListExecutableAppsRecursive("C:\\Program Files (x86)", apps);
    //ListExecutableAppsRecursive("C:\\Users", apps);
    ListExecutableAppsRecursive("D:", apps);
    ListExecutableAppsRecursive("C:", apps);
    if (outFile.is_open()) {
        for (const auto& app : apps) {
            outFile << app << endl;
        }
    }
    return;
}

void List_Services() {
    HRESULT hres;

    // Khởi tạo COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "COM initialization failed." << std::endl;
        return;
    }

    // Thiết lập bảo mật COM
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL,
        EOAC_NONE, NULL);

    if (FAILED(hres)) {
        CoUninitialize();
        std::cout << "Security initialization failed." << std::endl;
        return;
    }

    IWbemLocator* pLoc = NULL;

    // Tạo đối tượng Locator WMI
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres)) {
        CoUninitialize();
        std::cout << "WMI locator creation failed." << std::endl;
        return;
    }

    IWbemServices* pSvc = NULL;

    // Kết nối tới namespace WMI
    hres = pLoc->ConnectServer(
        BSTR(L"ROOT\\CIMV2"),
        NULL, NULL, 0, NULL, 0, 0, &pSvc);

    if (FAILED(hres)) {
        pLoc->Release();
        pLoc = nullptr;
        CoUninitialize();
        std::cout << "WMI connection failed." << std::endl;
        return;
    }

    // Thiết lập bảo mật WMI
    hres = CoSetProxyBlanket(
        pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE);

    if (FAILED(hres)) {
        pSvc->Release();
        pSvc = nullptr;
        pLoc->Release();
        pLoc = nullptr;
        CoUninitialize();
        std::cout << "Proxy blanket failed." << std::endl;
        return;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        BSTR(L"WQL"),
        BSTR(L"SELECT * FROM Win32_Service"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator);

    if (FAILED(hres)) {
        pSvc->Release();
        pSvc = nullptr;
        pLoc->Release();
        pLoc = nullptr;
        CoUninitialize();
        std::cout << "Query for services failed." << std::endl;
        return;
    }

    // Mở file để ghi danh sách dịch vụ
    std::ofstream outFile("list_service.txt");

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    // Ghi dữ liệu ra file
    outFile << "Danh sách các dịch vụ trên hệ thống:\n";
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (FAILED(hr) || uReturn == 0) {
            std::cout << "Hoàn thành việc liệt kê dịch vụ." << std::endl;
            break;
        }

        VARIANT vtProp;
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR) {
            outFile << "Service: " << (char*)_bstr_t(vtProp.bstrVal) << std::endl;
        }
        VariantClear(&vtProp);

        pclsObj->Release();
        pclsObj = nullptr;
    }

    // Đóng file
    outFile.close();

    // Dọn dẹp
    if (pEnumerator) {
        pEnumerator->Release();
        pEnumerator = nullptr;
    }
    if (pSvc) {
        pSvc->Release();
        pSvc = nullptr;
    }
    if (pLoc) {
        pLoc->Release();
        pLoc = nullptr;
    }

    // Hủy COM
    CoUninitialize();
}

string change(char* buffer) {
    string temp = "";
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] != 13 && buffer[i] != NULL) { // Bỏ qua ký tự xuống dòng
            temp += buffer[i];
        }
        else {
            break;
        }
    }
    return temp;
}

void LaunchApp(const std::string& appName) {
    std::wstring appNameW(appName.begin(), appName.end());
    ShellExecuteW(NULL, L"open", appNameW.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void stop_app(const std::string& processName) {
    string command = "taskkill /IM " + processName + " /F";
    system(command.c_str());
}

std::wstring ConvertToWideString(const std::string& input) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, nullptr, 0);
    std::wstring wideString(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &wideString[0], size_needed);
    return wideString;
}

bool ManageService(const std::string& serviceName, const std::string& action) {
    SC_HANDLE serviceManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    DWORD access = SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP;
    SC_HANDLE services = OpenService(serviceManager, ConvertToWideString(serviceName).c_str(), access);
    if (!services) {
        std::cerr << "Error opening service: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Service opened successfully!" << std::endl;
        CloseServiceHandle(services);
    }
    if (!serviceManager) {
        std::cerr << "Failed to open service manager. Error: " << GetLastError() << std::endl;
        return false;
    }

    DWORD desiredAccess = SERVICE_START;
    if (action == "stop") {
        desiredAccess = SERVICE_STOP;
    }

    std::wstring serviceNameW = ConvertToWideString(serviceName);
    SC_HANDLE service = OpenService(serviceManager, serviceNameW.c_str(), desiredAccess);
    if (!service) {
        std::cerr << "Failed to open service: " << serviceName << ". Error: " << GetLastError() << std::endl;
        CloseServiceHandle(serviceManager);
        return false;
    }

    bool result = false;
    if (action == "start") {
        if (StartService(service, 0, nullptr)) {
            std::cout << "Service " << serviceName << " started successfully." << std::endl;
            result = true;
        }
        else {
            std::cerr << "Failed to start service: " << serviceName << ". Error: " << GetLastError() << std::endl;
        }
    }
    else if (action == "stop") {
        SERVICE_STATUS serviceStatus;
        if (ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus)) {
            std::cout << "Service " << serviceName << " stopped successfully." << std::endl;
            result = true;
        }
        else {
            std::cerr << "Failed to stop service: " << serviceName << ". Error: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cerr << "Invalid action: " << action << ". Use 'start' or 'stop'." << std::endl;
    }

    CloseServiceHandle(service);
    CloseServiceHandle(serviceManager);
    return result;
}


void send_txt(SOCKET clientSocket, std::string a) {
    std::fstream file;
    if (a == "list_app") {
        file.open("list_app.txt", std::ios::in | std::ios::binary);
    }
    else if (a == "list_service") {
        file.open("list_service.txt", std::ios::in | std::ios::binary);
    }
    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, 1024);
        int bytesRead = file.gcount();
        send(clientSocket, buffer, bytesRead, 0);
    }
    file.close();
}