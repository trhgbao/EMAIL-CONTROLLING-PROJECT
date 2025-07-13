#include "receiveemail.h"
#include "Take_Del.h"

std::string smtpServer = "smtp.gmail.com";
int port = 587;
std::string username = "tchh2005@gmail.com";
std::string password = "vvfeeiqtitcbhvzk";
std::string fromEmail = "tchh2005@gmail.com";
std::string message = "Hello, this is a test email with an image attachment sent from C++ using cURL!";
std::string imagePath = "D:\\baooo\\test\\Client\\receiveemail\\received_image.jpg";
std::string list_app_Path = "D:\\baooo\\test\\Client\\receiveemail\\list_app.txt";
std::string list_service_Path = "D:\\baooo\\test\\Client\\receiveemail\\list_service.txt";
std::string received_video = "D:\\baooo\\test\\Client\\receiveemail\\received_video.mp4";

#define CHUNK_SIZE 4096 // Kích thước mỗi chunk đọc từ file

using namespace std;
using namespace EAGetMailObjLib;

std::string base64_encode(const std::string& in) {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

DWORD _getCurrentPath(LPTSTR lpPath, DWORD nSize) {
    DWORD dwSize = ::GetModuleFileName(NULL, lpPath, nSize);
    if (dwSize == 0 || dwSize == nSize) {
        return 0;
    }

    // Đổi tên tệp để lấy đường dẫn hiện tại
    LPCTSTR psz = _tcsrchr(lpPath, _T('\\'));
    if (psz != NULL) {
        lpPath[psz - lpPath] = _T('\0');
        return _tcslen(lpPath);
    }
    return 0;
}
// Hàm phân tích tệp .eml
void SaveEmailToTxt(LPCTSTR lpszFile, const char* outputFilePath) {
    try {
        IMailPtr oMail = NULL;
        oMail.CreateInstance(__uuidof(EAGetMailObjLib::Mail));
        oMail->LicenseCode = _T("TryIt");

        // Tải nội dung từ tệp .eml
        oMail->LoadFile(lpszFile, VARIANT_FALSE);

        // Lấy thông tin người gửi (Chuyển đổi BSTR thành std::wstring)
        _bstr_t bstrSender(oMail->From->Address);
        std::wstring senderStr = (const wchar_t*)bstrSender;

        // Lấy tiêu đề email (Chuyển đổi BSTR thành std::wstring)
        _bstr_t bstrSubject(oMail->Subject);
        std::wstring subjectStr = (const wchar_t*)bstrSubject;

        // Lấy nội dung email (Chuyển đổi BSTR thành std::wstring)
        _bstr_t bstrBody(oMail->TextBody);
        std::wstring bodyStr = (const wchar_t*)bstrBody;

        // Mở file todo.txt để lưu thông tin với mã hóa UTF-8
        std::ofstream outFile(outputFilePath, std::ios::out | std::ios::trunc);

        if (outFile.is_open()) {
            // Ghi thông tin vào file (chuyển std::wstring sang std::string với mã hóa UTF-8)
            outFile << std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(senderStr) << "\n";
            outFile << std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(subjectStr) << "\n";
            outFile << std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(bodyStr) << "\n";

            outFile.close();  // Đóng file sau khi ghi
            _tprintf(_T("Email information saved to output.txt\n"));
        }
        else {
            _tprintf(_T("Unable to open todo.txt for writing.\n"));
        }
    }
    catch (_com_error& ep) {
        _tprintf(_T("Error: %s\r\n"), (const TCHAR*)ep.Description());
    }
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);  // Mở file theo kiểu nhị phân
    if (!file) {
        std::cerr << "Cannot open file: " << filePath << std::endl;  // Kiểm tra xem file có mở được không
        return "";
    }

    std::ostringstream oss;  // Đối tượng để lưu dữ liệu file
    oss << file.rdbuf();  // Đọc toàn bộ dữ liệu từ file vào đối tượng oss

    return oss.str();  // Trả về nội dung file dưới dạng chuỗi
}

