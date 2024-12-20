#include "GuiComponent.hpp"

GuiText::GuiText(const string& text, Vector2 pos = {0, 0}, int fontSize = 28,
                 Color color = BLACK)
    : text(text), pos(pos), fontSize(fontSize), color(color) {}

GuiText::GuiText(const GuiText& _)
    : text(_.text), pos(_.pos), fontSize(_.fontSize), color(_.color) {}

void GuiText::render(const Font& font, bool withEffect, ...) {
  if (!withEffect) {
    DrawTextEx(font, text.c_str(), pos, fontSize, 0, color);
    return;
  }

  va_list args;
  va_start(args, withEffect);
  const int effectCount = va_arg(args, int);
  for (int i = 0; i < effectCount; i++) {
    int effect = va_arg(args, int);

    switch (effect) {
      case int(TextEffect::BREATH):
        DrawTextEx(font, text.c_str(), pos, fontSize, 0,
                   utils::color::calcBreathColor(color, va_arg(args, double)));
        continue;

      case int(TextEffect::PULSE):
        DrawTextEx(font, text.c_str(), pos, fontSize, 0,
                   utils::color::calcPulseColor(color, va_arg(args, double)));
        continue;

      default:
        continue;
    }
  }
  va_end(args);
}

GuiTextWrap::GuiTextWrap(const GuiText& text, Rectangle wrapper,
                         Color background, Rectangle size)
    : GuiText(text),
      wrapper(wrapper),
      background(background),
      wordWrap(true),
      resizing(false) {
  minWidth = size.x;
  minHeight = size.y;
  maxWidth = size.width;
  maxHeight = size.height;

  resizer = {wrapper.x + wrapper.width - 17, wrapper.y + wrapper.height - 17,
             14, 14};
}

void GuiTextWrap::updatePosition(Vector2 _) {
  wrapper.x = _.x, wrapper.y = _.y;
  updateResizer();
}

void GuiTextWrap::toggleWordWrap() { wordWrap = !wordWrap; }

void GuiTextWrap::updateWrapper(float w, float h) {
  wrapper.width = (w > minWidth) ? ((w < maxWidth) ? w : maxWidth) : minWidth;
  wrapper.height =
      (h > minHeight) ? ((h < maxHeight) ? h : maxHeight) : minHeight;

  updateResizer();
}

void GuiTextWrap::updateResizer() {
  resizer.x = wrapper.x + wrapper.width - 17;
  resizer.y = wrapper.y + wrapper.height - 17;
}

Vector2 GuiTextWrap::resizeHandler(const Vector2& lastMouse) {
  Vector2 mouse = GetMousePosition();
  if (resizing) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) resizing = false;

    float width = wrapper.width + (mouse.x - lastMouse.x);
    float height = wrapper.height + (mouse.y - lastMouse.y);

    updateWrapper(width, height);
  } else {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mouse, resizer))
      resizing = true;
  }

  return mouse;
}

void GuiTextWrap::renderWrapper() const {
  DrawRectangleLinesEx(wrapper, 3, background);
}

void GuiTextWrap::renderResizer() const {
  DrawRectangleRec(resizer, background);
  DrawRectangleLinesEx(resizer, 3, getColor());
}

void GuiTextWrap::rawRender(const Font& font) {
  const string& text = getText();
  const int length = text.size();
  const Rectangle rec = getBoundsInner();

  int selectStart = 0;
  int selectLength = 0;
  float spacing = 2.f;

  // Offset between lines (on line break '\n')
  float textOffsetY = 0;
  // Offset X to next character to draw
  float textOffsetX = 0.0f;
  // Character rectangle scaling factor
  float scaleFactor = getFontSize() / (float)font.baseSize;

  enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
  int state = wordWrap ? MEASURE_STATE : DRAW_STATE;

  int startLine = -1;  // Index where to begin drawing (where a line begins)
  int endLine = -1;    // Index where to stop drawing (where a line ends)
  int lastk = -1;      // Holds last value of the character position

  for (int i = 0, k = 0; i < length; i++, k++) {
    // Get next codepoint from byte string and glyph index in font
    int codepointByteCount = 0;
    int codepoint = GetCodepoint(&text[i], &codepointByteCount);
    int index = GetGlyphIndex(font, codepoint);

    // NOTE: Normally we exit the decoding sequence as soon as a bad byte is
    // found (and return 0x3f) but we need to draw all of the bad bytes using
    // the '?' symbol moving one byte
    if (codepoint == 0x3f) codepointByteCount = 1;
    i += (codepointByteCount - 1);

    float glyphWidth = 0;
    if (codepoint != '\n') {
      glyphWidth = (font.glyphs[index].advanceX == 0)
                       ? font.recs[index].width * scaleFactor
                       : font.glyphs[index].advanceX * scaleFactor;

      if (i + 1 < length) glyphWidth = glyphWidth + spacing;
    }

    if (state == MEASURE_STATE) {
      if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
        endLine = i;

      if ((textOffsetX + glyphWidth) > rec.width) {
        endLine = (endLine < 1) ? i : endLine;
        if (i == endLine) endLine -= codepointByteCount;
        if ((startLine + codepointByteCount) == endLine)
          endLine = (i - codepointByteCount);

        state = !state;
      } else if ((i + 1) == length) {
        endLine = i;
        state = !state;
      } else if (codepoint == '\n')
        state = !state;

      if (state == DRAW_STATE) {
        textOffsetX = 0;
        i = startLine;
        glyphWidth = 0;

        // Save character position when we switch states
        int tmp = lastk;
        lastk = k - 1;
        k = tmp;
      }
    } else {
      if (codepoint == '\n') {
        if (!wordWrap) {
          textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
          textOffsetX = 0;
        }
      } else {
        if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width)) {
          textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
          textOffsetX = 0;
        }

        // When text overflows rectangle height limit, just stop drawing
        if ((textOffsetY + font.baseSize * scaleFactor) > rec.height) break;

        // Draw selection background
        bool isGlyphSelected = false;
        if ((selectStart >= 0) && (k >= selectStart) &&
            (k < (selectStart + selectLength))) {
          DrawRectangleRec(
              (Rectangle){rec.x + textOffsetX - 1, rec.y + textOffsetY,
                          glyphWidth, (float)font.baseSize * scaleFactor},
              RED);
          isGlyphSelected = true;
        }

        // Draw current character glyph
        if ((codepoint != ' ') && (codepoint != '\t')) {
          DrawTextCodepoint(font, codepoint,
                            (Vector2){rec.x + textOffsetX, rec.y + textOffsetY},
                            getFontSize(), isGlyphSelected ? RED : getColor());
        }
      }

      if (wordWrap && (i == endLine)) {
        textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
        textOffsetX = 0;
        startLine = endLine;
        endLine = -1;
        glyphWidth = 0;
        selectStart += lastk - k;
        k = lastk;

        state = !state;
      }
    }

    if ((textOffsetX != 0) || (codepoint != ' '))
      textOffsetX += glyphWidth;  // avoid leading spaces
  }
}

void GuiTextWrap::render(const Font& font, bool withEffect, ...) {
  renderWrapper();
  renderResizer();
  rawRender(font);
}
