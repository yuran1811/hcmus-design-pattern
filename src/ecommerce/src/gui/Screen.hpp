#pragma once

#include <any>
#include <map>
#include <memory>
#include <functional>

#include "../shared/index.hpp"

using std::any;
using std::function;
using std::make_unique;
using std::map;
using std::unique_ptr;

class Application;

class Screen : public std::enable_shared_from_this<Screen> {
 private:
  function<any()> renderFunc;
  function<any()> handleFunc;

 public:
  Screen() = default;
  Screen(const Screen&) = delete;
  virtual ~Screen() = default;

  Screen& operator=(const Screen&) = delete;

  void setRenderFunc(function<any()> func) { renderFunc = func; }
  void setHandleFunc(function<any()> func) { handleFunc = func; }

  function<any()> getRenderFunc() const { return renderFunc; }
  function<any()> getHandleFunc() const { return handleFunc; }
};

class ScreenManager {
 private:
  map<GUIScreen, unique_ptr<Screen>> screens;
  GUIScreen currentScreen;

  ScreenManager() = default;
  ScreenManager(const ScreenManager&) = delete;
  ScreenManager& operator=(const ScreenManager&) = delete;

 public:
  static ScreenManager& getInstance() {
    static ScreenManager instance;
    return instance;
  }

  static void init();

  void addScreen(const GUIScreen&, function<any()>, function<any()>);
  void switchScreen(const GUIScreen& name);

  void renderCurrentScreen(Application*);
  void handleCurrentScreen(Application*);
};

#include "../core/Application.hpp"
