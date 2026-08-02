# 🎓 Flash Card - Spaced Repetition Edition
## Architecture & Implementation Details

---

## 📐 System Architecture

```
┌─────────────────────────────────────────────────────┐
│                    MAIN.CPP                         │
│  - Entry point & main loop                          │
│  - Quản lý Topics & Decks                           │
└──────────────────┬──────────────────────────────────┘
                   │
        ┌──────────┼──────────┐
        ▼          ▼          ▼
   ┌────────┐ ┌────────┐ ┌──────────┐
   │ UI.CPP │ │FLASHC. │ │STORAGE.  │
   │        │ │        │ │          │
   │Menu    │ │SM-2    │ │I/O       │
   │Learn   │ │Algo    │ │Persist   │
   │Stats   │ │Queue   │ │JSON-like │
   └────────┘ └────────┘ └──────────┘
```

---

## 🔑 Core Components

### 1. **flashcard.h/cpp** - Data Structures & SM-2 Algorithm

#### Structures:
```cpp
ReviewRecord {
    repetition        // Số lần học (0 = chưa học)
    easeFactor        // EF factor (1.3 - 5.0)
    interval          // Ngày tới lần ôn kế tiếp
    nextReviewDate    // Timestamp lần ôn kế tiếp
}

Card {
    english           // Từ tiếng Anh
    vietnamese        // Nghĩa tiếng Việt
    review            // ReviewRecord
    difficulty        // 1-5 (người dùng đánh giá)
    
    needsReview()     // Kiểm tra có cần ôn không
    updateReview()    // Cập nhật sau khi học
}

Deck {
    name              // Tên bộ thẻ
    cards             // Vector<Card>
    getTodayReviewCards()    // Lấy thẻ cần ôn hôm nay
    getNewCards()            // Lấy thẻ chưa học
}
```

#### SM-2 Algorithm:
```cpp
SpacedRepetitionEngine::calculateNextReview(Card, quality)
{
    // Điều chỉnh EF (Ease Factor)
    newEF = EF + (0.1 - (5 - quality) * (...))
    
    // Tính interval:
    if quality < 3:
        reset: repetition = 0, interval = 0  (quên)
    else:
        if repetition == 0: interval = 1
        if repetition == 1: interval = 3
        else: interval = interval * newEF
        
    // Cập nhật nextReviewDate
    nextReviewDate = now + interval * 86400
}
```

---

### 2. **ui.h/cpp** - Console User Interface

#### Menu Structure:
```
Main Menu
├─ [1] Manage Decks
│   ├─ Add/Edit/Delete Decks
│   └─ View Cards
├─ [2] Learn (Spaced Repetition)
│   ├─ [1] Learn New Cards
│   │   └─ Hiện thẻ → Nhấn Enter → Lật → Đánh giá (0-5)
│   └─ [2] Review Due Cards
│       └─ Hiện thẻ cần ôn → Lật → Đánh giá
└─ [3] Statistics
    └─ Hiển thị % nhớ, số từ còn ôn, v.v.
```

#### Key Functions:
```cpp
displayReviewCard(card, idx, total)     // Hiển thị thẻ
learnNewCards(deck)                     // Học từ mới
reviewDueCards(deck)                    // Ôn từ cũ
displayStats(deck)                      // Thống kê
displayQualityPrompt()                  // Nhắc người dùng đánh giá
```

---

### 3. **storage.h/cpp** - Data Persistence

#### File Format (Simple Text):
```
DECK:Phrasal Verbs
DESCRIPTION:Description...
CREATED:1722598800
CARDS:5
---
ENGLISH:get up
VIETNAMESE:dậy, thức dậy
DIFFICULTY:3
REPETITION:2
EASEFACTOR:2.6
INTERVAL:3
NEXTREVIEW:1722685200
---
[next card...]
```

#### Functions:
```cpp
saveDeck(filename, deck)              // Lưu 1 bộ thẻ
loadDeck(filename, deck)              // Tải 1 bộ thẻ
saveAllDecks(directory, topics)       // Lưu tất cả
loadAllDecks(directory, topics)       // Tải tất cả
```

---

## 🎯 Learning Flow (Chi tiết)

### Scenario: Học từ "get up"

**Ngày 1:**
```
User chọn "Learn New Cards"
  ↓
Xem: "get up" (tiếng Anh)
  ↓
Nhấn Enter để lật
  ↓
Xem: "dậy, thức dậy" (tiếng Việt)
  ↓
Đánh giá: 4 (Dễ nhớ)
  ↓
SM-2 tính toán:
  - EF = 2.5 + (0.1 - 1 * 0.10) = 2.5
  - repetition = 1
  - interval = 1 ngày
  - nextReviewDate = 2026-08-03
```

**Ngày 3 (2 ngày sau):**
```
User chọn "Review Due Cards"
  ↓
Xem: "get up" lại
  ↓
Đánh giá: 5 (Rất dễ)
  ↓
SM-2 tính toán:
  - EF = 2.5 + (0.1 - 0 * (...)) = 2.6 (tăng)
  - repetition = 2
  - interval = 1 * 2.6 = 2.6 ≈ 3 ngày
  - nextReviewDate = 2026-08-06
```

