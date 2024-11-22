#pragma once

#include <map>
#include <vector>

#include "raylib.h"

#include "../shared/index.hpp"
#include "../utils/index.hpp"

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
  static Font& getFont(const GuiFont&, const int& size,
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

 public:
  GuiTextWrap() = delete;
  GuiTextWrap(const GuiText&, Rectangle, Color, Vector4);
  ~GuiTextWrap() = default;

  const Rectangle& getWrapper() const { return wrapper; }
  const Rectangle& getResizer() const { return resizer; }

  Rectangle getBoundsInner() const {
    return {wrapper.x + 4, wrapper.y + 4, wrapper.width - 4,
            wrapper.height - 4};
  }

  void toggleWordWrap();

  void updateWrapper(float, float);
  void updateResizer();

  Vector2 resizeHandler(const Vector2&);

  void renderWrapper() const;
  void renderResizer() const;
  void render(const Font&, bool, ...);
};

class GuiButton {
 private:
  static vector<GuiButton*> buttons;

  string text;
  Rectangle bounds;
  Color textColor;
  Color color;
  int px, py;

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

  const bool isHovered() const;

  GuiButton* render(const Font&, bool, ...);
};
