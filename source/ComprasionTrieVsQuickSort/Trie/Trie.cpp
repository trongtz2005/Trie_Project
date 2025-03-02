#include "Trie.h"

void insert(TrieNode* root, const string& word) {
    if (word.length() < 3) return;

    TrieNode* node = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (node->children[index] == nullptr) {
            node->children[index] = new TrieNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
}

TrieNode* searchPrefix(TrieNode* root, const string& prefix, int& trieComparisons) {
    TrieNode* node = root;
    for (char ch : prefix) {
        trieComparisons++; 
        int index = ch - 'a';
        if (node->children[index] == nullptr) {
            return nullptr;
        }
        node = node->children[index];
    }
    return node;
}

vector<string> autocompleteTrie(TrieNode* root, const string& prefix, int maxSuggestions, int& trieComparisons) {
    vector<string> suggestions;
    TrieNode* node = searchPrefix(root, prefix, trieComparisons);
    if (!node) {
        return suggestions;
    }

    function<void(TrieNode*, string)> collectSuggestions = [&](TrieNode* currNode, string currentWord) {
        if (maxSuggestions <= 0) return;
        if (currNode->isEndOfWord) {
            trieComparisons++; 
            suggestions.push_back(currentWord);
            maxSuggestions--;  
        }
        for (int i = 0; i < ALPHABET_SIZE && maxSuggestions > 0; ++i) {
            trieComparisons++;
            if (currNode->children[i]) {
                collectSuggestions(currNode->children[i], currentWord + char(i + 'a'));
            }
        }
        };
    collectSuggestions(node, prefix);
    return suggestions;
}

void loadDictionary(TrieNode* root, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Khong the mo file: " << filename << endl;
        return;
    }

    string word;
    while (file >> word) {
        insert(root, word);
    }
    file.close();
}


string randomPrefix(TrieNode* root, int maxLength) {
    string prefix = "";
    TrieNode* current = root;

    for (int i = 0; i < maxLength; ++i) {
        vector<int> validIndices;

        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            if (current->children[j] != nullptr) {
                validIndices.push_back(j);
            }
        }

        if (validIndices.empty()) break; 
        int randomIndex = validIndices[rand() % validIndices.size()];
        prefix += char('a' + randomIndex); 
        current = current->children[randomIndex];

        if (current->isEndOfWord) {
            break;
        }
    }
    while (prefix.length() < maxLength) {
        prefix += 'a' + (rand() % ALPHABET_SIZE);
    }
    return prefix;
}

