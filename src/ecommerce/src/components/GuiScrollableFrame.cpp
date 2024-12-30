#include "GuiComponent.hpp"

GuiScrollableFrame::GuiScrollableFrame(Rectangle _v, Rectangle _c)
    : viewport(_v), content(_c), scrollOffset({0, 0}) {}

void GuiScrollableFrame::updateContentHeight(float _) {
  content.height = max(_, viewport.height);
}

void GuiScrollableFrame::handleScrolling() {
  if (!utils::ui::mouseHoveredInBox(viewport)) return;

  // Update scroll offset with mouse wheel and arrow keys
  if (IsKeyDown(KEY_UP)) scrollOffset.y += 5;
  if (IsKeyDown(KEY_DOWN)) scrollOffset.y -= 5;
  if (IsKeyDown(KEY_LEFT)) scrollOffset.x += 5;
  if (IsKeyDown(KEY_RIGHT)) scrollOffset.x -= 5;

  if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
    scrollOffset.x += GetMouseWheelMove() * 20;
  else
    scrollOffset.y += GetMouseWheelMove() * 20;

  // Clamp scrolling to content bounds
  if (scrollOffset.x > 0) scrollOffset.x = 0;
  if (scrollOffset.y > 0) scrollOffset.y = 0;

  if (scrollOffset.x < viewport.width - content.width)
    scrollOffset.x = viewport.width - content.width;
  if (scrollOffset.y < viewport.height - content.height)
    scrollOffset.y = viewport.height - content.height;
}

void GuiScrollableFrame::render(function<void()> callback) {
  BeginScissorMode(viewport.x, viewport.y, viewport.width, viewport.height);
  callback();
  EndScissorMode();
}
