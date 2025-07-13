#include "lib.h"
#include "screenshot.h"
#include "list_app.h"
#include "webcam.h"
#include "Take_Del.h"

#pragma comment(lib, "ws2_32.lib") // Link thư viện Winsock
using namespace std;

void handleClient(int client_sock) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int len = recv(client_sock, buffer, sizeof(buffer), 0);
    if (len <= 0) {
        return;
    }
    buffer[len] = '\0';
    cout << "Client: " << buffer << endl;

    // Các câu lệnh xử lý yêu cầu từ client
    if (strcmp(buffer, "shutdown") == 0) {
        system("shutdown /s /t 1");
        send(client_sock, "shutdown", 8, 0);
    }
    else if (strcmp(buffer, "screenshot") == 0) {
        CaptureScreen("screenshot.bmp");
        send(client_sock, "send_imp", 8, 0);
        send_imp(client_sock);
    }
    else if (strcmp(buffer, "turn_on_webcam") == 0) {
        OpenCamera();
        CameraStream();
        send(client_sock, "Camera turned on", 17, 0);
    }
    else if (strcmp(buffer, "turn_off_webcam") == 0) {
        CloseCamera();
        send(client_sock, "Webcam turned off", 18, 0);
    }
    else if (strcmp(buffer, "recording") == 0) {
        send(client_sock, "recording", 9, 0);
        recv(client_sock, buffer, sizeof(buffer), 0);
        int duration = stoi(buffer);
        RecordScreen(duration);
        std::ifstream inFile("D:\\baooo\\test\\Server\\Server\\output.mp4", std::ios::binary|std::ios::ate);
        size_t fileSize = inFile.tellg();
        send(client_sock,reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
        inFile.close();
        recv(client_sock, buffer, sizeof(buffer), 0);
        //send(client_sock, "Recording started", 9, 0);
        sendVideo(client_sock);
    }
    else if (strcmp(buffer, "fetch") == 0) {
        send(client_sock, "fetch", 5, 0);
        recv(client_sock, buffer, sizeof(buffer), 0);
        sendFile(client_sock, buffer);
    }
    else if (strcmp(buffer, "list_app") == 0) {
        send(client_sock, "list_app", 8, 0);
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);

        string temp, app_name;
        temp = change(buffer);
        if (temp == "start") {
            send(client_sock, "start", 5, 0);
            memset(buffer, 0, sizeof(buffer));
            len = recv(client_sock, buffer, sizeof(buffer), 0);
            buffer[len] = '\0';
            app_name = change(buffer);
            LaunchApp(app_name);
        }
        else if (temp == "stop") {
            send(client_sock, "stop", 4, 0);
            memset(buffer, 0, sizeof(buffer));
            len = recv(client_sock, buffer, sizeof(buffer), 0);
            buffer[len] = '\0';
            app_name = change(buffer);
            stop_app(app_name);
        }
        send_txt(client_sock, "list_app");
    }
    else if (strcmp(buffer, "list_service") == 0) {
        send(client_sock, "list_service", 12, 0);
        memset(buffer, 0, sizeof(buffer));
        len = recv(client_sock, buffer, sizeof(buffer), 0);

        string temp, service_name;
        temp = change(buffer);
        if (temp == "start") {
            send(client_sock, "start", 5, 0);
            memset(buffer, 0, sizeof(buffer));
            len = recv(client_sock, buffer, sizeof(buffer), 0);
            buffer[len] = '\0';
            service_name = change(buffer);
            ManageService(service_name, "start");
        }
        else if (temp == "stop") {
            send(client_sock, "stop", 4, 0);
            memset(buffer, 0, sizeof(buffer));
            len = recv(client_sock, buffer, sizeof(buffer), 0);
            buffer[len] = '\0';
            service_name = change(buffer);
            ManageService(service_name, "stop");
        }

        List_Services();
        send_txt(client_sock, "list_service");
    }
    else if (strcmp(buffer, "delete") == 0) {
        send(client_sock, "delete", 6, 0);
        recv(client_sock, buffer, sizeof(buffer), 0);
        wstring wmessage = stringToWstring(buffer);
        trim(wmessage);
        remove(wstringToString(wmessage).c_str());
        send(client_sock, "done", 4, 0);
    }
}

int main() {
    WSADATA wsa;
    setup_console();

    // Khởi tạo Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "WSAStartup failed. Error: " << WSAGetLastError() << endl;
        return 1;
    }

    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        cout << "Socket creation failed. Error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // Chấp nhận kết nối từ mọi địa chỉ
    server.sin_port = htons(8080); // Cổng server
    int bindResult = ::bind(server_sock, (sockaddr*)&server, sizeof(server));
    // Bind socket với địa chỉ và cổng
    if (bindResult == SOCKET_ERROR) {
        cerr << "Bind failed. Error: " << WSAGetLastError() << endl;
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }


    // Bắt đầu lắng nghe kết nối
    if (listen(server_sock, 3) == SOCKET_ERROR) {
        cout << "Listen failed. Error: " << WSAGetLastError() << endl;
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }

    cout << "Waiting for incoming connections..." << endl;

    SOCKET client_sock;
    struct sockaddr_in client;
    int c = sizeof(struct sockaddr_in);

    while (true) {
        client_sock = accept(server_sock, (struct sockaddr*)&client, &c);
        if (client_sock < 0) {
            cout << "Client accept failed. Error: " << WSAGetLastError() << endl;
            continue; // Tiếp tục vòng lặp nếu có lỗi
        }
        cout << "Client connected, handling requests..." << endl;

        // Xử lý client trong luồng riêng
        std::thread t(handleClient, client_sock);
        t.detach(); // Tách luồng để nó tự quản lý
    }

    // Đóng socket và dọn dẹp Winsock
    closesocket(server_sock);
    WSACleanup();
    return 0;
}
