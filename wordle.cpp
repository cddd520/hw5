#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
    std::string& current, 
    std::string floating, 
    const std::set<std::string>& dict, 
    std::set<std::string>& results, 
    size_t index);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    std::string current = in;
    wordleHelper(current, floating, dict, results, 0);
    return results;
}

// Define any helper functions here
void wordleHelper(
    std::string& current, 
    std::string floating, 
    const std::set<std::string>& dict, 
    std::set<std::string>& results, 
    size_t index)
{
    // Pruning: if we have more floating characters than spots left, we can't satisfy constraints
    size_t empty_spots = 0;
    for(size_t i = index; i < current.size(); i++) {
        if(current[i] == '-') empty_spots++;
    }
    if(floating.size() > empty_spots) {
        return;
    }

    if (index == current.size()) {
        if (floating.empty() && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }

    if (current[index] != '-') {
        wordleHelper(current, floating, dict, results, index + 1);
    } else {
        for (char c = 'a'; c <= 'z'; c++) {
            current[index] = c;
            std::string next_floating = floating;
            size_t pos = next_floating.find(c);
            if (pos != std::string::npos) {
                next_floating.erase(pos, 1);
            }
            wordleHelper(current, next_floating, dict, results, index + 1);
        }
        current[index] = '-'; // Backtrack
    }
}
