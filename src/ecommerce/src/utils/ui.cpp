#include "index.hpp"

bool utils::ui::mouseHoveredInBox(Rectangle rec) {
  return CheckCollisionPointRec(GetMousePosition(), rec);
}

bool utils::ui::mousePressedInBox(Rectangle rec, MouseButton type) {
  return IsMouseButtonPressed(type) && mouseHoveredInBox(rec);
}

bool utils::ui::mouseHoveredInCircle(Vector2 pos, float radius) {
  return CheckCollisionPointCircle(GetMousePosition(), pos, radius);
}

bool utils::ui::mousePressedInCircle(Vector2 pos, float radius,
                                     MouseButton type) {
  return IsMouseButtonPressed(type) && mouseHoveredInCircle(pos, radius);
}
