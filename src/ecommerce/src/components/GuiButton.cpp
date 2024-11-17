#include "GuiComponent.hpp"

GuiButton::GuiButton(const string& text, Rectangle bounds, Color fg, Color bg)
    : text(text), bounds(bounds), textColor(fg), color(bg) {}

void GuiButton::render(const Font& font, bool withEffect, ...) {
  DrawRectangleRec(bounds, color);
  DrawTextEx(font, text.c_str(), {bounds.x + 10, bounds.y + 10}, 28, 0,
             textColor);
}
