#include "index.hpp"

Editor::Editor()
    : caret(Caret()), selectionStart(Coord()), selectionEnd(Coord()) {}

std::string Editor::GetText() const {
  std::string fullText;
  for (const TextSection& section : sections) {
    fullText += section.content;
  }
  return fullText;
}

Coord Editor::GetCaretPosition() const { return caret.position; }

void Editor::AppendText(const std::string& newText) {
  if (sections.empty()) {
    sections.push_back({newText});
  } else {
    sections.back().content += newText;
  }
  caret.position.x += newText.size();
}

void Editor::SaveToFile(const std::string& filename) {
  std::ofstream file(
      utils::getResourcePath(ASSET_FOLDER::DATA, filename, ASSET_TYPE::TXT));

  if (file.is_open()) {
    file << text;
    file.close();
  } else {
  }
}

void Editor::DeleteLastChar() {
  if (sections.empty() || caret.position.x == 0) return;

  std::string& lastSection = sections.back().content;
  if (!lastSection.empty()) {
    lastSection.pop_back();
    // caretPosition--;
  }

  // If the last section is now empty, remove it
  if (lastSection.empty()) {
    sections.pop_back();
  }
}

void Editor::Clear() { text.clear(); }

void Editor::MoveCaretLeft() {
  if (caret.position.x > 0) caret.position.x--;
}

void Editor::MoveCaretRight() {
  if (caret.position.x < GetText().size()) caret.position.x++;
}

// Draw caret at the correct position
void Editor::DrawCaret(int x, int y, int fontSize) {
  int textWidth = 0;
  for (const auto& section : sections) {
    std::string text = section.content.substr(0, caret.position.x);
    textWidth = MeasureTextEx(GuiGetFont(), text.c_str(), fontSize, 1.0).x;
  }
  DrawLine(x + textWidth, y, x + textWidth, y + fontSize, BLACK);
}

void Editor::MoveCaretToStart() { caret.MoveToStart(); }

void Editor::MoveCaretToEnd() { caret.MoveToEnd(); }

void Editor::StartSelection() {
  isSelecting = true;
  selectionStart = caret.position;
}

void Editor::EndSelection() {
  isSelecting = false;
  selectionEnd = caret.position;
}

bool Editor::HasSelection() const { return selectionStart != selectionEnd; }

void Editor::ApplyFormatting(bool bold, bool italic, bool underline) {
  if (!HasSelection()) return;

  // Determine the selection range
  Coord start = std::min(selectionStart, selectionEnd);
  Coord end = std::max(selectionStart, selectionEnd);

  std::string fullText = GetText();

  // Split the text into three parts: before selection, selection, and after
  // selection
  std::string beforeSelection = fullText.substr(0, start.x);
  std::string selectionText = fullText.substr(start.x, end.x - start.x);
  std::string afterSelection = fullText.substr(end.x);

  sections.clear();  // Clear the sections

  // Add the "before selection" text as one section
  if (!beforeSelection.empty()) {
    sections.push_back({beforeSelection});
  }

  // Add the selected text as a separate section with formatting
  sections.push_back({selectionText, bold, italic, underline});

  // Add the "after selection" text as another section
  if (!afterSelection.empty()) {
    sections.push_back({afterSelection});
  }

  caret.position = end;  // Move caret to the end of the selection
  selectionStart = selectionEnd = caret.position;  // Clear the selection
}

void Editor::DrawFormattedText(int x, int y, int fontSize) {
  int offsetX = x;
  for (const auto& section : sections) {
    Color color = BLACK;

    // Draw text based on formatting
    if (section.bold) {
      for (int i = 0; i < 2; i++) {
        DrawTextEx(GuiGetFont(), section.content.c_str(),
                   (Vector2){offsetX + i, y + i}, fontSize, 1.0, color);
      }
    } else if (section.italic) {
      for (int i = 0; i < section.content.length(); i++) {
        DrawTextEx(GuiGetFont(), std::string(1, section.content[i]).c_str(),
                   (Vector2){offsetX + i * 5, y + i * 2}, fontSize, 1.0, color);
      }
    } else {
      DrawTextEx(GuiGetFont(), section.content.c_str(), (Vector2){offsetX, y},
                 fontSize, 1.0, color);
    }

    // Draw underline
    if (section.underline) {
      int textWidth =
          MeasureTextEx(GuiGetFont(), section.content.c_str(), fontSize, 1.0).x;
      DrawLine(offsetX, y + fontSize, offsetX + textWidth, y + fontSize, color);
    }

    // Adjust x position for next section
    offsetX +=
        MeasureTextEx(GuiGetFont(), section.content.c_str(), fontSize, 1.0).x;
  }
}

bool Editor::IsButtonClicked(const Button& button) {
  return IsAreaClicked(button.x, button.y, button.width, button.height);
}

bool Editor::IsAreaClicked(int x, int y, int width, int height) {
  Vector2 mousePos = GetMousePosition();
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePos.x >= x &&
         mousePos.x <= x + width && mousePos.y >= y && mousePos.y <= y + height;
}

void Editor::ExecuteCommand(Command* command) {
  command->Execute(*this);
  history.push(command);
}

void Editor::Undo() { history.Undo(*this); }

void Editor::Redo() { history.Redo(*this); }
