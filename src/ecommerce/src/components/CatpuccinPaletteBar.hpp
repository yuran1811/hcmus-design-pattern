#pragma once

#include <cmath>

#include "raylib.h"

#include "../utils/index.hpp"

class CatpuccinPaletteBar {
 public:
  enum class Direction { Horizontal, Vertical };
  enum class AnimateType { None, FadeIn, FadeOut, Slide };
  enum class AnimateDirection { Left, Right, Up, Down };

 public:
  CatpuccinPaletteBar() = default;
  CatpuccinPaletteBar(const CatpuccinPaletteBar&) = delete;
  ~CatpuccinPaletteBar() = default;

  CatpuccinPaletteBar& operator=(const CatpuccinPaletteBar&) = delete;

  static CatpuccinPaletteBar& getInstance();

  const Color* getPalette() const;
  const Color getPaletteColor(int index) const;
  const int getPaletteSize() const;

  void render(Vector2 pos, Vector2 size,
              Direction direction = Direction::Horizontal,
              AnimateType animateType = AnimateType::None,
              AnimateDirection animateDirection = AnimateDirection::Left,
              float animateProgress = 0.0f) const;
};