size_t payload_source_(void* ptr, size_t size, size_t nmemb, void* userp) {
    const char** payload_text = static_cast<const char**>(userp);

    // Kiểm tra kết thúc
    if ((size == 0) || (nmemb == 0) || (*payload_text == nullptr)) {
        return 0;
    }

    // Xác định kích thước dữ liệu còn lại
    size_t len = strlen(*payload_text);
    size_t to_copy = size * nmemb < len ? size * nmemb : len;

    // Sao chép dữ liệu từ payload vào buffer
    memcpy(ptr, *payload_text, to_copy);

    // Cập nhật con trỏ dữ liệu
    *payload_text += to_copy;

    return to_copy;
}

wchar_t getWideChar() {
    fstream f;
    f.open("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt");

    return L'∑'; // Một ký tự Unicode ví dụ
}

bool checkaddress(string a) {

    fstream f;
    string line, last, l;
    int stt, i = 1;
    f.open("user.txt", ios::in);
    while (getline(f, line)) {
        last = line;
    }
    f.close();
    stt = stoi(last);

    f.open("user.txt", ios::in);
    while (i < 4) {
        getline(f, l);
        if (l == a && i == stt) {
            return true;
        }
        i++;
    }
    cout << "Wrong Gmail-User\nDone!" << endl;
    return false;
}

//Hàm gửi email kèm file ảnh
bool sendEmail() {
    fstream f;
    string toEmail, subject, Content, filename;
    bool ct = false;
    f.open("output.txt", ios::in);
    getline(f, toEmail);
    getline(f, subject);
    if (!checkaddress(toEmail)) {
        return false;
    }
    if (subject == "screenshot" || subject == "list_app" || subject == "list_service" || subject == "recording" || subject == "fetch") {
        ct = true;
    }
    CURL* curl;
    CURLcode res = CURLE_OK;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    struct curl_slist* recipients = nullptr;
    curl = curl_easy_init();

    // Đọc nội dung file ảnh
    if (subject == "screenshot") {
        Content = readFile(imagePath);

        if (Content.empty()) {
            return false;
        }

        filename = imagePath.substr(imagePath.find_last_of("/\\") + 1);
    } 
    else if (subject == "list_app") {
        Content = readFile(list_app_Path);

        if (Content.empty()) {
            return false;
        }

        filename = "list_app.txt";
    }
    else if (subject == "list_service") {
        Content = readFile(list_service_Path);

        if (Content.empty()) {
            return false;
        }

        filename = "list_service.txt";
    }
    else if (subject == "recording") {
        Content = readFile(received_video);;
        if (Content.empty()) {
            return false;
        }

        filename = "video.mp4";
    }
    else if (subject == "fetch") {
        Content = readFile(dir);;
        if (Content.empty()) {
            return false;
        }

        filename = dir;
    }

    // Tạo nội dung email với MIME header
    std::string payload = "To: " + toEmail + "\r\n";
    payload += "From: " + fromEmail + "\r\n";
    payload += "Subject: " + subject + "\r\n";
    payload += "MIME-Version: 1.0\r\n";
    payload += "Content-Type: multipart/mixed; boundary=\"boundary12345\"\r\n";
    payload += "\r\n";
    payload += "--boundary12345\r\n";
    payload += "Content-Type: text/plain\r\n\r\n";
    payload += message + "\r\n";
    payload += "--boundary12345\r\n";
    if (subject == "screenshot") {
        payload += "Content-Type: image/jpeg; name=\"" + filename + "\"\r\n";
    }
    else if (subject == "list_app") {
        payload += "Content-Type: text/plain; name=\"" + filename + "\"\r\n";
    }
    else if (subject == "recording") {
        payload += "Content-Type: video/mp4; name=\"" + filename + "\"\r\n";
    }
    else if (subject == "fetch") {
        payload += "Content-Type: application/octet-stream; name=\"" + dir + "\"\r\n";
    }
    payload += "Content-Transfer-Encoding: base64\r\n";
    payload += "Content-Disposition: attachment; filename=\"" + filename + "\"\r\n";
    payload += "\r\n";

    // Mã hóa nội dung ảnh sang Base64
    if (ct) {
        std::string encodedImage = base64_encode(Content);

        payload += encodedImage + "\r\n";
    }
    payload += "--boundary12345--\r\n";

    const char* payload_text = payload.c_str();
    // Cấu hình cURL cho SMTP
    curl_easy_setopt(curl, CURLOPT_URL, (smtpServer + ":" + std::to_string(port)).c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmail.c_str());

    // Thêm người nhận email
    recipients = curl_slist_append(recipients, toEmail.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    // Gửi dữ liệu email
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source_);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload_text);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);


    // Thực hiện gửi email
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    res = curl_easy_perform(curl);

    // Dọn dẹp
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    cout << "Done!";
    return true;
}



