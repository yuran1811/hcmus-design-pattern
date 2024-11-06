#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#include "raylib.h"
#include "raygui.h"

#include "../utils/index.hpp"
#include "../shared/index.hpp"

struct Coord {
  int x, y;

  Coord() : x(0), y(0) {}
  Coord(int x, int y) : x(x), y(y) {}
  Coord(const Coord& other) : x(other.x), y(other.y) {}

  bool operator==(const Coord& other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Coord& other) const { return !(*this == other); }
  bool operator<(const Coord& other) const {
    return y < other.y || (y == other.y && x < other.x);
  }

  Coord operator-() const { return Coord(-x, -y); }

  Coord operator+(const Coord& other) const {
    return Coord(x + other.x, y + other.y);
  }
  Coord operator-(const Coord& other) const {
    return Coord(x - other.x, y - other.y);
  }
  Coord operator*(int scalar) const { return Coord(x * scalar, y * scalar); }
  Coord operator/(int scalar) const { return Coord(x / scalar, y / scalar); }

  Coord& operator+=(const Coord& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Coord& operator-=(const Coord& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Coord& operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  Coord& operator/=(int scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }
};

struct TextSection {
  std::string content = "";
  Color color = BLACK;

  bool bold = false;
  bool italic = false;
  bool underline = false;
  bool strikethrough = false;
  bool highlighted = false;
};

struct Caret {
  Coord position;

  Caret() : position(Coord()) {}
  Caret(int x, int y) : position(x, y) {}
  Caret(const Caret& other) : position(other.position) {}

  void Draw(int fontSize) {
    DrawLine(position.x, position.y, position.x, position.y + fontSize, BLACK);
  }

  void Move2D(Coord v) { position = position + v; }
  void MoveToStart() { position.x = 0; }
  void MoveToEnd() { position.x = GetScreenWidth(); }
};

class Editor;

class Command {
 public:
  virtual ~Command() = default;

  virtual void Execute(Editor& editor) = 0;
  virtual void Undo(Editor& editor) = 0;
};

class CommandHistory {
 private:
  std::stack<Command*> undoStack;
  std::stack<Command*> redoStack;

 public:
  CommandHistory();
  ~CommandHistory();

  void push(Command* command);
  void Undo(Editor& editor);
  void Redo(Editor& editor);
};

class Button {
 private:
  Command* command;

 public:
  int x, y, width, height;
  std::string text;

  Button();
  Button(int x, int y, int width, int height, const std::string& text);
  Button(const Button& other);
  ~Button();

  void render();
};

class Shortcut {
 public:
  int key;
  int modifier;
  Command* command;

  Shortcut() : key(0), modifier(0), command(nullptr) {}
  Shortcut(int key, int modifier, Command* command)
      : key(key), modifier(modifier), command(command) {}
  Shortcut(const Shortcut& other)
      : key(other.key), modifier(other.modifier), command(other.command) {}
  ~Shortcut() { delete command; }
};

class Editor {
 private:
  Caret caret;

  bool isSelecting = false;
  Coord selectionStart, selectionEnd;

  // Holds all sections of text with different formatting
  std::vector<TextSection> sections;
  std::string text;

  CommandHistory history;

 public:
  Editor();

  std::string GetText() const;
  Coord GetCaretPosition() const;

  void AppendText(const std::string& newText);
  void SaveToFile(const std::string& filename);
  void DeleteLastChar();
  void Clear();

  void MoveCaretLeft();
  void MoveCaretRight();
  void DrawCaret(int x, int y, int fontSize);

  void MoveCaretToStart();
  void MoveCaretToEnd();

  void StartSelection();
  void EndSelection();
  bool HasSelection() const;
  void ApplyFormatting(bool bold, bool italic, bool underline);
  void DrawFormattedText(int x, int y, int fontSize);

  static bool IsButtonClicked(const Button& button);
  static bool IsAreaClicked(int x, int y, int width, int height);

  void ExecuteCommand(Command* command);

  void Undo();
  void Redo();
};

class Theme {
 public:
  THEME_VARIANT name;

  void apply(const Editor& e);
};

class Settings {
 public:
  static Theme theme;
  static int fontSize;
};

class Application {
 private:
  std::string clipboard;

 public:
  std::vector<Editor*> editors;
  Editor* activeEditor = nullptr;

  Settings settings;

  bool fileSaved = false;

  static const int textX = 10;
  static const int textY = 50;  // Leave space for the toolbar

  Application();
  ~Application();

  void createEditor();

  void render();
  void eventHandler();
  void executeCommand(Command* command);
};

class TypeCommand : public Command {
 private:
  std::string textToType;

 public:
  TypeCommand(const std::string& text) : textToType(text) {}

  void Execute(Editor& editor) override { editor.AppendText(textToType); }

  void Undo(Editor& editor) override {
    for (size_t i = 0; i < textToType.size(); ++i) {
      editor.DeleteLastChar();
    }
  }
};

class DeleteCommand : public Command {
 private:
  char lastChar;

 public:
  void Execute(Editor& editor) override {
    if (!editor.GetText().empty()) {
      lastChar = editor.GetText().back();
      editor.DeleteLastChar();
    }
  }

  void Undo(Editor& editor) override {
    editor.AppendText(std::string(1, lastChar));
  }
};

class SaveCommand : public Command {
 private:
  std::string filename;

 public:
  SaveCommand(const std::string& filename) : filename(filename) {}

  void Execute(Editor& editor) override { editor.SaveToFile(filename); }

  void Undo(Editor& editor) override {}
};

class FormatCommand : public Command {
 private:
  bool bold, italic, underline;

 public:
  FormatCommand(bool bold, bool italic, bool underline)
      : bold(bold), italic(italic), underline(underline) {}

  void Execute(Editor& editor) override {
    editor.ApplyFormatting(bold, italic, underline);
  }

  void Undo(Editor& editor) override {
    editor.ApplyFormatting(!bold, !italic, !underline);
  }
};

namespace Handler {
void InputHandler(Editor& editor, bool& fileSaved);
void CaretHandler(Editor& editor);
}  // namespace Handler