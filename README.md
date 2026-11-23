# Khoảng khuất

Ứng dụng C++17 dựng lại cảnh một người đi sau thùng hàng. Hai góc nhìn dùng chung dữ liệu mô phỏng ngoại tuyến.

## Chạy bản desktop

Mở `khoang-khuat.pro` trong Qt Creator, chọn bộ công cụ **Qt 6 Desktop** rồi nhấn **Run**. Ứng dụng mở cửa sổ riêng.

Trên Linux, với trình biên dịch C++17, Make và gói phát triển Qt 6 Widgets (Debian có gói `qt6-base-dev`), có thể build từ thư mục gốc của repo bằng các lệnh sau.

```sh
mkdir -p build
cd build
qmake6 ../khoang-khuat.pro
make
./khoang-khuat
```

## Điều khiển

| Phím | Chức năng |
| --- | --- |
| `1`, `2`, `3` | Chọn Tùng, Mực hoặc đối chiếu hai góc nhìn |
| `Space` | Phát hoặc tạm dừng |
| `,` và `.` | Lùi hoặc tiến một khung hình |

Kéo thanh thời gian để tua. Nhấn **Về đầu** để quay lại khung đầu tiên.
