#include "Take_Del.h"

using namespace std;

void setup_console() {
    // Đảm bảo console sử dụng UTF-8
    system("chcp 65001");
    // Thiết lập địa phương để hỗ trợ tiếng Việt
    std::setlocale(LC_ALL, "vi_VN.UTF-8");
}

std::wstring stringToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// Trích xuất tên file từ đường dẫn
std::string extractFileName(const std::string& filePath) {
    size_t pos = filePath.find_last_of("\\/");
    return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
}

void trim(std::wstring& str) {
    // Xóa dấu cách ở đầu chuỗi
    str.erase(0, str.find_first_not_of(L" \t\n\r\f\v"));
    // Xóa dấu cách ở cuối chuỗi
    str.erase(str.find_last_not_of(L" \t\n\r\f\v") + 1);
}

std::string wstringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// Hàm gửi dữ liệu an toàn
int sendData(SOCKET socket, const char* buffer, int length) {
    int totalBytesSent = 0;
    while (totalBytesSent < length) {
        int bytesSent = send(socket, buffer + totalBytesSent, length - totalBytesSent, 0);
        if (bytesSent == SOCKET_ERROR) {
            cout << "Error sending data. Error: " << WSAGetLastError() << endl;
            return SOCKET_ERROR;
        }
        totalBytesSent += bytesSent;
    }
    return totalBytesSent;
}

void sendFile(SOCKET clientSocket, const std::string& filePath) {
    cout << filePath.c_str() << endl;
    wstring wmessage = stringToWstring(filePath);
    trim(wmessage);

    ifstream file(wmessage, ios::binary | ios::ate);
    if (!file.is_open()) {
        cout << "File not found!" << endl;
        perror("Error");
        return;
    }
    // Trích xuất tên file từ đường dẫn
    string fileName = extractFileName(filePath.c_str());
    //wcout << fileName;
    //cout << fileName;
    //string fileName = "Bảo.mp4";
    int fileNameLength = fileName.size();
    // Gửi độ dài tên file
    if (sendData(clientSocket, (char*)&fileNameLength, sizeof(fileNameLength)) == SOCKET_ERROR) {
        return;
    }
    // Gửi tên file
    if (sendData(clientSocket, fileName.c_str(), fileNameLength) == SOCKET_ERROR) {
        return;
    }
    // Lấy kích thước file
    int fileSize = file.tellg();
    file.seekg(0, ios::beg); // Quay lại đầu file
    // Gửi kích thước file
    if (sendData(clientSocket, (char*)&fileSize, sizeof(fileSize)) == SOCKET_ERROR) {
        return;
    }
    // Gửi dữ liệu file
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        if (sendData(clientSocket, buffer, sizeof(buffer)) == SOCKET_ERROR) {
            return;
        }
    }
    // Gửi phần còn lại của file nếu có
    if (file.gcount() > 0) {
        sendData(clientSocket, buffer, file.gcount());
    }
    cout << "File sent successfully: " << fileName << endl;
    file.close();
}