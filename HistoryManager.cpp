#include "HistoryManager.h"

void HistoryManager::executeAction(std::unique_ptr<Action> action) {
    if (currentIndex_ != static_cast<int>(history_.size()) - 1) {
        // If behind the latest action, remove all actions ahead
        history_.erase(history_.begin() + currentIndex_ + 1, history_.end());
    }
    action->doAction();
    history_.push_back(std::move(action));
    currentIndex_ = static_cast<int>(history_.size()) - 1;
}

void HistoryManager::undo() {
    if (canUndo()) {
        history_[currentIndex_--]->undoAction();
    }
}

void HistoryManager::redo() {
    if (canRedo()) {
        history_[++currentIndex_]->doAction();
    }
}

bool HistoryManager::canUndo() const {
    return currentIndex_ >= 0;
}

bool HistoryManager::canRedo() const {
    return currentIndex_ < static_cast<int>(history_.size()) - 1;
}
