#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <Arduino.h>
#include <vector>

class CommandHistory {

public:
    CommandHistory(size_t maxSize = 10);

    void    addCommand(const String& command);  // Add a command to history
    String  getPrevious();                      // Navigate history: previous
    String  getNext();                          // Navigate history: next
    String  getCurrent() const;                 // Get current command at cursor
    void    resetToEnd();                       // Reset to end of history
    bool    isEmpty() const;                    // Check if history is empty
    size_t  size() const;                       // Get history size
    void    clear();                            // Clear all history

private:
    std::vector<String> history;                // Command history storage
    int                 currentIndex;           // Current position in history
    size_t              maxHistorySize;         // Maximum number of commands to store

    void                trimHistory();          // Helper method to trim history
};

#endif
