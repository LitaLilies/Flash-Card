#include "ui.h"
#include <iostream>
#include <iomanip>
#include <ctime>

void FlashCardUI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void FlashCardUI::pause() {
    std::cout << "\nNhan phim bat ki de tiep tuc...";
    std::cin.ignore();
    std::cin.ignore();
}

std::string FlashCardUI::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int FlashCardUI::getIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            std::cin.ignore();
            return value;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Nhap khong hop le! Vui long nhap lai (" << min << "-" << max << ")\n";
    }
}

void FlashCardUI::displayMainMenu() {
    clearScreen();
    std::cout << "\n";
    std::cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║                        FLASH CARD                          ║\n";
    std::cout << "  ║               Chuong Trinh Hoc Tu Vung Tieng Anh           ║\n";
    std::cout << "  ║                 (voi Spaced Repetition)                    ║\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║   [1]  Quan ly Deck                                        ║\n";
    std::cout << "  ║   [2]  Hoc Flash Card (Spaced Repetition)                  ║\n";
    std::cout << "  ║   [3]  Xem thong ke                                        ║\n";
    std::cout << "  ║   [0]  Thoat                                               ║\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n";
}

void FlashCardUI::displayDeckMenu() {
    std::cout << "\n  [1]  Them Deck moi\n";
    std::cout << "  [2]  Xem danh sach Deck\n";
    std::cout << "  [3]  Sua/Xoa Deck\n";
    std::cout << "  [0]  Quay lai\n";
}

void FlashCardUI::displayLearnMenu() {
    std::cout << "\n  [1]  Hoc the moi (chua hoc bao gio)\n";
    std::cout << "  [2]  On tap the da hoc (Spaced Repetition)\n";
    std::cout << "  [3]  On tap ca (the moi + the can on)\n";
    std::cout << "  [0]  Quay lai\n";
}

void FlashCardUI::displayQualityPrompt() {
    std::cout << "\n  Danh gia muc do nho:\n";
    std::cout << "    [5] Rat de (nho ngay)\n";
    std::cout << "    [4] De nho\n";
    std::cout << "    [3] Trung binh (nho nhung kha lau)\n";
    std::cout << "    [2] Kho nho\n";
    std::cout << "    [1] Rat kho (can suy nghi)\n";
    std::cout << "    [0] Quen het\n";
    std::cout << "  Chon: ";
}

void FlashCardUI::displayReviewCard(const Card& card, int cardIndex, int total) {
    clearScreen();
    std::cout << "\n  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║  The " << cardIndex << "/" << total << "\n";
    
    // Hiển thị thông tin next review
    if (card.review.repetition > 0) {
        time_t nextReview = card.review.nextReviewDate;
        struct tm* timeinfo = localtime(&nextReview);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
        
        std::cout << "  ║  Lan kiem tra: " << card.review.repetition 
                  << " | EF: " << std::fixed << std::setprecision(2) 
                  << card.review.easeFactor 
                  << " | On tuc: " << buffer << "\n";
    }
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  TIENG ANH:                                                ║\n";
    std::cout << "  ║                                                            ║\n";
    std::cout << "  ║    " << std::left << std::setw(56) << card.english << "║\n";
    std::cout << "  ║                                                            ║\n";
    std::cout << "  ║  (Nhan phim bat ki de lat the)                             ║\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n";
}

void FlashCardUI::learnNewCards(Deck& deck) {
    std::vector<int> newCards = deck.getNewCards();
    
    if (newCards.empty()) {
        std::cout << "\nKhong con the moi!\n";
        pause();
        return;
    }
    
    std::cout << "\nCo " << newCards.size() << " the moi. Bat dau hoc...\n";
    pause();
    
    for (size_t i = 0; i < newCards.size(); ++i) {
        Card& card = deck.cards[newCards[i]];
        
        displayReviewCard(card, i + 1, newCards.size());
        std::cin.ignore();  // Chờ user lật thẻ
        
        // Hiển thị đáp án
        clearScreen();
        std::cout << "\n  ╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "  ║  The " << i + 1 << "/" << newCards.size() << "\n";
        std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "  ║  TIENG ANH:                                                ║\n";
        std::cout << "  ║    " << std::left << std::setw(56) << card.english << "║\n";
        std::cout << "  ║                                                            ║\n";
        std::cout << "  ║  TIENG VIET:                                               ║\n";
        std::cout << "  ║    " << std::left << std::setw(56) << card.vietnamese << "║\n";
        std::cout << "  ║                                                            ║\n";
        std::cout << "  ╚════════════════════════════════════════════════════════════╝\n";
        
        displayQualityPrompt();
        int quality = getIntInput("", 0, 5);
        
        card.updateReview(quality);
        std::cout << "Da cap nhat! Lan kem theo: " << card.review.nextReviewDate << "\n";
    }
    
    std::cout << "\n✓ Hoan thanh hoc " << newCards.size() << " the moi!\n";
    pause();
}

