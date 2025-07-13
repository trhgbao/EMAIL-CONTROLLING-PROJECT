#include "Take_Del.h"
#include "stdafx.h"

using namespace std;

string dir = "";

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

std::string wstringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void trim(std::wstring& str) {
    // Xóa dấu cách ở đầu chuỗi
    str.erase(0, str.find_first_not_of(L" \t\n\r\f\v"));
    // Xóa dấu cách ở cuối chuỗi
    str.erase(str.find_last_not_of(L" \t\n\r\f\v") + 1);
}

// Hàm nhận file từ server và lưu vào file cục bộ
void receiveFile(SOCKET serverSocket, string& dir) {
    // Nhận độ dài tên file
    int fileNameLength;
    if (recv(serverSocket, (char*)&fileNameLength, sizeof(fileNameLength), 0) <= 0) {
        cout << "Error receiving file name length!" << endl;
        return;
    }

    // Nhận tên file
    char* fileName = new char[fileNameLength + 1];
    if (recv(serverSocket, fileName, fileNameLength, 0) <= 0) {
        cout << "Error receiving file name!" << endl;
        delete[] fileName;
        return;
    }
    fileName[fileNameLength] = '\0';

    // Chuyển tên file từ char[] sang wstring để xử lý tên file có tiếng Việt
    wstring savePath = L"D:\\baooo\\test\\Client\\receiveemail\\" + stringToWstring(string(fileName));
    trim(savePath);
    // Tạo file để lưu dữ liệu
    ofstream file(savePath, ios::binary);
    if (!file.is_open()) {
        wcout << "Unable to create or open the file: " << savePath << endl;
        delete[] fileName;
        perror("Error");
        return;
    }

    dir = wstringToString(savePath); // Đường dẫn file được lưu

    // Nhận kích thước file
    int fileSize;
    if (recv(serverSocket, (char*)&fileSize, sizeof(fileSize), 0) <= 0) {
        cout << "Error receiving file size!" << endl;
        file.close();
        delete[] fileName;
        return;
    }

    char buffer[1024];
    int totalBytesReceived = 0;
    while (totalBytesReceived < fileSize) {
        int bytesReceived = recv(serverSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "Error: File received incomplete!" << endl;
            break;
        }
        totalBytesReceived += bytesReceived;
        file.write(buffer, bytesReceived);
    }

    if (totalBytesReceived == fileSize) {
        cout << "\nFile received successfully!" << endl;
    }
    else {
        cout << "Error: File received incomplete!" << endl;
    }

    file.close();
    delete[] fileName;
}
