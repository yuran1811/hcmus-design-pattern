#include "raylib.h"

#include "components/index.hpp"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "Example");
  SetTargetFPS(60);

  const char text[] =
      "Text cannot escape\tthis container\t...word wrap also works when active"
      " so here's a long text for testing.\n\nLorem ipsum dolor sit amet, "
      " consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore"
      " et dolore magna aliqua .Nec ullamcorper sit amet risus nullam eget "
      "felis eget.";

  const Font font = GetFontDefault();
  GuiText guiText(text, {25, 25}, 20, BLUE);
  GuiTextWrap textWrap(guiText, {25, 25, screenWidth - 50, screenHeight - 250},
                       SKYBLUE,
                       {60, 60, screenWidth - 50.0f, screenHeight - 160.0f});

  Vector2 lastMouse = {0.0f, 0.0f};
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_SPACE)) textWrap.toggleWordWrap();

    lastMouse = textWrap.resizeHandler(lastMouse);

    // Rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);
    textWrap.render(font, true);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

/* #include <cmath>
#include <iostream>

#include "raylib.h"

#include "components/index.hpp"
#include "utils/index.hpp"

using namespace std;

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Rainbow Sliding Bar");
  SetTargetFPS(60);

  CatpuccinPaletteBar catpuccinMachiatoPalette;

  GuiFont font("IntelOneMono", AssetType::TTF);
  GuiText text("Rainbow Sliding Bar", {screenWidth / 2 - 145, 20}, 32,
               catpuccinMachiatoPalette.getPaletteColor(12));
  GuiText text2("Rainbow Sliding Bar", {screenWidth / 2 - 145, 60}, 32,
                catpuccinMachiatoPalette.getPaletteColor(4));

  float time = 0.0f;

  while (!WindowShouldClose()) {
    time += GetFrameTime() * 2.f;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    catpuccinMachiatoPalette.render({0, 0}, {screenWidth, 10},
                                    CatpuccinPaletteBar::Direction::Horizontal);

    text.render(GuiFont::getFont(font, 32), true, 1,
                GuiText::TextEffect::BREATH, time);
    text2.render(GuiFont::getFont(font, 32), true, 1,
                 GuiText::TextEffect::PULSE, time);

    EndDrawing();
  }

  vector<Font> fontVariants = GuiFont::getFontVariants(font);
  for (Font fontVariant : fontVariants) UnloadFont(fontVariant);

  CloseWindow();

  return 0;
} */

/* #include <cmath>
#include <iostream>

#include "raylib.h"

#include "components/index.hpp"
#include "utils/index.hpp"

using namespace std;

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Rainbow Sliding Bar");
  SetTargetFPS(60);

  // Others
  CatpuccinPaletteBar catpuccinMachiatoPalette;

  // GuiText
  GuiFont font("IntelOneMono", AssetType::TTF);
  GuiText text("Rainbow Sliding Bar", {screenWidth / 2 - 145, 20}, 32,
               catpuccinMachiatoPalette.getPaletteColor(4));
  GenTextureMipmaps(&GuiFont::getFont(font, 32).texture);
  SetTextureFilter(GuiFont::getFont(font, 32).texture,
                   TEXTURE_FILTER_TRILINEAR);

  // ⏰ Timing and brightness variables
  float time = 0.0f;

  // 🌈 Bar dimensions and animation variables
  const int barWidth = 360;
  const int barHeight = 4;
  float hueOffset = 0.0f;
  float hueSpeed = 520.0f;

  // 🎨 Main game loop
  while (!WindowShouldClose()) {
    time += GetFrameTime() * 2.f;

    hueOffset += GetFrameTime() * hueSpeed;
    if (hueOffset > 360.0f || hueOffset < 0.f) hueSpeed *= -1;

    // Begin drawing
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Loop through the width of the bar, setting colors along the way
    for (int x = 0; x < barWidth; x++) {
      // Calculate hue for each x position
      float hue = fmodf(hueOffset + (float)x / barWidth * 360.0f, 360.0f);
      Color color = ColorFromHSV(hue, 1.0f, 1.0f);

      // Draw each pixel of the rainbow bar
      DrawRectangle(screenWidth / 2 - barWidth / 2 + x,
                    screenHeight / 2 - barHeight / 2, 1, barHeight, color);
    }

    catpuccinMachiatoPalette.render({0, 0}, {screenWidth, 10},

                                    CatpuccinPaletteBar::Direction::Horizontal);

    // Draw text
    text.render(GuiFont::getFont(font, 32), true, 1,
GuiText::TextEffect::PULSE, time); DrawText("Rainbow Sliding Bar", screenWidth
/ 2 - 120, 60, 20, BLUE);

    EndDrawing();
  }

  vector<Font> fontVariants = GuiFont::getFontVariants(font);
  for (Font fontVariant : fontVariants) UnloadFont(fontVariant);

  CloseWindow();
  return 0;
}
 */