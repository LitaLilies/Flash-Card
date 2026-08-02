# Flash Card - Spaced Repetition Edition

## 🎯 Tính năng chính

- **Spaced Repetition (SM-2 Algorithm)**: Học hiệu quả nhất
- **Flash Card**: Lật thẻ Anh-Việt
- **Đánh giá mức nhớ**: 0-5 để tính toán lịch ôn
- **Lịch ôn tập thông minh**: Chỉ học thẻ cần thiết
- **Thống kê tiến độ**: Xem mức độ nhớ từng từ

## 📖 Spaced Repetition là gì?

Thuật toán SM-2 (SuperMemo 2) giúp bạn:
- ✅ Nhớ từ lâu hơn
- ✅ Tối ưu hóa thời gian học
- ✅ Ôn tập đúng lúc (trước khi quên)

### Cách hoạt động:
1. Lần đầu học từ → schedule lại sau 1 ngày
2. Lần 2 ôn tập → schedule lại sau 3 ngày
3. Lần 3+ → interval = interval × EF (Ease Factor)
4. Nếu nhớ tốt (4-5) → EF tăng → interval dài hơn
5. Nếu nhớ kém (0-2) → reset về đầu

## 🚀 Biên dịch & Chạy

### Linux/macOS:
```bash
cd flashcard-app
mkdir build && cd build
cmake ..
make
./flashcard
```

### Windows (MinGW):
```cmd
cd flashcard-app
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
flashcard.exe
```

### Windows (MSVC):
```cmd
cd flashcard-app
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
Release\flashcard.exe
```

## 📂 Cấu trúc file

```
flashcard-app/
├── flashcard.h          ← Data structures (Card, Deck, SR Engine)
├── flashcard.cpp        ← SM-2 algorithm implementation
├── ui.h                 ← UI functions declaration
├── ui.cpp               ← Console UI (menu, learn mode)
├── main.cpp             ← Main entry point
├── CMakeLists.txt       ← Build configuration
└── README.md            ← This file
```

## 🔧 Cấu trúc dữ liệu

### ReviewRecord (Spaced Repetition tracking)
```cpp
repetition       // Số lần đã học (0 = chưa học, 1, 2, 3, ...)
easeFactor       // EF: 1.3 - 5.0 (mặc định 2.5)
interval         // Khoảng cách ngày tới ôn lại
lastReviewDate   // Timestamp lần cuối học
nextReviewDate   // Timestamp khi nên ôn lại
```

### Card (Flash card)
```cpp
english          // Từ tiếng Anh
vietnamese       // Nghĩa tiếng Việt
review           // ReviewRecord data
difficulty       // 1-5 (được người dùng đánh giá)
```

### Deck (Bộ thẻ)
```cpp
name             // Tên bộ thẻ (VD: "Phrasal Verbs")
description      // Mô tả
cards            // Vector chứa các Card
createdDate      // Ngày tạo
```

## ⚙️ Đánh giá mức độ nhớ (Quality Score)

Sau mỗi lần xem thẻ, bạn chọn từ 0-5:

- **5** ⭐⭐⭐⭐⭐ Rất dễ (nhớ ngay lần đầu)
- **4** ⭐⭐⭐⭐ Dễ nhớ
- **3** ⭐⭐⭐ Trung bình (nhớ nhưng hơi lâu)
- **2** ⭐⭐ Khó nhớ (cần suy nghĩ)
- **1** ⭐ Rất khó (sai hoàn toàn)
- **0** ❌ Quên hết

## 📊 SM-2 Formula

```
Ease Factor (EF) = max(1.3, EF + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02)))

Intervals:
  1st repetition:  1 ngày
  2nd repetition:  3 ngày
  3rd+ repetition: interval = interval × EF
```

## 🎮 Sử dụng

1. **Chọn [2] Hoc Flash Card**
2. **Chọn [1] để học thẻ mới** hoặc **[2] để ôn tập**
3. Xem thẻ tiếng Anh → Nhấn Enter để lật
4. Xem đáp án → Đánh giá mức nhớ (0-5)
5. Chương trình tự động tính kế tiếp ôn lại khi nào

## 🔄 Quy trình ôn tập tự động

- Mỗi ngày, chương trình chỉ hiển thị những thẻ cần ôn
- Nếu `nextReviewDate <= hôm nay` → thẻ xuất hiện
- Nếu quên từ → reset repetition = 0, interval = 0
- Nếu nhớ tốt → EF tăng, interval dài hơn

## 📈 Lợi ích của Spaced Repetition

Nghiên cứu chỉ ra:
- Nhớ được **95% từ vựng** so với 40% học thường xuyên
- Giảm thời gian học **70%** so với cách học truyền thống
- Từ nhớ được sẽ tồn tại lâu dài (long-term memory)

## 🚀 Tiếp theo (Planned Features)

- [ ] JSON persist (save/load decks)
- [ ] Multiple choice mode
- [ ] Word pronunciation guide
- [ ] Statistics dashboard
- [ ] Leaderboard & achievements
- [ ] Spaced Repetition history
- [ ] Import từ CSV
- [ ] Network multiplayer

## 📝 License

Miễn phí sử dụng cho mục đích học tập

---

**Hãy bắt đầu học với Spaced Repetition! 🚀**
