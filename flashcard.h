#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>
using namespace std;

// =====================================================================
//  4. SINGLY LINKED LIST - Lich su kiem tra
// =====================================================================
struct HistoryNode
{
    string date;
    string topic;
    int correct;
    int total;
    HistoryNode *next;
};

struct HistoryList
{
    HistoryNode *head;
    int count;
};

void initHistory(HistoryList &list);
// Them vao dau (moi nhat o dau)
void addHistory(HistoryList &list, const string &date, const string &topic, int correct, int total);
// Xoa 1 muc theo STT (1-based)
void deleteHistory(HistoryList &list, int index);
// Xoa toan bo lich su
void clearHistory(HistoryList &list);
void freeHistory(HistoryList &list);

// =====================================================================
//  5. FILE I/O
// =====================================================================
void loadTopics(const string &filename, TopicNode *&root);
void saveTopics(const string &filename, TopicNode *root);

void loadCards(const string &filename, CardList &list);
void saveCards(const string &filename, CardList &list);
