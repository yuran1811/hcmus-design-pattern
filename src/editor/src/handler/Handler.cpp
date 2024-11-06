#include "../core/index.hpp"

void Handler::InputHandler(Editor &editor, bool &fileSaved) {
  CaretHandler(editor);

  int key = GetCharPressed();
  if (key > 0) {
    editor.ExecuteCommand(new TypeCommand(std::string(1, (char)key)));
    fileSaved = false;
  }

  if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
    editor.ExecuteCommand(new DeleteCommand());
    fileSaved = false;
  }

  // Handle actions
  if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
    if (IsKeyPressed(KEY_Z)) {
      editor.Undo();
      fileSaved = false;
    }

    if (IsKeyPressed(KEY_Y)) {
      editor.Redo();
      fileSaved = false;
    }
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) editor.StartSelection();
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) editor.EndSelection();

  if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
      IsKeyDown(KEY_S)) {
    editor.SaveToFile("note");
    fileSaved = true;
  }
}

void Handler::CaretHandler(Editor &editor) {
  if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))
    editor.MoveCaretLeft();
  if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))
    editor.MoveCaretRight();

  if (IsKeyPressed(KEY_HOME)) editor.MoveCaretToStart();
  if (IsKeyPressed(KEY_END)) editor.MoveCaretToEnd();
}
