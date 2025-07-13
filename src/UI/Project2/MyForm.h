#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <msclr/marshal_cppstd.h>

namespace Project2 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Diagnostics;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
        }
    public: void UpdateOutput(String^ text) {
        if (this->InvokeRequired) {
            this->Invoke(gcnew Action<String^>(this, &MyForm::UpdateOutput), text);
        }
        else {
            this->outputTextBox->Text += "\r\n" + text;
            // Tự động cuộn xuống dòng mới nhất

        }
    }
    public: bool AuthenticateUser(String^ username, String^ password) {
        // Danh sách tài khoản hợp lệ
        System::Collections::Generic::Dictionary<String^, String^>^ validUsers =
            gcnew System::Collections::Generic::Dictionary<String^, String^>();

        validUsers->Add("tchh2005@gmail.com", "vvfeeiqtitcbhvzk");

        // Kiểm tra tài khoản có tồn tại trong danh sách không
        if (validUsers->ContainsKey(username)) {
            return validUsers[username] == password; // So khớp mật khẩu
        }

        return false; // Nếu không tìm thấy hoặc mật khẩu sai
    }
    public: void run() {
        try {
            // Tạo ProcessStartInfo với các cấu hình cụ thể
            ProcessStartInfo^ startInfo = gcnew ProcessStartInfo();

            // Đường dẫn đến client.exe
            startInfo->FileName = System::IO::Path::Combine("D:\\baooo\\test\\Client\\x64\\Debug", "receiveemail.exe");

            // Thiết lập để đọc dữ liệu từ console của process
            startInfo->RedirectStandardOutput = true;
            startInfo->RedirectStandardError = true;
            startInfo->UseShellExecute = false;
            startInfo->CreateNoWindow = true;

            // Khởi tạo process
            Process^ clientProcess = gcnew Process();
            clientProcess->StartInfo = startInfo;

            // Bắt đầu xử lý sự kiện nhận dữ liệu từ console
            clientProcess->OutputDataReceived += gcnew DataReceivedEventHandler(this, &MyForm::ProcessOutputHandler);
            clientProcess->ErrorDataReceived += gcnew DataReceivedEventHandler(this, &MyForm::ProcessErrorHandler);

            // Bắt đầu process
            clientProcess->Start();

            // Bắt đầu đọc luồng dữ liệu
            clientProcess->BeginOutputReadLine();
            clientProcess->BeginErrorReadLine();

            this->outputTextBox->Text += "Client dang chay\r\n";
        }
        catch (Exception^ ex) {
            this->outputTextBox->Text += "Erorr" + ex->Message + "\r\n";
        }
    }
    public: void updateIP() {
        System::Collections::Generic::List<String^>^ validIps = gcnew System::Collections::Generic::List<String^>();
        std::fstream f;
        std::string line;
        std::vector<std::string> lines;

        // Mở file để đọc và lưu tất cả các dòng vào vector
        f.open("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt", std::ios::in);
        while (getline(f, line)) {
            lines.push_back(line);
        }
        f.close();

        // Xóa nội dung cũ của addipLabel
        this->addipLabel->Text = "";

        // Duyệt và in các phần tử từ đầu đến phần tử gần cuối
        for (size_t j = 0; j < lines.size() - 1; j++) {
            this->addipLabel->Text += gcnew String(lines[j].c_str()) + "\n";
        }
    }

    public: bool AuthenticateIp(String^ ip) {
        // Danh sách các IP hợp lệ
        System::Collections::Generic::List<String^>^ validIps =
            gcnew System::Collections::Generic::List<String^>();

        // Thêm các IP hợp lệ vào danh sách
        std::fstream f;
        std::string line;
        std::vector<std::string> lines; // Vector lưu các dòng của file
        f.open("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt", std::ios::in);
        while (getline(f, line)) {
            validIps->Add(msclr::interop::marshal_as<String^>(line));
            lines.push_back(line); // Lưu dòng vào vector
        }
        f.close();

        // Kiểm tra IP có tồn tại trong danh sách hay không
        int index = validIps->IndexOf(ip);
        if (index >= 0) {
            // IP tồn tại, cập nhật dòng cuối của file
            std::ofstream outFile("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt");
            if (!outFile.is_open()) {
                throw gcnew System::Exception("Unable to open the file for writing.");
            }

            // Ghi lại các dòng, thay dòng cuối bằng vị trí của IP
            for (size_t i = 0; i < lines.size() - 1; ++i) {
                outFile << lines[i] << std::endl;
            }
            outFile << index << std::endl; // Thay đổi dòng cuối bằng vị trí IP
            outFile.close();
        }

        return index >= 0; // Trả về true nếu IP tồn tại
    }


    public: bool user_server(int a) {
        std::fstream f;
        if (a == 1) {
            f.open("D:\\baooo\\test\\Client\\receiveemail\\user.txt", std::ios::in);
        }
        else {
            f.open("server.txt", std::ios::in);
        }

        // Mở file dựa trên tham số 'a'

        std::string line;
        String^ outputText = ""; // Tạo biến String^ để nối văn bản vào outputTextBox

        int i = 1;
        while (std::getline(f, line)) {
            // Thêm mỗi dòng vào outputText
            if (a == 1 && i == 1) {
                this->gm1->Text = gcnew String(line.c_str());
                i++;
            }
            else if (a == 1 && i == 2) {
                this->gm2->Text = gcnew String(line.c_str());
                i++;
            }
            else if (a == 1 && i == 3) {
                this->gm3->Text = gcnew String(line.c_str());
                i++;
            }
        }
        this->label_sv->Text = Convert::ToString(i - 1) + " gmail-user are allowed\n\n        Choose one:";

        f.close(); // Đóng file sau khi đọc xong
        return true;
    }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private: System::Windows::Forms::Label^ label;
    private: System::Windows::Forms::Label^ usernameLabel;
    private: System::Windows::Forms::Label^ passwordLabel;
    private: System::Windows::Forms::Label^ ipLabel;
    private: System::Windows::Forms::Label^ label_sv;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Label^ label2;
    private: System::Windows::Forms::Label^ credit_label;
    private: System::Windows::Forms::TextBox^ outputTextBox;
    private: System::Windows::Forms::Label^ addipLabel;
    private: System::Windows::Forms::TextBox^ usernameTextBox;
    private: System::Windows::Forms::TextBox^ passwordTextBox;
    private: System::Windows::Forms::TextBox^ ipTextBox;
    private: System::Windows::Forms::TextBox^ addipTextBox;
    private: System::Windows::Forms::TextBox^ gm1;
    private: System::Windows::Forms::TextBox^ gm2;
    private: System::Windows::Forms::TextBox^ gm3;
    private: System::Windows::Forms::Button^ acceptButton;
    private: System::Windows::Forms::Button^ loginButton;
    private: System::Windows::Forms::Button^ addipButton;
    private: System::Windows::Forms::Button^ add;
    private: System::Windows::Forms::Button^ del;
    private: System::Windows::Forms::Button^ addgmailButton;
    private: System::Windows::Forms::Button^ startButton;
    private: System::Windows::Forms::Button^ exitButton;
    private: System::Windows::Forms::Button^ gm1_b;
    private: System::Windows::Forms::Button^ gm2_b;
    private: System::Windows::Forms::Button^ gm3_b;
    private: System::Windows::Forms::Button^ returnButton;
    private: System::Windows::Forms::Button^ Credits;
    private: System::Windows::Forms::PictureBox^ logo;

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            //run();
            this->label = (gcnew System::Windows::Forms::Label());
            this->usernameLabel = (gcnew System::Windows::Forms::Label());
            this->passwordLabel = (gcnew System::Windows::Forms::Label());
            this->ipLabel = (gcnew System::Windows::Forms::Label());
            this->label_sv = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->credit_label = (gcnew System::Windows::Forms::Label());
            this->outputTextBox = (gcnew System::Windows::Forms::TextBox());
            this->addipLabel = (gcnew System::Windows::Forms::Label());
            this->usernameTextBox = (gcnew System::Windows::Forms::TextBox());
            this->passwordTextBox = (gcnew System::Windows::Forms::TextBox());
            this->ipTextBox = (gcnew System::Windows::Forms::TextBox());
            this->addipTextBox = (gcnew System::Windows::Forms::TextBox());
            this->gm1 = (gcnew System::Windows::Forms::TextBox());
            this->gm2 = (gcnew System::Windows::Forms::TextBox());
            this->gm3 = (gcnew System::Windows::Forms::TextBox());
            this->acceptButton = (gcnew System::Windows::Forms::Button());
            this->loginButton = (gcnew System::Windows::Forms::Button());
            this->startButton = (gcnew System::Windows::Forms::Button());
            this->exitButton = (gcnew System::Windows::Forms::Button());
            this->addipButton = (gcnew System::Windows::Forms::Button());
            this->add = (gcnew System::Windows::Forms::Button());
            this->del = (gcnew System::Windows::Forms::Button());
            this->addgmailButton = (gcnew System::Windows::Forms::Button());
            this->gm1_b = (gcnew System::Windows::Forms::Button());
            this->gm2_b = (gcnew System::Windows::Forms::Button());
            this->gm3_b = (gcnew System::Windows::Forms::Button());
            this->returnButton = (gcnew System::Windows::Forms::Button());
            this->Credits = (gcnew System::Windows::Forms::Button());
            this->logo = (gcnew System::Windows::Forms::PictureBox());
            this->BackColor = System::Drawing::Color::FromArgb(232, 231, 225);
            this->SuspendLayout();
            // 
            // label
            // 
            this->label->Location = System::Drawing::Point(100, 100);
            this->label->Name = L"label";
            this->label->Size = System::Drawing::Size(200, 30);
            // 
            // usernameLabel
            // 
            this->usernameLabel->AutoSize = true;
            this->usernameLabel->Font = (gcnew System::Drawing::Font(L"Times New Roman", 20, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->usernameLabel->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->usernameLabel->Location = System::Drawing::Point(30, 180);
            this->usernameLabel->Name = L"usernameLabel";
            this->usernameLabel->Size = System::Drawing::Size(86, 15);
            this->usernameLabel->Text = L"CLIENT-GMAIL:";
            // 
            // passwordLabel
            // 
            this->passwordLabel->AutoSize = true;
            this->passwordLabel->Font = (gcnew System::Drawing::Font(L"Times New Roman", 20, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->passwordLabel->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->passwordLabel->Location = System::Drawing::Point(30, 350);
            this->passwordLabel->Name = L"passwordLabel";
            this->passwordLabel->Size = System::Drawing::Size(101, 15);
            this->passwordLabel->Text = L"APP-PASSWORD:";
            // 
            // usernameTextBox
            // 
            this->usernameTextBox->Location = System::Drawing::Point(90, 240);
            this->usernameTextBox->Name = L"usernameTextBox";
            this->usernameTextBox->Size = System::Drawing::Size(700, 50);
            this->usernameTextBox->Font = gcnew System::Drawing::Font(L"Times New Roman", 20);
            this->usernameTextBox->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            // 
            // passwordTextBox
            // 
            this->passwordTextBox->Location = System::Drawing::Point(90, 410);
            this->passwordTextBox->Name = L"passwordTextBox";
            this->passwordTextBox->PasswordChar = '*';
            this->passwordTextBox->Size = System::Drawing::Size(700, 50);
            this->passwordTextBox->Font = gcnew System::Drawing::Font(L"Times New Roman", 20);
            this->passwordTextBox->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            // 
            // ipTextBox
            // 
            this->ipTextBox->Location = System::Drawing::Point(80, 260);
            this->ipTextBox->Name = L"ipTextBox";
            this->ipTextBox->Font = gcnew System::Drawing::Font(L"Times New Roman", 20);
            this->ipTextBox->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->ipTextBox->Size = System::Drawing::Size(700, 50);
            this->ipTextBox->Visible = false;
            // 
            // ipLabel
            // 
            this->ipLabel->AutoSize = true;
            this->ipLabel->Font = (gcnew System::Drawing::Font(L"Times New Roman", 24, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->ipLabel->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->ipLabel->Location = System::Drawing::Point(30, 180);
            this->ipLabel->Name = L"ipLabel";
            this->ipLabel->Size = System::Drawing::Size(86, 15);
            this->ipLabel->Visible = false;
            this->ipLabel->Text = L"SERVER-IP:";
            // 
            // acceptButton
            // 
            this->acceptButton->Location = System::Drawing::Point(540, 350);
            this->acceptButton->Name = L"acceptButton";
            this->acceptButton->Size = System::Drawing::Size(250, 80);
            this->acceptButton->Text = L"Accept";
            this->acceptButton->UseVisualStyleBackColor = true;
            this->acceptButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->acceptButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->acceptButton->Visible = false;
            this->acceptButton->Click += gcnew System::EventHandler(this, &MyForm::acceptButton_Click);
            // 
            // loginButton
            // 
            this->loginButton->Location = System::Drawing::Point(345, 500);
            this->loginButton->Name = L"loginButton";
            this->loginButton->Size = System::Drawing::Size(150, 70);
            this->loginButton->Text = L"Login";
            this->loginButton->UseVisualStyleBackColor = true;
            this->loginButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->loginButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->loginButton->Click += gcnew System::EventHandler(this, &MyForm::loginButton_Click);
            // 
            // startButton
            // 
            this->startButton->Location = System::Drawing::Point(30, 197);
            this->startButton->Name = L"startButton";
            this->startButton->Size = System::Drawing::Size(200, 100);
            this->startButton->Text = L"Start Program";
            this->startButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 22);
            this->startButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->startButton->UseVisualStyleBackColor = true;
            this->startButton->Visible = false;
            this->startButton->Click += gcnew System::EventHandler(this, &MyForm::startButton_Click);
            // 
            // addipButton
            // 
            this->addipButton->Location = System::Drawing::Point(90, 350);
            this->addipButton->Name = L"addipButton";
            this->addipButton->Size = System::Drawing::Size(280, 80);
            this->addipButton->Text = L"Add Ip";
            this->addipButton->UseVisualStyleBackColor = true;
            this->addipButton->Visible = false;
            this->addipButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 22);
            this->addipButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->addipButton->Click += gcnew System::EventHandler(this, &MyForm::addipButton_Click);
            // 
            // add
            // 
            this->add->Location = System::Drawing::Point(90, 200);
            this->add->Name = L"add";
            this->add->Size = System::Drawing::Size(280, 80);
            this->add->Text = L"Add";
            this->add->UseVisualStyleBackColor = true;
            this->add->Visible = false;
            this->add->Font = gcnew System::Drawing::Font(L"Times New Roman", 22);
            this->add->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->add->Click += gcnew System::EventHandler(this, &MyForm::addd_Click);
            // 
            // del
            // 
            this->del->Location = System::Drawing::Point(540, 200);
            this->del->Name = L"del";
            this->del->Size = System::Drawing::Size(280, 80);
            this->del->Text = L"Del";
            this->del->UseVisualStyleBackColor = true;
            this->del->Visible = false;
            this->del->Font = gcnew System::Drawing::Font(L"Times New Roman", 22);
            this->del->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->del->Click += gcnew System::EventHandler(this, &MyForm::del_Click);
            // 
            // addipTextBox
            // 
            this->addipTextBox->Location = System::Drawing::Point(275, 130);
            this->addipTextBox->Name = L"addipTextBox";
            this->addipTextBox->Font = gcnew System::Drawing::Font(L"Times New Roman", 20);
            this->addipTextBox->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->addipTextBox->Size = System::Drawing::Size(350, 50);
            this->addipTextBox->Visible = false;
            // 
            // addipLabel
            // 
            this->addipLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->addipLabel->Location = System::Drawing::Point(90, 320);
            this->addipLabel->Name = L"addipLabel";
            this->addipLabel->Size = System::Drawing::Size(730, 300);
            this->addipLabel->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->addipLabel->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->addipLabel->Visible = false;
            this->addipLabel->Text = L"";
            // 
            // addgmailButton
            // 
            this->addgmailButton->Location = System::Drawing::Point(30, 447);
            this->addgmailButton->Name = L"addgmailButton";
            this->addgmailButton->Size = System::Drawing::Size(200, 100);
            this->addgmailButton->Text = L"Add gmail";
            this->addgmailButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 22);
            this->addgmailButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->addgmailButton->UseVisualStyleBackColor = true;
            this->addgmailButton->Visible = false;
            //this->addgmailButton->Click += gcnew System::EventHandler(this, &MyForm::addgmailButton_Click);
            // 
            // gm1_b
            // 
            this->gm1_b->Location = System::Drawing::Point(680, 280);
            this->gm1_b->Name = L"gm1";
            this->gm1_b->Size = System::Drawing::Size(130, 50);
            this->gm1_b->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->gm1_b->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->gm1_b->Text = L"Choose";
            this->gm1_b->UseVisualStyleBackColor = true;
            this->gm1_b->Visible = false;
            this->gm1_b->Click += gcnew System::EventHandler(this, &MyForm::gm_Click);
            // 
            // gm2_b
            // 
            this->gm2_b->Location = System::Drawing::Point(680, 410);
            this->gm2_b->Name = L"gm2";
            this->gm2_b->Size = System::Drawing::Size(130, 50);
            this->gm2_b->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->gm2_b->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->gm2_b->Text = L"Choose";
            this->gm2_b->UseVisualStyleBackColor = true;
            this->gm2_b->Visible = false;
            this->gm2_b->Click += gcnew System::EventHandler(this, &MyForm::gm_Click);
            // 
            // gm3_b
            // 
            this->gm3_b->Location = System::Drawing::Point(680, 540);
            this->gm3_b->Name = L"gm3";
            this->gm3_b->Size = System::Drawing::Size(130, 50);
            this->gm3_b->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->gm3_b->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->gm3_b->Text = L"Choose";
            this->gm3_b->UseVisualStyleBackColor = true;
            this->gm3_b->Visible = false;
            this->gm3_b->Click += gcnew System::EventHandler(this, &MyForm::gm_Click);
            // 
            // returnButton
            // 
            this->returnButton->Location = System::Drawing::Point(50, 80);
            this->returnButton->Name = L"returnButton";
            this->returnButton->Size = System::Drawing::Size(130, 60);
            this->returnButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->returnButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->returnButton->Tag = 0;
            this->returnButton->Text = L"Return";
            this->returnButton->UseVisualStyleBackColor = true;
            this->returnButton->Visible = false;
            this->returnButton->Click += gcnew System::EventHandler(this, &MyForm::returnButton_Click);
            // 
            // gm1
            // 
            this->gm1->Location = System::Drawing::Point(100, 280);
            this->gm1->Name = L"gm1";
            this->gm1->Size = System::Drawing::Size(550, 100);
            this->gm1->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->gm1->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->gm1->TabIndex = 9;
            this->gm1->Visible = false;
            // 
            // gm2
            // 
            this->gm2->Location = System::Drawing::Point(100, 410);
            this->gm2->Name = L"gm2";
            this->gm2->Size = System::Drawing::Size(550, 100);
            this->gm2->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->gm2->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->gm2->Visible = false;
            // 
            // gm3
            // 
            this->gm3->Location = System::Drawing::Point(100, 540);
            this->gm3->Name = L"gm3";
            this->gm3->Size = System::Drawing::Size(550, 100);
            this->gm3->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->gm3->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->gm3->Visible = false;
            // 
            // exitButton
            // 
            this->exitButton->Location = System::Drawing::Point(30, 572);
            this->exitButton->Name = L"exitButton";
            this->exitButton->Size = System::Drawing::Size(200, 100);
            this->exitButton->Text = L"EXIT";
            this->exitButton->Font = gcnew System::Drawing::Font(L"Times New Roman", 22);
            this->exitButton->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->exitButton->UseVisualStyleBackColor = true;
            this->exitButton->Visible = false;
            this->exitButton->Click += gcnew System::EventHandler(this, &MyForm::exitButton_Click);
            // 
            // Credits
            // 
            this->Credits->Location = System::Drawing::Point(50, 500);
            this->Credits->Name = L"Credits";
            this->Credits->Size = System::Drawing::Size(150, 70);
            this->Credits->Text = L"Credits";
            this->Credits->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->Credits->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->Credits->UseVisualStyleBackColor = true;
            this->Credits->Visible = true;
            this->Credits->Click += gcnew System::EventHandler(this, &MyForm::Credits_Click);
            // 
            // credit_label
            // 
            this->credit_label->AutoSize = true;
            this->credit_label->Font = (gcnew System::Drawing::Font(L"Times New Roman", 26, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->credit_label->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->credit_label->Location = System::Drawing::Point(200, 250);
            this->credit_label->Name = L"credit_label";
            this->credit_label->Size = System::Drawing::Size(600, 500);
            this->credit_label->Visible = false;
            this->credit_label->Text = L"23122003   -   Nguyễn Văn Linh\n\n23122022   -   Trần Hoàng Gia Bảo\n\n23122026   -   Trần Chấn Hiệp \n\n GVHD: Đỗ Hoàng Cường";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 30, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->label1->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->label1->Location = System::Drawing::Point(164, 9);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(382, 31);
            this->label1->Text = L"WELCOME TO OUR GROUP";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->label2->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->label2->Location = System::Drawing::Point(265, 70);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(245, 23);
            this->label2->Text = L"ĐỒ ÁN MẠNG MÁY TÍNH";
            // 
            // outputTextBox
            // 
            this->outputTextBox = gcnew System::Windows::Forms::TextBox();
            this->outputTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->outputTextBox->Location = System::Drawing::Point(250, 197);
            this->outputTextBox->Name = L"outputTextBox";
            this->outputTextBox->Size = System::Drawing::Size(600, 475);
            this->outputTextBox->Font = gcnew System::Drawing::Font(L"Times New Roman", 25);
            this->outputTextBox->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->outputTextBox->BackColor = System::Drawing::Color::White;
            this->outputTextBox->Multiline = true; // Cho phép hiển thị nhiều dòng
            this->outputTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical; // Thêm thanh cuộn dọc
            this->outputTextBox->ReadOnly = true; // Chỉ cho phép đọc
            this->outputTextBox->Visible = false;
            this->outputTextBox->Text = L"Waiting for client messages...";
            // 
            // label_sv
            // 
            this->label_sv->AutoSize = true;
            this->label_sv->Font = (gcnew System::Drawing::Font(L"Times New Roman", 22, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->label_sv->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->label_sv->Location = System::Drawing::Point(40, 150);
            this->label_sv->Name = L"label_sv";
            this->label_sv->Size = System::Drawing::Size(700, 500);
            this->label_sv->Visible = false;
            // 
            // logo
            // 
            this->logo->AutoSize = true;
            this->logo->Image = Image::FromFile("D:\\baooo\\test\\UI\\logo.png");
            this->logo->ForeColor = System::Drawing::Color::FromArgb(83, 67, 52);
            this->logo->Location = System::Drawing::Point(670, 40);
            this->logo->Name = L"logo";
            this->logo->Width = 40;
            this->logo->Height = 30;
            this->logo->Visible = true;
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(901, 698);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->credit_label);
            this->Controls->Add(this->acceptButton);
            this->Controls->Add(this->loginButton);
            this->Controls->Add(this->startButton);
            this->Controls->Add(this->exitButton);
            this->Controls->Add(this->addipButton);
            this->Controls->Add(this->Credits);
            this->Controls->Add(this->add);
            this->Controls->Add(this->del);
            this->Controls->Add(this->addipLabel);
            this->Controls->Add(this->addgmailButton);
            this->Controls->Add(this->gm1);
            this->Controls->Add(this->gm2);
            this->Controls->Add(this->gm3);
            this->Controls->Add(this->gm1_b);
            this->Controls->Add(this->gm2_b);
            this->Controls->Add(this->gm3_b);
            this->Controls->Add(this->returnButton);
            this->Controls->Add(this->passwordTextBox);
            this->Controls->Add(this->passwordLabel);
            this->Controls->Add(this->usernameTextBox);
            this->Controls->Add(this->usernameLabel);
            this->Controls->Add(this->ipTextBox);
            this->Controls->Add(this->addipTextBox);
            this->Controls->Add(this->ipLabel);
            this->Controls->Add(this->label);
            this->Controls->Add(this->label_sv);
            this->Controls->Add(this->outputTextBox);
            this->Controls->Add(this->logo);
            this->Name = L"MyForm";
            this->Text = L"MyForm";
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private: System::Void loginButton_Click(System::Object^ sender, System::EventArgs^ e) {
        String^ username = this->usernameTextBox->Text;
        String^ password = this->passwordTextBox->Text;

        this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 34, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(163)));
        this->label2->Location = System::Drawing::Point(150, 20);
        this->label1->Visible = false;

        if (AuthenticateUser(username, password)) {

            this->label_sv->Visible = true;
            user_server(1);
            this->gm1->Visible = true;
            this->gm2->Visible = true;
            this->gm3->Visible = true;
            this->gm1_b->Visible = true;
            this->gm2_b->Visible = true;
            this->gm3_b->Visible = true;
            this->returnButton->Visible = true;
            this->returnButton->Tag = (int)(this->returnButton->Tag) + 1;

            // Ẩn các control đăng nhập
            this->usernameLabel->Visible = false;
            this->passwordLabel->Visible = false;
            this->usernameTextBox->Visible = false;
            this->passwordTextBox->Visible = false;
            this->loginButton->Visible = false;
            this->logo->Visible = false;
            this->Credits->Visible = false;

            Application::DoEvents();
        }
        else {
            System::Windows::Forms::DialogResult result =
                MessageBox::Show("Incorrect username or password. Please try again.", "Login Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            if (result == System::Windows::Forms::DialogResult::Yes) {
                System::Void loginButton_Click(System::Object ^ sender, System::EventArgs ^ e);
            }
        }
    }

    private: System::Void gm_Click(System::Object^ sender, System::EventArgs^ e) {
        System::Drawing::Point mousePos = this->PointToClient(System::Windows::Forms::Cursor::Position);
        int y = mousePos.Y;
        std::fstream f;
        f.open("D:\\baooo\\test\\Client\\receiveemail\\user.txt", std::ios::out);
        f << msclr::interop::marshal_as<std::string>(this->gm1->Text) + "\n";
        f << msclr::interop::marshal_as<std::string>(this->gm2->Text) + "\n";
        f << msclr::interop::marshal_as<std::string>(this->gm3->Text) + "\n";
        if (y >= 280 && y <= 330) {
            f << 1;
        }
        else if (y >= 410 && y <= 460) {
            f << 2;
        }
        else {
            f << 3;
        }
        f.close();

        this->returnButton->Tag = (int)(this->returnButton->Tag) + 1;

        this->label_sv->Visible = false;
        this->gm1->Visible = false;
        this->gm2->Visible = false;
        this->gm3->Visible = false;
        this->gm1_b->Visible = false;
        this->gm2_b->Visible = false;
        this->gm3_b->Visible = false;

        this->ipLabel->Visible = true;
        this->ipTextBox->Visible = true;
        this->acceptButton->Visible = true;
        this->addipButton->Visible = true;
        updateIP();
    }


    private: void ProcessOutputHandler(Object^ sender, DataReceivedEventArgs^ e) {
        try {
            if (!String::IsNullOrEmpty(e->Data)) {
                UpdateOutput(e->Data);
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error in output handler: " + ex->Message);
        }
    }

    private: void ProcessErrorHandler(Object^ sender, DataReceivedEventArgs^ e) {
        if (!String::IsNullOrEmpty(e->Data)) {
            // Cập nhật lỗi lên outputTextBox
            this->UpdateOutput("ERROR: " + e->Data);
        }
    }

    private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
    }
           
    private: System::Void acceptButton_Click(System::Object^ sender, System::EventArgs^ e) {
        String^ ip = this->ipTextBox->Text;
        this->returnButton->Tag = (int)(this->returnButton->Tag) + 1;
        if (AuthenticateIp(ip)) {
            // Hiển thị label output
            this->outputTextBox->Visible = true;
            this->outputTextBox->Text = "Wait...\r\n";

            this->ipLabel->Visible = false;
            this->ipTextBox->Visible = false;
            this->acceptButton->Visible = false;
            this->addipButton->Visible = false;

            this->startButton->Visible = true;
            this->exitButton->Visible = true;

        }
        else {
            System::Windows::Forms::DialogResult result =
                MessageBox::Show("Incorrect IP. Please try again.", "Login IP Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            if (result == System::Windows::Forms::DialogResult::Yes) {
                System::Void loginButton_Click(System::Object ^ sender, System::EventArgs ^ e);
            }
        }

    }
    private: System::Void startButton_Click(System::Object^ sender, System::EventArgs^ e) {
        try {
            // Tạo ProcessStartInfo với các cấu hình cụ thể
            ProcessStartInfo^ startInfo = gcnew ProcessStartInfo();

            this->outputTextBox->Text += "\n";

            // Đường dẫn đến client.exe
            startInfo->FileName = System::IO::Path::Combine("D:\\baooo\\test\\Client\\x64\\Debug", "receiveemail.exe");

            startInfo->WorkingDirectory = "D:\\baooo\\test\\Client\\receiveemail";

            startInfo->EnvironmentVariables["PATH"] += ";D:\\baooo\\test\\Client\\receiveemail\\curl\\libcurl_a_debug.lib;D:\\baooo\\test\\Client\\receiveemail\\curl\\curl\\libcurl_a.lib";

            // Thiết lập để đọc dữ liệu từ console của process
            startInfo->RedirectStandardOutput = true;
            startInfo->RedirectStandardError = true;
            startInfo->UseShellExecute = false;
            startInfo->CreateNoWindow = true;

            // Khởi tạo process
            Process^ clientProcess = gcnew Process();
            clientProcess->StartInfo = startInfo;

            // Bắt đầu xử lý sự kiện nhận dữ liệu từ console
            clientProcess->OutputDataReceived += gcnew DataReceivedEventHandler(this, &MyForm::ProcessOutputHandler);
            clientProcess->ErrorDataReceived += gcnew DataReceivedEventHandler(this, &MyForm::ProcessErrorHandler);

            // Bắt đầu process
            clientProcess->Start();

            // Bắt đầu đọc luồng dữ liệu
            clientProcess->BeginOutputReadLine();
            clientProcess->BeginErrorReadLine();

            //this->outputTextBox->Text += "Client dang chay\r\n";
        }
        catch (Exception^ ex) {
            this->outputTextBox->Text += "Erorr" + ex->Message + "\r\n";
        }
    }
    private: System::Void exitButton_Click(System::Object^ sender, System::EventArgs^ e) {
        // Hiển thị thông báo xác nhận trước khi thoát
        System::Windows::Forms::DialogResult result =
            MessageBox::Show("?",
                "Exit?",
                MessageBoxButtons::YesNo,
                MessageBoxIcon::Question);

        if (result == System::Windows::Forms::DialogResult::Yes) {
            Application::Exit(); // Thoát ứng dụng
        }
    }
    private: System::Void addipButton_Click(System::Object^ sender, System::EventArgs^ e) {

        this->returnButton->Tag = (int)(this->returnButton->Tag) + 2;
        this->ipLabel->Visible = false;
        this->ipTextBox->Visible = false;
        this->acceptButton->Visible = false;
        this->addipButton->Visible = false;

        this->add->Visible = true;
        this->del->Visible = true;
        this->addipTextBox->Visible = true;
        this->addipLabel->Visible = true;

    }
    private: System::Void returnButton_Click(System::Object^ sender, System::EventArgs^ e) {

        if ((int)this->returnButton->Tag == 3) {
            this->returnButton->Tag = (int)(this->returnButton->Tag) - 1;

            this->ipLabel->Visible = true;
            this->ipTextBox->Visible = true;
            this->acceptButton->Visible = true;
            this->addipButton->Visible = true;

            this->outputTextBox->Visible = false;
            this->startButton->Visible = false;
            this->exitButton->Visible = false;


        }
        else if ((int)this->returnButton->Tag == 2) {
            this->returnButton->Tag = (int)(this->returnButton->Tag) - 1;

            this->label_sv->Visible = true;
            this->gm1->Visible = true;
            this->gm2->Visible = true;
            this->gm3->Visible = true;
            this->gm1_b->Visible = true;
            this->gm2_b->Visible = true;
            this->gm3_b->Visible = true;

            this->ipLabel->Visible = false;
            this->ipTextBox->Visible = false;
            this->acceptButton->Visible = false;
            this->addipButton->Visible = false;

        }
        else if ((int)this->returnButton->Tag == 1) {
            this->returnButton->Tag = (int)(this->returnButton->Tag) - 1;

            this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->label2->Location = System::Drawing::Point(265, 70);
            this->label1->Visible = true;

            this->label_sv->Visible = false;
            this->gm1->Visible = false;
            this->gm2->Visible = false;
            this->gm3->Visible = false;
            this->gm1_b->Visible = false;
            this->gm2_b->Visible = false;
            this->gm3_b->Visible = false;
            this->returnButton->Visible = false;

            this->usernameLabel->Visible = true;
            this->passwordLabel->Visible = true;
            this->usernameTextBox->Visible = true;
            this->passwordTextBox->Visible = true;
            this->loginButton->Visible = true;
            this->logo->Visible = true;
            this->Credits->Visible = true;

        }
        else if ((int)this->returnButton->Tag == 4) {
            this->returnButton->Tag = (int)(this->returnButton->Tag) - 2;

            this->ipLabel->Visible = true;
            this->ipTextBox->Visible = true;
            this->acceptButton->Visible = true;
            this->addipButton->Visible = true;

            this->add->Visible = false;
            this->del->Visible = false;
            this->addipTextBox->Visible = false;
            this->addipLabel->Visible = false;
        }
        else if ((int)this->returnButton->Tag == 5) {
            this->returnButton->Tag = (int)(this->returnButton->Tag) - 5;

            this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(163)));
            this->label2->Location = System::Drawing::Point(265, 70);
            this->label1->Visible = true;

            this->returnButton->Visible = false;
            this->credit_label ->Visible = false;

            this->usernameLabel->Visible = true;
            this->passwordLabel->Visible = true;
            this->usernameTextBox->Visible = true;
            this->passwordTextBox->Visible = true;
            this->loginButton->Visible = true;
            this->Credits->Visible = true;
        }
    }
    private: System::Void addd_Click(System::Object^ sender, System::EventArgs^ e) {
    std::vector<std::string> parts;
    std::string part;
    std::fstream f;
    std::string line;

    // Lấy địa chỉ IP từ TextBox
    line = msclr::interop::marshal_as<std::string>(this->addipTextBox->Text);
    std::stringstream ss(line);

    // Phân tích địa chỉ IP thành các phần
    while (getline(ss, part, '.')) {
        parts.push_back(part);
    }

    // Kiểm tra tính hợp lệ của địa chỉ IP
    if (parts.size() != 4) {
        MessageBox::Show("Error: An IPv4 address must have exactly 4 parts separated by dots.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    for (const std::string& p : parts) {
        if (p.empty()) {
            MessageBox::Show("Error: Each part of the IP address must not be empty.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        for (char c : p) {
            if (!isdigit(c)) {
                MessageBox::Show("Error: Each part of the IP address must contain only digits.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
        }
        int num = stoi(p);
        if (num < 0 || num > 255 || (p.size() > 1 && p[0] == '0')) {
            MessageBox::Show("Error: Each part of the IP address must be in the range 0 to 255.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
    }

    // Đọc toàn bộ nội dung file vào vector
    std::vector<std::string> lines;
    f.open("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt", std::ios::in);
    while (getline(f, line)) {
        lines.push_back(line);
    }
    f.close();

    // Thêm địa chỉ IP vào vị trí gần cuối
    line = msclr::interop::marshal_as<std::string>(this->addipTextBox->Text);
    if (lines.size() > 1) {
        lines.insert(lines.end() - 1, line); // Chèn vào vị trí áp chót
    }
    else {
        lines.insert(lines.begin(), line); // Nếu file chỉ có 1 dòng, thêm vào cuối
    }

    // Ghi lại toàn bộ nội dung vào file
    f.open("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt", std::ios::out | std::ios::trunc);
    for (const auto& l : lines) {
        f << l << std::endl;
    }
    f.close();

    // Xóa TextBox và cập nhật hiển thị
    this->addipTextBox->Text = "";
    updateIP();
}
    private: System::Void del_Click(System::Object^ sender, System::EventArgs^ e) {
    std::ifstream infile("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt");
    std::vector<std::string> lines;
    std::string line;

    // Đọc tất cả các dòng vào vector
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    // Xóa dòng gần cuối (áp chót) nếu có đủ dòng
    if (lines.size() > 1) {
        lines.erase(lines.end() - 2); // Xóa phần tử thứ hai từ cuối
    }
    else if (lines.size() == 1) {
        return;
    }

    // Ghi lại các dòng còn lại vào file
    std::ofstream outfile("D:\\baooo\\test\\Client\\receiveemail\\server_ip.txt");
    for (const auto& l : lines) {
        outfile << l << std::endl;
    }
    outfile.close();

    // Reset TextBox và cập nhật IP
    this->addipTextBox->Text = "";
    updateIP();
}
    private: System::Void Credits_Click(System::Object^ sender, System::EventArgs^ e) {
        this->returnButton->Tag = (int)(this->returnButton->Tag) + 5;
        this->credit_label->Visible = true;
        this->returnButton->Visible = true;

        // Ẩn các control đăng nhập
        this->usernameLabel->Visible = false;
        this->passwordLabel->Visible = false;
        this->usernameTextBox->Visible = false;
        this->passwordTextBox->Visible = false;
        this->loginButton->Visible = false;
        this->Credits->Visible = false;
    }
    };
}