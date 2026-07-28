# Flash Card - Chương Trình Học Từ Vựng Tiếng Anh

Ứng dụng console hỗ trợ học từ vựng tiếng Anh bằng phương pháp Flash Card, được xây dựng bằng C++ với các cấu trúc dữ liệu cơ bản.

## Tính năng

- **Quản lý chủ đề (Deck)** – Tạo, đổi tên, xóa các bộ thẻ theo chủ đề
- **Quản lý Flash Card** – Thêm, sửa, xóa thẻ (Tiếng Anh – Tiếng Việt)
- **Học từ vựng** – Duyệt qua các thẻ để ghi nhớ
- **Kiểm tra (Quiz)** – Viết lại nghĩa từ vựng, ghi nhận kết quả
- **Ôn tập** – Các từ chưa nhớ được đưa lại hàng đợi để ôn lại
- **Lịch sử kiểm tra** – Xem lại kết quả các lần kiểm tra trước

## Cấu trúc dữ liệu sử dụng

| # | Cấu trúc | Mục đích |
|---|-----------|----------|
| 1 | Danh sách liên kết đôi (Doubly Linked List) | Lưu trữ Flash Card trong mỗi chủ đề | Mục xem từ vựng
| 2 | Cây nhị phân tìm kiếm (BST) | Quản lý danh sách chủ đề theo thứ tự |
| 3 | Hàng đợi (Queue) | Xử lý học và ôn tập Flash Card (FIFO) |
| 4 | Danh sách liên kết đơn (Singly Linked List) | Lưu lịch sử kiểm tra |

## Cấu trúc file

```
Flash-Card/
├── main.cpp          # Hàm main, khởi tạo chương trình
├── flashcard.h       # Khai báo cấu trúc dữ liệu & hàm xử lý
├── flashcard.cpp     # Cài đặt các hàm xử lý dữ liệu & file I/O
├── ui.h              # Khai báo các hàm giao diện console
├── ui.cpp            # Cài đặt giao diện, menu, hiển thị
├── Topics.txt        # Danh sách chủ đề (tự động tạo)
├── History.txt       # Lịch sử kiểm tra (tự động tạo)
└── README.md
```

**File dữ liệu sinh ra khi sử dụng:**
- `<Tên chủ đề>.txt` – Flash Card của chủ đề
- `Wrong_<Tên chủ đề>.txt` – Các từ chưa nhớ / làm sai
