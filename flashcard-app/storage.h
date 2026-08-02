#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <map>
#include "flashcard.h"

// Simple JSON-like storage for persistence
class StorageManager {
public:
    // Save deck to file
    static bool saveDeck(const std::string& filename, const Deck& deck);
    
    // Load deck from file
    static bool loadDeck(const std::string& filename, Deck& deck);
    
    // Save all decks (multiple topics)
    static bool saveAllDecks(const std::string& directory, 
                            const std::vector<Topic>& topics);
    
    // Load all decks
    static bool loadAllDecks(const std::string& directory, 
                            std::vector<Topic>& topics);
    
private:
    // Helper methods to convert to/from simple JSON format
    static std::string cardToJson(const Card& card);
    static Card jsonToCard(const std::string& json);
    static std::string reviewToJson(const ReviewRecord& review);
    static ReviewRecord jsonToReview(const std::string& json);
};

#endif
