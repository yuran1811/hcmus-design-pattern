#pragma once

#include <map>
#include <vector>
#include <functional>

#include "raylib.h"

#include "../shared/index.hpp"
#include "../utils/index.hpp"

using std::function;
using std::map;
using std::vector;

class GuiFont {
 public:
  enum class FontStyle {
    BOLD,
    BOLD_ITALIC,
    ITALIC,
    LIGHT,
    LIGHT_ITALIC,
    MEDIUM,
    MEDIUM_ITALIC,
    REGULAR
  };

 private:
  string fontName;
  AssetType type;

 public:
  GuiFont() = delete;
  GuiFont(const string&, AssetType);
  ~GuiFont() = default;

  string getFontName() const { return fontName; }
  AssetType getFontType() const { return type; }

  static const string getFontStyle(const FontStyle&);
  static string getFontPath(const GuiFont&, const FontStyle&);
  static vector<Font>& getFontVariants(const GuiFont&);
  static Font& getFont(const GuiFont&, const int&,
                       FontStyle style = FontStyle::REGULAR);
};

class GuiText {
 public:
  enum class TextEffect { BREATH, PULSE };

 private:
  string text;
  Vector2 pos;
  int fontSize;
  Color color;

 public:
  GuiText() = delete;
  GuiText(const string&, Vector2, int, Color);
  GuiText(const GuiText&);
  ~GuiText() = default;

  const string& getText() const { return text; }
  void setText(const string& _text) { text = _text; }

  int getFontSize() const { return fontSize; }
  void setFontSize(const int _fontSize) { fontSize = _fontSize; }

  const Vector2& getPosition() const { return pos; }
  void setPosition(const Vector2& _pos) { pos = _pos; }

  const Color& getColor() const { return color; }
  void setColor(const Color& _color) { color = _color; }

  void render(const Font&, bool, ...);
};

class GuiTextWrap : public GuiText {
 private:
  float minWidth;
  float minHeight;
  float maxWidth;
  float maxHeight;

  Rectangle wrapper;
  Rectangle resizer;

  Color background;

  bool wordWrap;
  bool resizing;

  float contentHeight;

 public:
  GuiTextWrap() = delete;
  GuiTextWrap(const GuiText&, Rectangle, Color, Rectangle);
  ~GuiTextWrap() = default;

  const Rectangle& getWrapper() const { return wrapper; }
  const Rectangle& getResizer() const { return resizer; }
  const float& getContentHeight() const { return contentHeight; }

  Rectangle getBoundsInner() const {
    return {wrapper.x + 4, wrapper.y + 4, wrapper.width - 4,
            wrapper.height - 4};
  }

  void toggleWordWrap();

  void updatePosition(Vector2 _);
  void updateWrapper(float, float);
  void updateResizer();

  Vector2 resizeHandler(const Vector2&);

  void renderWrapper() const;
  void renderResizer() const;
  void rawRender(const Font&);
  void render(const Font&, bool, ...);
};

class GuiButton {
 public:
  enum class EVENT { CLICK = 0 };

 private:
  static vector<GuiButton*> buttons;

  string text;
  Rectangle bounds;
  Color textColor;
  Color color;
  int px, py;

  map<EVENT, function<void()>> events;

 public:
  GuiButton() = delete;
  GuiButton(const string&, Rectangle, Color, Color, int);
  GuiButton(const string&, Rectangle, Color, Color, int, int);
  GuiButton(const GuiButton&) = delete;
  ~GuiButton() = default;

  const string& getText() const { return text; }
  const Rectangle& getBounds() const { return bounds; }

  static vector<GuiButton*>& getButtons() { return buttons; }
  static void releaseButtons();
  static void eventsHandler();

  const bool isHovered() const;

  GuiButton* render(const Font&, bool, ...);

  GuiButton* setEvent(EVENT, function<void()>);
  function<void()> getEvent(EVENT e) { return events[e]; }
};

class GuiScrollableFrame {
 private:
  Rectangle viewport;
  Rectangle content;
  Vector2 scrollOffset;

 public:
  GuiScrollableFrame() = delete;
  GuiScrollableFrame(Rectangle, Rectangle);
  ~GuiScrollableFrame() = default;

  const Rectangle& getViewport() const { return viewport; }
  const Rectangle& getContent() const { return content; }
  const Vector2& getScrollOffset() const { return scrollOffset; }
  Vector2 getRenderPosition() const {
    return {content.x + scrollOffset.x, content.y + scrollOffset.y};
  }

  void handleScrolling();
  void render(function<void()>);
};

class GuiModal {
 public:
  enum class EVENT { CLOSE = 0 };

 private:
  static map<string, GuiModal*> modals;

  const string modalID;
  string title;
  string message;
  Rectangle bounds;

  float messageContainerHeight;

  GuiTextWrap msgText;
  GuiScrollableFrame scrollableMessageFrame;

  map<EVENT, function<void()>> events;

 public:
  GuiModal() = delete;
  GuiModal(const string&, const string&, const string&, Rectangle);
  GuiModal(const GuiModal&) = delete;
  ~GuiModal() = default;

  const string& getTitle() const { return title; }
  const string& getMessage() const { return message; }
  const Rectangle& getBounds() const { return bounds; }

  Rectangle getTitleBounds() const;
  Rectangle getTitleContainer() const;
  Rectangle getTitleWrapperClamp() const;
  Rectangle getMessageBounds() const;
  Rectangle getMessageContainer() const;
  Rectangle getMessageWrapperClamp() const;

  Rectangle getCloseBtnBounds() const;

  static GuiModal* getModal(const string& _) { return modals[_]; }
  static void releaseModals();
  static void eventsHandler();

  GuiModal* render(const Font&, bool, ...);

  GuiModal* setEvent(EVENT, function<void()>);
  function<void()> getEvent(EVENT e) { return events[e]; }
};
