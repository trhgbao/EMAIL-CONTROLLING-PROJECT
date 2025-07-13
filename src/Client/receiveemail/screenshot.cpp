#include "stdafx.h"
#include "screenshot.h"

#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#define CHUNK_SIZE 1024


// Hàm lưu ảnh chụp màn hình thành file BMP
void receive_imp(SOCKET clientSocket) {
    // Nhận kích thước file ảnh
    streamsize imageSize;
    recv(clientSocket, (char*)&imageSize, sizeof(imageSize), 0);

    // Nhận dữ liệu ảnh
    char* imageData = new char[imageSize];
    int bytesReceived = 0;
    while (bytesReceived < imageSize) {
        int result = recv(clientSocket, imageData + bytesReceived, imageSize - bytesReceived, 0);
        if (result <= 0) {
            cout << "Receive failed." << endl;
            delete[] imageData;
            return;
        }
        bytesReceived += result;
    }

    // Lưu ảnh vào file
    ofstream outFile("received_image.jpg", ios::binary);
    outFile.write(imageData, imageSize);
    outFile.close();
}

void receiveVideo(int serverSocket, size_t fileSize) {
    std::ofstream outFile("received_video.mp4", std::ios::binary | std::ios::trunc);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }


    char buffer[CHUNK_SIZE];
    size_t totalBytesReceived = 0; // Tổng số byte đã nhận
    int bytesReceived;

    while (totalBytesReceived < fileSize) {
        // Nhận dữ liệu từ server, không vượt quá CHUNK_SIZE
        bytesReceived = recv(serverSocket, buffer, min(CHUNK_SIZE, fileSize - totalBytesReceived), 0);

        if (bytesReceived > 0) {
            // Ghi dữ liệu nhận được vào file
            outFile.write(buffer, bytesReceived);
            totalBytesReceived += bytesReceived;
        }
        else if (bytesReceived == 0) {
            // Kết nối đóng sớm hơn dự kiến
            std::cerr << "Connection closed before receiving full file.\n";
            break;
        }
        else {
            // Lỗi trong quá trình nhận
            std::cerr << "Error receiving data from server.\n";
            break;
        }
    }

    if (totalBytesReceived == fileSize) {
        std::cout << "Video received successfully. Total size: " << totalBytesReceived << " bytes.\n";
    }
    else {
        std::cerr << "File size mismatch. Expected: " << fileSize << " bytes, received: " << totalBytesReceived << " bytes.\n";
    }

    // Đóng file
    outFile.close();
}
