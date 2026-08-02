#include "flashcard.h"
#include <cmath>
#include <algorithm>

const double SpacedRepetitionEngine::MIN_EF = 1.3;
const double SpacedRepetitionEngine::MAX_EF = 5.0;

// ============ Card Implementation ============

bool Card::needsReview() const {
    if (review.repetition == 0) return true;  // Thẻ chưa học
    return review.nextReviewDate <= time(nullptr);
}

void Card::updateReview(int quality) {
    SpacedRepetitionEngine::calculateNextReview(*this, quality);
}

// ============ Deck Implementation ============

std::vector<int> Deck::getTodayReviewCards() const {
    std::vector<int> result;
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i].needsReview()) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> Deck::getNewCards() const {
    std::vector<int> result;
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i].review.repetition == 0) {
            result.push_back(i);
        }
    }
    return result;
}

// ============ Spaced Repetition Engine ============

void SpacedRepetitionEngine::calculateNextReview(Card& card, int quality) {
    // Đảm bảo quality trong range 0-5
    quality = std::max(0, std::min(5, quality));
    
    ReviewRecord& r = card.review;
    
    // SM-2 Algorithm
    // EF' = EF + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02))
    double newEF = r.easeFactor + (0.1 - (5.0 - quality) * (0.08 + (5.0 - quality) * 0.02));
    r.easeFactor = std::max(MIN_EF, newEF);
    
    if (quality < 3) {
        // Nếu chất lượng trả lời < 3, reset lại
        r.repetition = 0;
        r.interval = 0;
    } else {
        // Tăng số lần học
        if (r.repetition == 0) {
            r.interval = 1;  // Lần đầu: ôn lại sau 1 ngày
        } else if (r.repetition == 1) {
            r.interval = 3;  // Lần 2: ôn lại sau 3 ngày
        } else {
            // Từ lần 3 trở đi: interval = interval * EF
            r.interval = static_cast<int>(r.interval * r.easeFactor);
        }
        r.repetition++;
    }
    
    // Cập nhật thời gian
    r.lastReviewDate = time(nullptr);
    r.nextReviewDate = r.lastReviewDate + (r.interval * 86400);  // 86400 giây = 1 ngày
}