int run(int argc, _TCHAR* argv[]) {
    const int MailServerImap4 = 1;

    // Khởi tạo môi trường COM
    ::CoInitialize(NULL);

    // Tạo thư mục "inbox" để lưu email tải về
    TCHAR szPath[MAX_PATH + 1];
    _getCurrentPath(szPath, MAX_PATH);

    TCHAR szMailBox[MAX_PATH + 1];
    wsprintf(szMailBox, _T("%s\\inbox"), szPath);

    ::CreateDirectory(szMailBox, NULL);

    try {
        IMailServerPtr oServer = NULL;
        oServer.CreateInstance(__uuidof(EAGetMailObjLib::MailServer));

        // Sử dụng máy chủ IMAP của Gmail
        oServer->Server = _T("imap.gmail.com");
        oServer->User = _T("tchh2005@gmail.com");      // Thay bằng email của bạn
        oServer->Password = _T("vvfeeiqtitcbhvzk");         // Thay bằng mật khẩu ứng dụng của bạn
        oServer->Protocol = MailServerImap4;

        // Thiết lập kết nối SSL cho IMAP
        oServer->SSLConnection = VARIANT_TRUE;
        oServer->Port = 993;

        IMailClientPtr oClient = NULL;
        oClient.CreateInstance(__uuidof(EAGetMailObjLib::MailClient));
        oClient->LicenseCode = _T("TryIt");

        oClient->Connect(oServer);
        //_tprintf(_T("Connected\r\n"));

        IMailInfoCollectionPtr infos = oClient->GetMailInfoList();
        //_tprintf(_T("Total %d emails\r\n"), infos->Count);

        if (infos->Count > 0) {
            // Lấy email cuối cùng (sử dụng chỉ số Count - 1 để lấy email cuối cùng)
            IMailInfoPtr pInfo = infos->GetItem(infos->Count - 1);

            TCHAR szFile[MAX_PATH + 1];
            ::wsprintf(szFile, _T("%s\\last_email.eml"), szMailBox);  // Đặt tên là last_email.eml

            // Nhận email từ máy chủ IMAP
            IMailPtr oMail = oClient->GetMail(pInfo);

            // Lưu email vào ổ đĩa
            oMail->SaveAs(szFile, VARIANT_TRUE);

            // Phân tích email sau khi lưu
            SaveEmailToTxt(szFile, "output.txt");  // Đọc và phân tích tệp .eml
        }
        else {
            _tprintf(_T("No emails found.\r\n"));
        }

        oClient->Quit();  // Thoát phiên IMAP
    }
    catch (_com_error& ep) {
        _tprintf(_T("Error: %s\r\n"), (const TCHAR*)ep.Description());
    }

    // Dọn dẹp môi trường COM
    ::CoUninitialize();
    return 0;
}