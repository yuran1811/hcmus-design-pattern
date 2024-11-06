#include "index.hpp"

Application::Application() {
  SetTraceLogCallback(utils::CustomLog);
  SetTargetFPS(60);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_SIZE.width, SCREEN_SIZE.height, APP_TITLE.c_str());
  SetExitKey(KEY_NULL);

  GuiSetFont(utils::GetFont(settings.fontSize));
}

Application::~Application() {
  for (auto editor : editors) delete editor;
  delete activeEditor;

  CloseWindow();
}

void Application::createEditor() {
  auto editor = new Editor();
  editors.push_back(editor);
  activeEditor = editor;
}

void Application::render() {
  activeEditor->DrawFormattedText(textX, textY, settings.fontSize);
  activeEditor->DrawCaret(textX, textY, settings.fontSize);
}

void Application::eventHandler() {
  Handler::InputHandler(*activeEditor, fileSaved);
}
