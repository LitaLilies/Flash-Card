#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include "flashcard.h"

class FlashCardUI {
public:
    // Menu chính
    static void displayMainMenu();
    static void displayDeckMenu();
    static void displayLearnMenu();
    
    // Học thẻ với Spaced Repetition
    static void learnCards(Deck& deck);
    static void learnNewCards(Deck& deck);
    static void reviewDueCards(Deck& deck);
    
    // Quản lý thẻ
    static void manageDeck(Deck& deck);
    static void addCard(Deck& deck);
    static void editCard(Deck& deck);
    static void deleteCard(Deck& deck);
    static void viewCards(const Deck& deck);
    
    // Thống kê
    static void displayStats(const Deck& deck);
    
    // Utility
    static void clearScreen();
    static void pause();
    static std::string getInput(const std::string& prompt);
    static int getIntInput(const std::string& prompt, int min, int max);
    
private:
    static void displayReviewCard(const Card& card, int cardIndex, int total);
    static void displayQualityPrompt();
};

#endif
