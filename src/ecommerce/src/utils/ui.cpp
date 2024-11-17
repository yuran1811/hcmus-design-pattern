#include "index.hpp"

bool utils::ui::mousePressedInBox(Rectangle rec, MouseButton type) {
  return IsMouseButtonPressed(type) &&
         CheckCollisionPointRec(GetMousePosition(), rec);
}
