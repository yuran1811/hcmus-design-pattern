/* // Modal Box

#include "components/GuiComponent.hpp"

int main(void) {
  const int screenWidth = 720;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Modal Box");
  SetTargetFPS(60);

  const string modalId = utils::uuid_v4();

  bool showModal = true;
  (new GuiModal(
       modalId, "Modal Title",
       "\"l-value\" refers to memory location which identifies an object. "
       "modifiable l-value cannot have an array type, an incomplete type, or a "
       "identifier is a modifiable lvalue if it refers to a memory location "
       "that designates the storage region to which ptr points. In C, the "
       "that locate (designate) objects.\n\n\tyuran1811",
       {screenWidth / 2 - 200, screenHeight / 2 - 200, 400, 400}))
      ->setEvent(GuiModal::EVENT::CLOSE, [&showModal]() { showModal = false; });

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_SPACE)) showModal = !showModal;
    if (showModal && IsKeyPressed(KEY_ENTER)) showModal = false;
    GuiModal::eventsHandler();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Press SPACE to toggle modal", 200, screenHeight / 2, 20,
             DARKGRAY);
    if (showModal) {
      GuiModal* modal = GuiModal::getModal(modalId);
      if (modal) modal->render(GetFontDefault(), true);
    }
    EndDrawing();
  }

  GuiModal::releaseModals();
  CloseWindow();

  return 0;
} */

/* // Text effects

#include <cmath>

#include "components/index.hpp"

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

/* // Rainbow Sliding Bar

#include <cmath>

#include "raylib.h"

int main(void) {
  const int screenWidth = 400;
  const int screenHeight = 60;
  InitWindow(screenWidth, screenHeight, "Rainbow Sliding Bar");
  SetTargetFPS(60);

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

    EndDrawing();
  }

  CloseWindow();
  return 0;
} */

/* // Testing Template

#include <iostream>

#include "raylib.h"

using namespace std;

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Test");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
} */
