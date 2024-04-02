#pragma once
#include <vector>
#include <memory>
#include "Actions.h"

class HistoryManager {
public:
    void executeAction(std::unique_ptr<Action> action);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

private:
    std::vector<std::unique_ptr<Action>> history_;
    int currentIndex_ = -1;
};
