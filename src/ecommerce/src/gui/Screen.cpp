#include "Screen.hpp"

void ScreenManager::init() {
  ScreenManager& instance = getInstance();

  instance.addScreen(
      GUIScreen::MAIN, []() -> any { return nullptr; },
      []() -> any { return nullptr; });
  instance.addScreen(
      GUIScreen::ARCHIVED, []() -> any { return nullptr; },
      []() -> any { return nullptr; });
}

void ScreenManager::addScreen(const GUIScreen& name, function<any()> renderF,
                              function<any()> handleF) {
  screens[name] = make_unique<Screen>();

  auto& newSreen = screens[name];
  newSreen->setRenderFunc(renderF);
  newSreen->setHandleFunc(handleF);
}

void ScreenManager::switchScreen(const GUIScreen& name) {
  if (screens.find(name) == screens.end()) return;
  currentScreen = name;
}

void ScreenManager::renderCurrentScreen(Application* app) {
  if (screens.find(currentScreen) == screens.end()) return;
}

void ScreenManager::handleCurrentScreen(Application* app) {
  if (screens.find(currentScreen) == screens.end()) return;
}
