# 🃏 Flash Card — Học Từ Vựng Tiếng Anh
> Ứng dụng console C++ • Cấu trúc dữ liệu & Giải thuật • Quick Sort + Merge Sort

---

## ✨ Chức năng

| # | Chức năng | Mô tả |
|---|---|---|
| 1 | 📚 Quản lý chủ đề | Tạo / đổi tên / xóa bộ thẻ |
| 2 | 🃏 Quản lý Flash Card | Thêm / sửa / xóa thẻ Anh–Việt |
| 3 | 🎓 Học Flash Card | Lật thẻ, đánh giá nhớ / chưa nhớ |
| 4 | 📝 Kiểm tra | Nhập nghĩa, chấm điểm tự động |
| 5 | 🔁 Ôn tập | Từ sai tự động đưa vào hàng ôn |
| 6 | 📊 Lịch sử | Xem kết quả các lần kiểm tra |
| 7 | ⚡ Sắp xếp thẻ | **Quick Sort** A→Z theo tên tiếng Anh |
| 8 | 🔀 Sắp xếp lịch sử | **Merge Sort** theo điểm % cao→thấp |
| 9 | 🎮 Trò chơi | Nối từ & Viết tiếng Anh |

---

## 🗂️ Cấu trúc dữ liệu

```
Doubly Linked List ──► Lưu Flash Card (duyệt tiến/lùi ←→)
BST                ──► Quản lý chủ đề (tự động A→Z, O(log n))
Queue (FIFO)       ──► Học & ôn tập (thẻ chưa nhớ → enqueue lại)
Singly Linked List ──► Lịch sử kiểm tra (thêm đầu O(1))
```

---

## ⚡ Thuật toán sắp xếp

### Quick Sort — Sắp xếp Flash Card A→Z
```
[Dog, Apple, Cat, Bird]  →  pivot = "Cat"
                         →  [Apple, Bird] | Cat | [Dog]
                         →  [Apple, Bird, Cat, Dog] ✓
```
- Kích hoạt: **Quản lý Flash Card → [5] Sắp xếp A→Z**
- Độ phức tạp: **O(n log n)** trung bình

### Merge Sort — Sắp xếp lịch sử theo điểm %
```
[80%, 45%, 90%, 30%]  →  chia đôi → gộp giảm dần
                      →  [90%, 80%, 45%, 30%] ✓
```
- Kích hoạt: **Lịch sử → [3] Sắp xếp theo điểm %**
- Độ phức tạp: **O(n log n)** mọi trường hợp, **ổn định (stable)**

---

## 🖥️ Giao diện

```
  ╔════════════════════════════════════════════════════════════╗
  ║                        FLASH CARD                          ║
  ║               Chuong Trinh Hoc Tu Vung Tieng Anh           ║
  ╠════════════════════════════════════════════════════════════╣
  ║   [1]  Quan ly chu de (Deck)                               ║
  ║   [2]  Hoc Flash Card                                      ║
  ║   [3]  Kiem tra                                            ║
  ║   [4]  On tap                                              ║
  ║   [5]  Lich su ket qua                                     ║
  ║   [6]  Tro choi Noi tu                                     ║
  ║   [7]  Tro choi Viet tieng Anh                             ║
  ║   [0]  Thoat                                               ║
  ╚════════════════════════════════════════════════════════════╝
```

---

## 🔨 Biên dịch & Chạy

```bash
# macOS / Linux
g++ -o flashcard main.cpp flashcard.cpp ui.cpp -std=c++17
./flashcard

# Windows (MinGW)
g++ -o flashcard.exe main.cpp flashcard.cpp ui.cpp -std=c++17
flashcard.exe

# Windows (MSVC)
cl /EHsc /utf-8 main.cpp flashcard.cpp ui.cpp
```

---

## 📁 Cấu trúc file

```
Flash-Card/
├── main.cpp            ← Khởi tạo, load data, gọi menu
├── flashcard.h         ← Khai báo struct + Quick/Merge Sort
├── flashcard.cpp       ← CTDL + Quick Sort + Merge Sort + I/O
├── ui.h                ← Khai báo hàm giao diện
├── ui.cpp              ← Giao diện console Unicode
├── Topics.txt          ← Danh sách chủ đề
├── *.txt               ← Flash card từng chủ đề
├── Wrong_*.txt         ← Từ chưa nhớ / làm sai
├── History.txt         ← Lịch sử kiểm tra
├── .gitignore
└── BAO_CAO.md          ← Báo cáo chi tiết
```

---

## 📋 Yêu cầu

- Compiler C++ hỗ trợ **C++17**
- macOS / Linux / Windows
