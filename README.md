# EMAIL_CONTROLLING

Đây là đồ án môn học Mạng máy tính tại Trường Đại học Khoa học Tự nhiên, thực hiện bởi nhóm 8.

Tên và mã số sinh viên:
23122003 - Nguyễn Văn Linh
23122022 - Trần Hoàng Gia Bảo
23122026 - Trần Chấn Hiệp

##  Video Demo & Thuyết trình

Email Controlling(https://img.youtube.com/vi/AFb7PMyPbHU/0.jpg)](https://www.youtube.com/watch?v=AFb7PMyPbHU)

##  Tổng quan dự án

Dự án sẽ gồm Server, Client
User sẽ thực hiện chương trình bằng cách gởi mail là yêu cầu cần thực hiện đến mail của Client, Client sẽ xử yêu cầu và gởi yêu cầu cho Server thông qua 
Socket và Server sẽ thực hiện yêu cầu rồi phản hồi về Client và Client sẽ gởi mail là kết quả về cho User
---

##  Cấu trúc Repository

- **/src**: Chứa toàn bộ mã nguồn của dự án.
- **/report**: Chứa file báo cáo chi tiết của đồ án.

---

## 🚀 Hướng dẫn cài đặt và sử dụng

Vào properties của solution
chọn  C/C++ -> General
copy đường link của file include và include\opencv2 vào additional include directories
chọn linker -> gerneral
copy đường lik của file opencv\build\x64\vc16\lib vào additional library directories
chọn input
chọn additional dependencies
dán này vô
opencv_world4100.lib
opencv_world4100d.lib
