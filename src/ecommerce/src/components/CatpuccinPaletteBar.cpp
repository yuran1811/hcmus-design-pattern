#include "CatpuccinPaletteBar.hpp"

CatpuccinPaletteBar& CatpuccinPaletteBar::getInstance() {
  static CatpuccinPaletteBar instance;
  return instance;
}

const Color* CatpuccinPaletteBar::getPalette() const {
  static const Color catpuccinMachiatoPalette[] = {
      {244, 219, 214, 255}, {240, 198, 198, 255}, {245, 189, 230, 255},
      {198, 160, 246, 255}, {237, 135, 150, 255}, {238, 153, 160, 255},
      {245, 169, 127, 255}, {238, 212, 159, 255}, {166, 218, 149, 255},
      {139, 213, 202, 255}, {145, 215, 227, 255}, {125, 196, 228, 255},
      {138, 173, 244, 255}, {183, 189, 248, 255}, {197, 207, 245, 255}};

  return catpuccinMachiatoPalette;
}

const Color CatpuccinPaletteBar::getPaletteColor(int index) const {
  index = abs(index) % getPaletteSize();
  return getPalette()[index];
}

const int CatpuccinPaletteBar::getPaletteSize() const { return 15; }

void CatpuccinPaletteBar::render(Vector2 pos, Vector2 size, Direction direction,
                                 AnimateType animateType,
                                 AnimateDirection animateDirection,
                                 float animateProgress) const {
  const Color* palette = getPalette();
  const int paletteSize = getPaletteSize();

  if (direction == Direction::Horizontal)
    for (int i = 0; i < paletteSize; i++)
      DrawRectangleV({pos.x + 1.f * i * size.x / paletteSize, pos.y},
                     {size.x / paletteSize, size.y}, palette[i]);
  else
    for (int i = 0; i < paletteSize; i++)
      DrawRectangleV({pos.x, pos.y + 1.f * i * size.y / paletteSize},
                     {size.x, size.y / paletteSize}, palette[i]);
}
