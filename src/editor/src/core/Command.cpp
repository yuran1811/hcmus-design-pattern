#include "index.hpp"

CommandHistory::CommandHistory() {}

CommandHistory::~CommandHistory() {
  while (!undoStack.empty()) {
    delete undoStack.top();
    undoStack.pop();
  }

  while (!redoStack.empty()) {
    delete redoStack.top();
    redoStack.pop();
  }
}

void CommandHistory::push(Command* command) { undoStack.push(command); }

void CommandHistory::Undo(Editor& editor) {
  if (!undoStack.empty()) {
    Command* command = undoStack.top();
    command->Undo(editor);
    undoStack.pop();
    redoStack.push(command);
  }
}

void CommandHistory::Redo(Editor& editor) {
  if (!redoStack.empty()) {
    Command* command = redoStack.top();
    command->Execute(editor);
    redoStack.pop();
    undoStack.push(command);
  }
}