**Ngày 6:**
```
Nhấn Review → xem "get up" lại
  ↓
Đánh giá: 4 (Dễ nhớ)
  ↓
- EF = 2.6 + (0.1 - 1 * 0.10) = 2.6
- repetition = 3
- interval = 3 * 2.6 = 7.8 ≈ 8 ngày
- nextReviewDate = 2026-08-14
```

**Nếu người dùng quên (đánh giá 0 hoặc 1):**
```
Đánh giá: 0 (Quên hết)
  ↓
SM-2: quality < 3
  - repetition = 0 (RESET)
  - interval = 0
  - nextReviewDate = today (xuất hiện lại lần tới khi chọn "Learn New")
```

---

## 💾 Data Storage Flow

```
User Learning Process
        ↓
    Card.updateReview()  (SM-2 calculation)
        ↓
    StorageManager.saveDeck()
        ↓
    File: Phrasal_Verbs.txt
        ↓
    (Lần tiếp theo)
    StorageManager.loadDeck()
        ↓
    Chương trình biết khi nào ôn lại từ
```

---

## 📊 SM-2 Parameters

```cpp
// Default values:
reviewRecord.repetition = 0        // Lần đầu chưa học
reviewRecord.easeFactor = 2.5      // EF trung bình (1.3-5.0)
reviewRecord.interval = 0          // Lần đầu: interval 0
reviewRecord.nextReviewDate = today

// Quality scores and their effects:
Quality 5 → EF += 0.1  (tăng nhiều) → interval dài
Quality 4 → EF += 0.0  (giữ nguyên) → interval bình thường
Quality 3 → EF += 0.0  (giữ nguyên) → interval bình thường
Quality 2 → EF -= 0.14 (giảm)      → interval ngắn
Quality 1 → EF -= 0.32 (giảm nhiều)
Quality 0 → RESET                   → học lại từ đầu
```

---

## 🚀 Execution Flow

```
main.cpp
  ↓
while (true):
  - displayMainMenu()
  - switch (user_choice)
  
  Case 1: Manage Decks
    ├─ FlashCardUI::manageDeck(deck)
    └─ (Not implemented yet)
  
  Case 2: Learn
    ├─ FlashCardUI::learnCards(deck)
    │  ├─ [1] learnNewCards()
    │  │   └─ getNewCards() → review each → updateReview()
    │  └─ [2] reviewDueCards()
    │      └─ getTodayReviewCards() → review each → updateReview()
    └─ StorageManager::saveDeck()  [optional]
  
  Case 3: Statistics
    └─ FlashCardUI::displayStats(deck)
  
  Case 0: Exit
    └─ break
```

---

## 📈 Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Load deck | O(n) | n = số thẻ |
| Save deck | O(n) | Ghi tất cả thẻ |
| Get due cards | O(n) | Duyệt tất cả thẻ |
| Update review | O(1) | Tính toán SM-2 |

---

## 🔄 Next Implementation Steps

### Phase 1: Core (✅ DONE)
- [x] Data structures (Card, Deck, ReviewRecord)
- [x] SM-2 algorithm implementation
- [x] Basic UI (learn, review, menu)
- [x] Storage (text-based)

### Phase 2: Features
- [ ] Add/Edit/Delete cards in UI
- [ ] Multiple choice mode
- [ ] CSV import
- [ ] Better statistics dashboard
- [ ] Pronunciation support

### Phase 3: Advanced
- [ ] Spaced Repetition history tracking
- [ ] Word difficulty analysis
- [ ] Leaderboard
- [ ] Network sync (cloud)
- [ ] Mobile app version

---

## 🧪 Testing Scenarios

### Test 1: New Card Learning
```
1. Load sample deck
2. Learn 5 new cards → rate quality 3-5
3. Verify: repetition > 0, interval > 0, nextReviewDate set
4. Save to file
5. Reload & verify data persisted
```

### Test 2: Spaced Repetition
```
1. Create card with known schedule
2. Mark nextReviewDate as today (simulate time passage)
3. getTodayReviewCards() should include it
4. Update with quality 5 → EF should increase
5. Update with quality 0 → repetition should reset
```

### Test 3: File I/O
```
1. Create deck with 10 cards
2. saveDeck("test.txt")
3. Create new deck
4. loadDeck("test.txt")
5. Verify all data matches
```

---

## 📝 Files Overview

| File | LOC | Purpose |
|------|-----|---------|
| flashcard.h | ~80 | Header: data structures |
| flashcard.cpp | ~90 | SM-2 algorithm & logic |
| ui.h | ~50 | UI function declarations |
| ui.cpp | ~350 | Console UI implementation |
| storage.h | ~50 | Storage interface |
| storage.cpp | ~200 | File I/O implementation |
| main.cpp | ~40 | Entry point & main loop |
| CMakeLists.txt | ~30 | Build configuration |

**Total: ~890 lines of C++ code**

---

## 🎓 Key Learning Concepts

1. **Spaced Repetition (SM-2)**: Optimal review scheduling
2. **Data Structures**: Vector, struct composition
3. **File I/O**: Text-based persistence
4. **State Management**: Tracking review history
5. **User Interaction**: Console menu system
6. **Algorithm Implementation**: Time-based scheduling

---

**Happy Learning! 🚀**