void FlashCardUI::reviewDueCards(Deck& deck) {
    std::vector<int> dueCards = deck.getTodayReviewCards();
    
    if (dueCards.empty()) {
        std::cout << "\nKhong con the can on tap hom nay! Chuc mung!\n";
        pause();
        return;
    }
    
    std::cout << "\nCo " << dueCards.size() << " the can on tap hom nay...\n";
    pause();
    
    for (size_t i = 0; i < dueCards.size(); ++i) {
        Card& card = deck.cards[dueCards[i]];
        
        displayReviewCard(card, i + 1, dueCards.size());
        std::cin.ignore();
        
        clearScreen();
        std::cout << "\n  ╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "  ║  The " << i + 1 << "/" << dueCards.size() << "\n";
        std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "  ║  TIENG ANH:                                                ║\n";
        std::cout << "  ║    " << std::left << std::setw(56) << card.english << "║\n";
        std::cout << "  ║                                                            ║\n";
        std::cout << "  ║  TIENG VIET:                                               ║\n";
        std::cout << "  ║    " << std::left << std::setw(56) << card.vietnamese << "║\n";
        std::cout << "  ║                                                            ║\n";
        std::cout << "  ╚════════════════════════════════════════════════════════════╝\n";
        
        displayQualityPrompt();
        int quality = getIntInput("", 0, 5);
        
        card.updateReview(quality);
    }
    
    std::cout << "\n✓ Hoan thanh on tap " << dueCards.size() << " the!\n";
    pause();
}

void FlashCardUI::learnCards(Deck& deck) {
    if (deck.cards.empty()) {
        std::cout << "\nKhong co the trong Deck nay!\n";
        pause();
        return;
    }
    
    while (true) {
        clearScreen();
        displayLearnMenu();
        int choice = getIntInput("  Chon: ", 0, 3);
        
        if (choice == 1) {
            learnNewCards(deck);
        } else if (choice == 2) {
            reviewDueCards(deck);
        } else if (choice == 3) {
            std::cout << "\n[Tinh nang nay se duoc them sau]\n";
            pause();
        } else {
            break;
        }
    }
}

void FlashCardUI::displayStats(const Deck& deck) {
    clearScreen();
    std::cout << "\n  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║  THONG KE - " << deck.name << "\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    
    int total = deck.cards.size();
    int learned = 0;
    int dueReview = 0;
    
    for (const auto& card : deck.cards) {
        if (card.review.repetition > 0) {
            learned++;
            if (card.needsReview()) dueReview++;
        }
    }
    
    std::cout << "  ║  Tong so the: " << total << "\n";
    std::cout << "  ║  Da hoc: " << learned << "\n";
    std::cout << "  ║  Con can on: " << dueReview << "\n";
    std::cout << "  ║  Chua hoc: " << (total - learned) << "\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n";
    
    pause();
}

void FlashCardUI::manageDeck(Deck& deck) {
    std::cout << "\n[Tinh nang quan ly Deck se duoc them sau]\n";
    pause();
}

void FlashCardUI::addCard(Deck& deck) {
    std::cout << "\n[Tinh nang them the se duoc them sau]\n";
    pause();
}

void FlashCardUI::editCard(Deck& deck) {
    std::cout << "\n[Tinh nang sua the se duoc them sau]\n";
    pause();
}

void FlashCardUI::deleteCard(Deck& deck) {
    std::cout << "\n[Tinh nang xoa the se duoc them sau]\n";
    pause();
}

void FlashCardUI::viewCards(const Deck& deck) {
    std::cout << "\n[Tinh nang xem the se duoc them sau]\n";
    pause();
}
