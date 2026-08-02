#include <iostream>
#include "flashcard.h"
#include "ui.h"

int main() {
    // Tạo sample deck để test
    Deck sampleDeck("Phrasal Verbs");
    sampleDeck.cards.push_back(Card("get up", "dậy, thức dậy"));
    sampleDeck.cards.push_back(Card("put off", "hoãn lại, trì hoãn"));
    sampleDeck.cards.push_back(Card("look forward to", "mong chờ, trông đợi"));
    sampleDeck.cards.push_back(Card("break down", "hỏng, phá vỡ"));
    sampleDeck.cards.push_back(Card("give up", "từ bỏ, thôi"));
    
    // Main loop
    while (true) {
        FlashCardUI::displayMainMenu();
        int choice = FlashCardUI::getIntInput("  Chon: ", 0, 3);
        
        if (choice == 0) {
            std::cout << "\nTam biet!\n";
            break;
        } else if (choice == 1) {
            FlashCardUI::manageDeck(sampleDeck);
        } else if (choice == 2) {
            FlashCardUI::learnCards(sampleDeck);
        } else if (choice == 3) {
            FlashCardUI::displayStats(sampleDeck);
        }
    }
    
    return 0;
}
