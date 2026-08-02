#include "storage.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <ctime>

namespace fs = std::filesystem;

// Helper: Convert Card to JSON string
std::string StorageManager::cardToJson(const Card& card) {
    std::stringstream ss;
    ss << "{"
       << "\"english\":\"" << card.english << "\","
       << "\"vietnamese\":\"" << card.vietnamese << "\","
       << "\"difficulty\":" << card.difficulty << ","
       << "\"review\":" << reviewToJson(card.review)
       << "}";
    return ss.str();
}

// Helper: Convert review record to JSON
std::string StorageManager::reviewToJson(const ReviewRecord& review) {
    std::stringstream ss;
    ss << "{"
       << "\"repetition\":" << review.repetition << ","
       << "\"easeFactor\":" << review.easeFactor << ","
       << "\"interval\":" << review.interval << ","
       << "\"nextReviewDate\":" << review.nextReviewDate
       << "}";
    return ss.str();
}

// Save deck to file (simple text format for now)
bool StorageManager::saveDeck(const std::string& filename, const Deck& deck) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return false;
    }
    
    // Write header
    file << "DECK:" << deck.name << "\n";
    file << "DESCRIPTION:" << deck.description << "\n";
    file << "CREATED:" << deck.createdDate << "\n";
    file << "CARDS:" << deck.cards.size() << "\n";
    file << "---\n";
    
    // Write each card
    for (const auto& card : deck.cards) {
        file << "ENGLISH:" << card.english << "\n";
        file << "VIETNAMESE:" << card.vietnamese << "\n";
        file << "DIFFICULTY:" << card.difficulty << "\n";
        file << "REPETITION:" << card.review.repetition << "\n";
        file << "EASEFACTOR:" << card.review.easeFactor << "\n";
        file << "INTERVAL:" << card.review.interval << "\n";
        file << "NEXTREVIEW:" << card.review.nextReviewDate << "\n";
        file << "---\n";
    }
    
    file.close();
    return true;
}

// Load deck from file
bool StorageManager::loadDeck(const std::string& filename, Deck& deck) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return false;
    }
    
    std::string line;
    std::vector<Card> cards;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        if (line.substr(0, 5) == "DECK:") {
            deck.name = line.substr(5);
        } else if (line.substr(0, 12) == "DESCRIPTION:") {
            deck.description = line.substr(12);
        } else if (line.substr(0, 8) == "CREATED:") {
            deck.createdDate = std::stol(line.substr(8));
        } else if (line.substr(0, 8) == "ENGLISH:") {
            Card card;
            card.english = line.substr(8);
            
            // Read remaining fields
            while (std::getline(file, line) && line != "---") {
                if (line.substr(0, 11) == "VIETNAMESE:") {
                    card.vietnamese = line.substr(11);
                } else if (line.substr(0, 11) == "DIFFICULTY:") {
                    card.difficulty = std::stoi(line.substr(11));
                } else if (line.substr(0, 11) == "REPETITION:") {
                    card.review.repetition = std::stoi(line.substr(11));
                } else if (line.substr(0, 11) == "EASEFACTOR:") {
                    card.review.easeFactor = std::stod(line.substr(11));
                } else if (line.substr(0, 9) == "INTERVAL:") {
                    card.review.interval = std::stoi(line.substr(9));
                } else if (line.substr(0, 11) == "NEXTREVIEW:") {
                    card.review.nextReviewDate = std::stol(line.substr(11));
                }
            }
            
            cards.push_back(card);
        }
    }
    
    deck.cards = cards;
    file.close();
    return true;
}

// Save all decks to directory
bool StorageManager::saveAllDecks(const std::string& directory, 
                                  const std::vector<Topic>& topics) {
    try {
        fs::create_directories(directory);
    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << e.what() << "\n";
        return false;
    }
    
    for (const auto& topic : topics) {
        for (const auto& deck : topic.decks) {
            std::string filename = directory + "/" + deck.name + ".txt";
            if (!saveDeck(filename, deck)) {
                return false;
            }
        }
    }
    
    return true;
}

// Load all decks from directory
bool StorageManager::loadAllDecks(const std::string& directory, 
                                  std::vector<Topic>& topics) {
    try {
        if (!fs::exists(directory)) {
            std::cout << "Directory not found: " << directory << "\n";
            return false;
        }
        
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                Deck deck;
                if (loadDeck(entry.path().string(), deck)) {
                    // Add to topics (simplify: one topic = all decks)
                    if (topics.empty()) {
                        topics.push_back(Topic("My Decks"));
                    }
                    topics[0].decks.push_back(deck);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading decks: " << e.what() << "\n";
        return false;
    }
    
    return !topics[0].decks.empty();
}
