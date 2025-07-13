#pragma once
#include "stdafx.h"
#include <filesystem>

const std::string SMTP_SERVER = "smtp://smtp.gmail.com:587";
const std::string FROM_EMAIL = "tchh2005@gmail.com";
const std::string PASSWORD = "qbwhdukvicfrxuej"; // mật khẩu ứng dụng của Gmail
const std::string TO_EMAIL = "abc.defg.010505@gmail.com";
const std::string SUBJECT = "";
const std::string FOLDER_PATH = "D:\\Computer_Network\\Read_Mail\\receiveemail\\receiveemail\\received_image.jpg"; // Đường dẫn thư mục hình ảnh

void receive_imp(SOCKET clientSocket);

std::string readFile(const std::string& filePath);

void sendEmail(const std::vector<std::string>& attachments);