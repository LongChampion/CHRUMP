# Giới thiệu
Đây là một tool nhỏ dùng để dump history, cookies và các password được lưu trong Google Chrome.  
Tool hiện tại chỉ chạy được trên Windows 10 và chỉ hỗ trợ trình duyệt Chrome.

# Cách tải xuống
Trước hết bạn cần tải xuống mã nguồn từ [GitLab](https://gitlab.com/) bằng lệnh sau:
> `git clone https://gitlab.com/LongChampion/chrump.git CHRUMP`

Tool có sử dụng thư viện [CryptoPP](https://github.com/weidai11/cryptopp) nên trước khi compile, bạn cần build library của CryptoPP.  
Cách làm như sau:  
1. Dùng termimal (hoặc command prompt) để vào trong thư mục `CryptoPP` của repository.
2. Dùng lệnh sau để tải mã nguồn của CryptoPP về:
    > `git submodule update --init --remote --recursive`
3. Dùng Visual Studio để mở solution có tên `cryptest.sln` trong thư mục `CryptoPP` vừa tải về.
4. Mở hộp thoại **Build --> Batch Build...**.
5. Chọn các target *`cryptlib: Debug, Win32`*, *`cryptlib: Release, Win32`*, *`cryptlib: Release, x64`* và *`cryptlib: Release, x64`* rồi bấm nút **Build**.
6. Sau khi việc build hoàn tất, bạn kiểm tra sẽ thấy trong thư mục CryptoPP có hai thư mục mới là **Win** và **x64**, đó làn nơi chứa những library cần thiết cho việc build tool sau này.

# Build
Solution đi kèm đã được điều chỉnh một số option và thêm những thư viện cần thiết như Crypt32.lib, WinSQLite3.lib và cryptolib.lib (được build ở bước trước), bạn có thể dùng Visual Studio để biên dịch tool ngay bây giờ :).
