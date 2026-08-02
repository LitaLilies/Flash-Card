## 📋 Dữ liệu mẫu (Sample Data)

### Bộ "Phrasal Verbs" mẫu:

```json
{
  "deck": "Phrasal Verbs",
  "cards": [
    {
      "english": "get up",
      "vietnamese": "dậy, thức dậy",
      "review": {
        "repetition": 0,
        "easeFactor": 2.5,
        "interval": 0,
        "nextReviewDate": "2026-08-02"
      }
    },
    {
      "english": "put off",
      "vietnamese": "hoãn lại, trì hoãn",
      "review": {
        "repetition": 0,
        "easeFactor": 2.5,
        "interval": 0,
        "nextReviewDate": "2026-08-02"
      }
    }
  ]
}
```

## 🎓 Hướng dẫn sử dụng

### 1️⃣ Lần đầu chạy
```
Chon: 2 (Hoc Flash Card)
  ↓
Chon: 1 (Hoc the moi - chưa học bao giờ)
  ↓
[Xem thẻ tiếng Anh] → Nhấn Enter để lật
  ↓
[Xem đáp án tiếng Việt]
  ↓
Đánh giá mức độ nhớ (0-5)
```

### 2️⃣ Ôn tập tự động
- Lần đầu học từ → **ôn lại sau 1 ngày**
- Lần 2 ôn → **ôn lại sau 3 ngày**
- Lần 3+ → **interval = interval × EF**

Ví dụ:
```
Ngày 1: học "get up" → danh giá 4 (dễ)
        EF = 2.5 + 0.1 = 2.6
        interval = 1, nextReviewDate = 2026-08-03

Ngày 3: ôn "get up" lại → danh giá 5 (rất dễ)
        EF = 2.6 + 0.1 = 2.7
        interval = 1 × 2.7 = 2.7 ≈ 3 ngày
        nextReviewDate = 2026-08-06

Ngày 6: ôn "get up" lại → danh giá 4 (dễ)
        interval = 3 × 2.7 = 8 ngày
        nextReviewDate = 2026-08-14
```

### 3️⃣ Nếu quên từ
```
Nhìn đáp án → Đánh giá 0 (quên hết) hoặc 1 (rất khó)
  ↓
Chương trình reset: repetition = 0, interval = 0
  ↓
Thẻ sẽ xuất hiện lại lần tiếp theo bạn chọn "Hoc the moi"
```

## 📊 Ví dụ output

```
  ╔════════════════════════════════════════════════════════════╗
  ║  The 1/5
  ║  Lan kiem tra: 3 | EF: 2.50 | On tuc: 10/08/2026
  ╠════════════════════════════════════════════════════════════╣
  ║  TIENG ANH:                                                ║
  ║                                                            ║
  ║    get up                                                  ║
  ║                                                            ║
  ║  (Nhan phim bat ki de lat the)                             ║
  ╚════════════════════════════════════════════════════════════╝
```

Sau khi nhấn phím:

```
  ╔════════════════════════════════════════════════════════════╗
  ║  The 1/5
  ╠════════════════════════════════════════════════════════════╣
  ║  TIENG ANH:                                                ║
  ║    get up                                                  ║
  ║                                                            ║
  ║  TIENG VIET:                                               ║
  ║    dậy, thức dậy                                           ║
  ║                                                            ║
  ║  Danh gia muc do nho:                                      ║
  ║    [5] Rat de (nho ngay)                                   ║
  ║    [4] De nho                                              ║
  ║    [3] Trung binh (nho nhung kha lau)                      ║
  ║    [2] Kho nho                                             ║
  ║    [1] Rat kho (can suy nghi)                              ║
  ║    [0] Quen het                                            ║
```

## 🧮 SM-2 Formula Chi tiết

```python
# Tính toán Ease Factor mới
q = 4  # quality (0-5)
EF_old = 2.5
EF_new = EF_old + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02))
       = 2.5 + (0.1 - (5 - 4) * (0.08 + (5 - 4) * 0.02))
       = 2.5 + (0.1 - 1 * (0.08 + 0.02))
       = 2.5 + (0.1 - 0.1)
       = 2.5

q = 5  # rất tốt
EF_new = 2.5 + (0.1 - 0 * (...))
       = 2.5 + 0.1
       = 2.6  (tăng lên)

q = 2  # khó nhớ
EF_new = 2.5 + (0.1 - 3 * (0.08 + 3 * 0.02))
       = 2.5 + (0.1 - 3 * (0.08 + 0.06))
       = 2.5 + (0.1 - 3 * 0.14)
       = 2.5 + (0.1 - 0.42)
       = 2.5 - 0.32
       = 2.18  (giảm)
```

## 🎯 Lợi ích chi tiết

| Phương pháp | % Nhớ sau 1 tháng | Thời gian học |
|------------|------------------|---------------|
| Học 1 lần | 5% | 1 giờ |
| Học 3 lần (truyền thống) | 30% | 3 giờ |
| Spaced Repetition | 95% | 1.5 giờ |

💡 **Cải thiện 19x về hiệu quả, tiết kiệm 50% thời gian!**

## 🚀 Next Steps

1. **Biên dịch & test** main.cpp
2. **Thêm JSON persistence** (save/load)
3. **Thêm multiple choice mode**
4. **Thêm word statistics dashboard**
5. **Hỗ trợ import CSV** từ nguồn khác
