#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <memory>

// Spaced Repetition - SM-2 Algorithm
struct ReviewRecord {
    int repetition;         // Số lần học (1, 2, 3, ...)
    double easeFactor;      // EF (1.3 - 5.0), mặc định 2.5
    int interval;           // Khoảng cách ngày để ôn lại
    time_t lastReviewDate;  // Lần cuối ôn tập
    time_t nextReviewDate;  // Khi nào cần ôn lại
    
    ReviewRecord() 
        : repetition(0), easeFactor(2.5), interval(0), 
          lastReviewDate(0), nextReviewDate(time(nullptr)) {}
};

// Flash Card
struct Card {
    std::string english;
    std::string vietnamese;
    ReviewRecord review;
    int difficulty;         // 1-5 (5 = rất dễ, 1 = rất khó) - người dùng đánh giá
    
    Card(const std::string& en = "", const std::string& vi = "") 
        : english(en), vietnamese(vi), difficulty(3) {}
    
    // Kiểm tra xem thẻ có cần ôn tập hôm nay không
    bool needsReview() const;
    
    // Cập nhật review record sau khi học (SM-2 algorithm)
    void updateReview(int quality);  // quality: 0-5 (0=quên, 5=rất dễ)
};

// Deck (Bộ thẻ)
struct Deck {
    std::string name;
    std::string description;
    std::vector<Card> cards;
    time_t createdDate;
    
    Deck(const std::string& n = "") 
        : name(n), createdDate(time(nullptr)) {}
    
    // Lấy danh sách thẻ cần ôn hôm nay (theo SR)
    std::vector<int> getTodayReviewCards() const;
    
    // Lấy danh sách thẻ mới chưa học
    std::vector<int> getNewCards() const;
};

// Topic (Chủ đề) - quản lý nhiều deck
struct Topic {
    std::string name;
    std::vector<Deck> decks;
    
    Topic(const std::string& n = "") : name(n) {}
};

// Spaced Repetition Engine
class SpacedRepetitionEngine {
public:
    // Tính toán interval tiếp theo theo SM-2 algorithm
    static void calculateNextReview(Card& card, int quality);
    
    // Quality: 0-5
    // 5: Rất dễ (ngay lần đầu nhớ)
    // 4: Dễ nhớ
    // 3: Trung bình (nhớ nhưng hơi lâu)
    // 2: Khó nhớ (dùng rất nhiều công sức)
    // 1: Rất khó (sai hoàn toàn)
    // 0: Quên hết
    
private:
    static const double MIN_EF;
    static const double MAX_EF;
};

#endif
