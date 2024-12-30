#pragma once

#include <map>
#include <memory>
#include <functional>

#include "../shared/index.hpp"

using std::function;
using std::make_unique;
using std::map;
using std::unique_ptr;

typedef function<void()> ScreenFunc;

class Screen : public std::enable_shared_from_this<Screen> {
 private:
  ScreenFunc renderFunc;
  ScreenFunc handleFunc;

 public:
  Screen() = default;
  Screen(const Screen &) = delete;
  virtual ~Screen() = default;

  Screen &operator=(const Screen &) = delete;

  void setRenderFunc(ScreenFunc func) { renderFunc = func; }
  void setHandleFunc(ScreenFunc func) { handleFunc = func; }

  void render() const { renderFunc(); }
  void handle() const { handleFunc(); }
};

// Singleton class for managing GUI screens
class ScreenManager {
 private:
  map<GUIScreen, unique_ptr<Screen>> screens;
  GUIScreen currentScreen = GUIScreen::MAIN;

  ScreenManager() = default;
  ScreenManager(const ScreenManager &) = delete;
  ScreenManager &operator=(const ScreenManager &) = delete;

 public:
  static ScreenManager &getInstance() {
    static ScreenManager instance;
    return instance;
  }

  GUIScreen getCurrentScreen() const;

  void addScreen(const GUIScreen &, ScreenFunc, ScreenFunc);
  void switchScreen(const GUIScreen &name);

  void renderCurrentScreen();
  void handleCurrentScreen();
};
