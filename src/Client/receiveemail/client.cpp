#include "client.h"
#include "screenshot.h"
#include "Take_Del.h"
#include "receiveemail.h"

using namespace std; 

SOCKET sock;

void receive_txt(SOCKET serverSocket, const char* outputFilePath) {
    std::ofstream file(outputFilePath, std::ios::out | std::ios::binary);

    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(serverSocket, buffer, 1024, 0)) > 0) {
        file.write(buffer, bytesReceived);
    }
}

wstring GetLineAtIndex() {
    std::wifstream file("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::vector<std::wstring> lines;
    std::wstring line;

    // Đọc từng dòng và lưu vào vector
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    // Kiểm tra nếu file rỗng
    if (lines.empty()) {
        throw std::runtime_error("The file is empty");
    }

    // Lấy dòng cuối cùng và chuyển thành số nguyên
    int index;
    try {
        index = std::stoi(lines.back());
    }
    catch (const std::exception&) {
        throw std::runtime_error("The last line is not a valid integer");
    }

    // Kiểm tra chỉ số hợp lệ
    if (index < 0 || index >= static_cast<int>(lines.size()) - 1) {
        throw std::out_of_range("Index from the last line is out of range");
    }

    // Trả về dòng tương ứng
    return lines[index].c_str();
}

bool client_run() {
    WSADATA wsa;

    // Khởi tạo Winsock

    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server2;
    if (InetPton(AF_INET, GetLineAtIndex().c_str(), &server2.sin_addr) != 1) {
        std::cout << "Invalid address" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //memset(&server2, 0, sizeof(server2));
    server2.sin_family = AF_INET;
    server2.sin_port = htons(8080); // Cổng server

    // Kết nối tới server
    if (connect(sock, (struct sockaddr*)&server2, sizeof(server2)) < 0) {
        cout << "Connection failed. Error: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buffer[1024];

    string message;
    ifstream f("output.txt");
    getline(f, message);
    if (!checkaddress(message)) {
        return false;
    }
    while (getline(f, message)) {
        // Gửi tin nhắn
        send(sock, message.c_str(), message.length(), 0);

        // Đợi nhận phản hồi từ server
        memset(buffer, 0, sizeof(buffer)); // Xóa bộ đệm
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len <= 0) {
            cout << "Server disconnected or recv error." << endl;
            break;
        }

        buffer[len] = '\0'; // Kết thúc chuỗi
        if (strcmp(buffer, "send_imp") == 0) {
            receive_imp(sock);
            cout << "Done!";
            break;
        }
        cout << "Function: " << buffer;
        if (strcmp(buffer, "shutdown") == 0) {
            cout << "Done!" ;
            break;
        }
        if (strcmp(buffer, "recording") == 0) {
            size_t fileSize, totalBytesReceived = 0;
            getline(f, message);
            //int duration = stoi(message);
            //std::string fileSizeStr = std::to_string(fileSize);
            send(sock, message.c_str(), message.length(), 0);
            recv(sock, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
            std::cerr << "Received file size: " << fileSize << std::endl;
            send(sock, "", message.length(), 0);
            receiveVideo(sock, fileSize);
            cout << "Done!";
            break;
        }

        if (strcmp(buffer, "list_app") == 0) {
            getline(f, message);
            send(sock, message.c_str(), message.length(), 0);
            cout << "Function: " << message;
            memset(buffer, 0, sizeof(buffer)); // Xóa bộ đệm
            recv(sock, buffer, sizeof(buffer), 0);
            getline(f, message);
            send(sock, message.c_str(), message.length(), 0);
            cout << "Function: " << message;
            receive_txt(sock, "list_app.txt");
            cout << "Done!";
            break;
        }

        if (strcmp(buffer, "list_service") == 0) {
            getline(f, message);
            send(sock, message.c_str(), message.length(), 0);
            cout << "Function: " << message;
            memset(buffer, 0, sizeof(buffer)); // Xóa bộ đệm
            int len = recv(sock, buffer, sizeof(buffer), 0);
            cout << buffer << endl;
            getline(f, message);
            send(sock, message.c_str(), message.length(), 0);
            cout << "Function: " << message;
            receive_txt(sock, "list_service.txt");
            cout << "Done!";
            break;
        }

        if (strcmp(buffer, "fetch") == 0) {
            getline(f, message);
            send(sock, message.c_str(), message.length(), 0);
            memset(buffer, 0, sizeof(buffer)); // Xóa bộ đệm
            receiveFile(sock, dir); 
            cout << "Done!";
            break;
        }
        if (strcmp(buffer, "delele") == 0) {
            getline(f, message);
            send(sock, message.c_str(), message.length(), 0);
            recv(sock, buffer, sizeof(buffer), 0);
            cout << "Done!";
            break;
        }
    }
    closesocket(sock);
    // Dọn dẹp Winsock
    WSACleanup();
    return true;
}

