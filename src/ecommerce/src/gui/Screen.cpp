#include "Screen.hpp"

GUIScreen ScreenManager::getCurrentScreen() const { return currentScreen; }

void ScreenManager::addScreen(const GUIScreen& name, ScreenFunc renderF,
                              ScreenFunc handleF) {
  screens[name] = make_unique<Screen>();

  auto& newSreen = screens[name];
  newSreen->setRenderFunc(renderF);
  newSreen->setHandleFunc(handleF);
}

void ScreenManager::switchScreen(const GUIScreen& name) {
  currentScreen =
      (screens.find(name) == screens.end()) ? GUIScreen::NOT_FOUND : name;
}

void ScreenManager::renderCurrentScreen() {
  if (screens.find(currentScreen) == screens.end()) return;
  screens[currentScreen]->render();
}

void ScreenManager::handleCurrentScreen() {
  if (screens.find(currentScreen) == screens.end()) return;
  screens[currentScreen]->handle();
}
