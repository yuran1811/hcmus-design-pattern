#include "GuiComponent.hpp"

vector<GuiButton*> GuiButton::buttons;

GuiButton::GuiButton(const string& text, Rectangle bounds, Color fg, Color bg,
                     int padding)
    : text(text),
      bounds(bounds),
      textColor(fg),
      color(bg),
      px(padding),
      py(padding) {
  buttons.push_back(this);
}

GuiButton::GuiButton(const string& text, Rectangle bounds, Color fg, Color bg,
                     int px, int py)
    : text(text), bounds(bounds), textColor(fg), color(bg), px(px), py(py) {
  buttons.push_back(this);
}

void GuiButton::releaseButtons() {
  for (GuiButton* _ : buttons) delete _;
  buttons.clear();
}

void GuiButton::eventsHandler() {
  for (GuiButton* button : buttons) {
    if (!(button->isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
      continue;

    if (button->events.find(EVENT::CLICK) != button->events.end())
      button->events[EVENT::CLICK]();
  }
}

const bool GuiButton::isHovered() const {
  return CheckCollisionPointRec(GetMousePosition(), bounds);
}

GuiButton* GuiButton::render(const Font& font, bool withEffect, ...) {
  const bool hoverState = isHovered();

  DrawRectangleRounded(
      bounds, .25f, 40,
      hoverState ? color : Color{color.r, color.g, color.b, 185});
  DrawTextEx(font, text.c_str(), {bounds.x + px, bounds.y + py}, 20, 1.8f,
             textColor);

  return this;
}

GuiButton* GuiButton::setEvent(EVENT e, function<void()> _) {
  events[e] = _;
  return this;
}
