#include "CommandHistory.h"

// Constructor
CommandHistory::CommandHistory(size_t maxSize) 
    : currentIndex(-1), maxHistorySize(maxSize) {
    history.reserve(maxHistorySize);
}

// Add a command to history
void CommandHistory::addCommand(const String& command) {
    // Don't add empty commands or duplicates of the last command
    if (command.length() == 0 || (!history.empty() && history.back() == command)) {
        resetToEnd();
        return;
    }
    
    history.push_back(command);
    trimHistory();
    resetToEnd();
}

// Get previous command in history
String CommandHistory::getPrevious() {
    if (history.empty()) {
        return "";
    }
    
    if (currentIndex <= 0) {
        currentIndex = 0;
    } else {
        currentIndex--;
    }
    
    return history[currentIndex];
}

// Get next command in history
String CommandHistory::getNext() {
    if (history.empty()) {
        return "";
    }
    
    if (currentIndex >= (int)history.size() - 1) {
        currentIndex = history.size();
        return ""; // Return empty string when at end
    } else {
        currentIndex++;
        return history[currentIndex];
    }
}

// Get current command at cursor
String CommandHistory::getCurrent() const {
    if (history.empty() || currentIndex < 0 || currentIndex >= (int)history.size()) {
        return "";
    }
    return history[currentIndex];
}

// Reset to end of history (after last command)
void CommandHistory::resetToEnd() {
    currentIndex = history.size();
}

// Check if history is empty
bool CommandHistory::isEmpty() const {
    return history.empty();
}

// Get history size
size_t CommandHistory::size() const {
    return history.size();
}

// Clear all history
void CommandHistory::clear() {
    history.clear();
    currentIndex = -1;
}

// Trim history to maximum size
void CommandHistory::trimHistory() {
    while (history.size() > maxHistorySize) {
        history.erase(history.begin());
    }
}
