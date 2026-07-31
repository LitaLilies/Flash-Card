#include "flashcard.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <cctype>
#include <windows.h>

// =====================================================================
//  HISTORY - Singly Linked List
// =====================================================================

void initHistory(HistoryList &list)
{
    list.head = nullptr;
    list.count = 0;
}

void addHistory(HistoryList &list, const string &date, const string &topic, int correct, int total)
{
    HistoryNode *node = new HistoryNode;
    node->date = date;
    node->topic = topic;
    node->correct = correct;
    node->total = total;
    node->next = list.head;
    list.head = node;
    list.count++;
}

void deleteHistory(HistoryList &list, int index)
{
    if (index < 1 || index > list.count)
        return;

    if (index == 1)
    {
        HistoryNode *temp = list.head;
        list.head = list.head->next;
        delete temp;
    }
    else
    {
        HistoryNode *prev = list.head;
        for (int i = 1; i < index - 1; i++)
            prev = prev->next;
        HistoryNode *temp = prev->next;
        prev->next = temp->next;
        delete temp;
    }
    list.count--;
}
void clearHistory(HistoryList &list)
{
    freeHistory(list);
}

void freeHistory(HistoryList &list)
{
    HistoryNode *curr = list.head;
    while (curr)
    {
        HistoryNode *next = curr->next;
        delete curr;
        curr = next;
    }
    list.head = nullptr;
    list.count = 0;
}

// =====================================================================
//  FILE I/O
// =====================================================================

string getCardFileName(const string &topicName)
{
    return topicName + ".txt";
}

string getWrongFileName(const string &topicName)
{
    return "Wrong_" + topicName + ".txt";
}

// --- Topics ---

static void saveTopicsHelper(ofstream &f, TopicNode *root)
{
    if (root == nullptr)
        return;
    saveTopicsHelper(f, root->left);
    f << root->name << "\n";
    saveTopicsHelper(f, root->right);
}
