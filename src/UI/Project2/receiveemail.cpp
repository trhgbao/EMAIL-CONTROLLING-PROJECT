#include "stdafx.h"

using namespace std;
using namespace EAGetMailObjLib;

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

        // Mở file .txt để ghi
        fstream outFile;
        outFile.open(outputFilePath, ios::out);
        if (!outFile.is_open()) {
            _tprintf(_T("Không thể mở file để ghi: %s\n"), outputFilePath);
            return;
        }

        // Ghi thông tin email vào file
        outFile << (const char*)oMail->From->Address << endl;
        outFile << (const char*)oMail->Subject << endl;
        outFile << (const char*)oMail->TextBody << endl;

        // Ghi thông tin các tệp đính kèm
        IAttachmentCollectionPtr attachments = oMail->AttachmentList;
        if (attachments->Count > 0) {
            outFile << "Attachments:" << endl;
            for (long i = 0; i < attachments->Count; i++) {
                IAttachmentPtr pAtt = attachments->GetItem(i);
                outFile << "\t- " << (const char*)pAtt->Name << endl;
            }
        }
        else {
            outFile << "No attachments." << endl;
        }

        // Đóng file
        outFile.close();

        _tprintf(_T("Thông tin email đã được lưu vào file: %s\n"), outputFilePath);
    }
    catch (_com_error& ep) {
        _tprintf(_T("Error: %s\r\n"), (const TCHAR*)ep.Description());
    }
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
        oServer->Password = _T("qbwhdukvicfrxuej");         // Thay bằng mật khẩu ứng dụng của bạn
        oServer->Protocol = MailServerImap4;

        // Thiết lập kết nối SSL cho IMAP
        oServer->SSLConnection = VARIANT_TRUE;
        oServer->Port = 993;

        IMailClientPtr oClient = NULL;
        oClient.CreateInstance(__uuidof(EAGetMailObjLib::MailClient));
        oClient->LicenseCode = _T("TryIt");

        oClient->Connect(oServer);
        _tprintf(_T("Connected\r\n"));

        IMailInfoCollectionPtr infos = oClient->GetMailInfoList();
        _tprintf(_T("Total %d emails\r\n"), infos->Count);

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