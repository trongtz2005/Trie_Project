#pragma once
#include "lib.h"

// Node trong Trie
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

// Cac ham quan li trie
void insert(TrieNode* root, const string& word);
TrieNode* searchPrefix(TrieNode* root, const string& prefix, int& trieComparisons);
void suggestWords(TrieNode* node,string& prefix, int maxSuggestions, int& trieComparisons);
vector<string> autocompleteTrie(TrieNode* root, const string& prefix, int maxSuggestions, int& trieComparisons);
void loadDictionary(TrieNode* root, const string& filename);
string randomPrefix(TrieNode* root, int maxLength);


