// Hard: Word Ladder (BFS)
// Given two words (beginWord and endWord), and a dictionary, find the shortest transformation sequence from beginWord to endWord.
#include <iostream>
#include <unordered_set>
#include <queue>
#include <string>
using namespace std;

int ladderLength(string beginWord, string endWord, unordered_set<string>& wordList) {
    // Check if endWord exists in wordList
    if (wordList.find(endWord) == wordList.end()) {
        return 0;
    }
    
    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    
    while (!q.empty()) {
        auto current = q.front(); 
        q.pop();
        string word = current.first;
        int len = current.second;
        
        if (word == endWord) {
            return len;
        }
        
        // Try all possible single character transformations
        for (int i = 0; i < word.size(); i++) {
            char original_char = word[i];
            for (char c = 'a'; c <= 'z'; c++) {
                if (c == original_char) continue;
                
                string next = word;
                next[i] = c;
                
                if (wordList.find(next) != wordList.end()) {
                    q.push({next, len + 1});
                    wordList.erase(next);
                }
            }
        }
    }
    
    return 0;
}

int main() {
    unordered_set<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};
    string beginWord = "hit", endWord = "cog";
    
    // Add "hit" to wordList if not present
    wordList.insert("hit");
    
    int result = ladderLength(beginWord, endWord, wordList);
    if (result > 0) {
        cout << "Shortest transformation length: " << result << endl;
    } else {
        cout << "No transformation sequence exists." << endl;
    }
    
    return 0;
}
