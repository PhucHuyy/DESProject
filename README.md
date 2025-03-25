# DES Encryption/Decryption qua Socket
# Dự án này sử dụng thuật toán DES để mã hóa file trên một máy (Laptop A) và gửi qua socket đến máy khác (Laptop B) để giải mã.

# Môi trường IDE
IDE: Visual Studio Code (VS Code)
Compiler: MinGW (g++)
Hệ điều hành: Windows
Thư viện mạng: Winsock2 (yêu cầu link với -lws2_32)

# Cách Compile File
# Trên Laptop A (Máy gửi)
Mở terminal trong VS Code hoặc Command Prompt tại thư mục dự án.
Compile file encrypt_and_send.cpp:
g++ -I ./include ./src/DES.cpp ./src/encrypt_and_send.cpp -o ./bin/Debug/encrypt_and_send.exe -lws2_32

# Trên Laptop B (Máy nhận)
Mở terminal trong VS Code hoặc Command Prompt tại thư mục dự án.
Compile file receive_and_decrypt.cpp (thay 192.168.1.x trong code bằng IP của Laptop A trước khi compile):
g++ -I ./include ./src/DES.cpp ./src/receive_and_decrypt.cpp -o ./bin/Debug/receive_and_decrypt.exe -lws2_32

# Cách Thực Thi
# Điều kiện tiên quyết
Cả hai máy phải kết nối chung một Wi-Fi.
Laptop A cần file sample.txt trong thư mục input.
Kiểm tra IP của Laptop A bằng lệnh ipconfig trong CMD.
Trên Laptop A (Máy gửi)
Chạy file thực thi:
.\bin\Debug\encrypt_and_send.exe
Đợi thông báo "Dang cho ket noi tu Laptop B...".

Trên Laptop B (Máy nhận)
Chạy file thực thi:
.\bin\Debug\receive_and_decrypt.exe
Kết quả: File giải mã được lưu tại output/decrypted.txt.

# Lưu ý
Chạy Laptop A trước, sau đó chạy Laptop B.
Đảm bảo tắt firewall hoặc mở port 12345 nếu kết nối không thành công.
Khóa DES trong encrypt_and_send.cpp và receive_and_decrypt.cpp phải giống nhau.
