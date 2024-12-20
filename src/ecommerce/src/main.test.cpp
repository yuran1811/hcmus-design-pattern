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
      ->setEvent(GuiModal::MODAL_EVENT::CLOSE,
                 [&showModal]() { showModal = false; });

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

/* // Mock Order

#include <cmath>
#include <cstring>
#include <iostream>

#include "raylib.h"

#include "core/index.hpp"

using namespace std;

void renderOrderDetail(shared_ptr<Order> order,
                       const pair<bool, vector<string>>& orderReturn,
                       const Price& total, int& posY) {
  DrawText(("Order Details - " + order->getOrderId()).c_str(), 10, posY, 20,
           DARKGRAY);
  for (const string& detail : orderReturn.second) {
    posY += 25;
    DrawText(detail.c_str(), 10, posY, 20, DARKGRAY);
  }

  posY += 25;
  DrawText(("Total: " + total.format()).c_str(), 10, posY, 20, DARKGRAY);
}

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Order Mock");
  SetTargetFPS(60);

  time_t COUPON_ALIVE = time(nullptr) + 3;

  const vector<Coupon> COUPONS = {{"DISCOUNT_10", 10, false, COUPON_ALIVE, 60},
                                  {"DISCOUNT_20", 20, false, COUPON_ALIVE, 60},
                                  {"DISCOUNT_30", 30, false, COUPON_ALIVE, 60}};

  CouponSystem::getInstance()->importCoupons(COUPONS);

  shared_ptr<Order> order = make_shared<BasicOrder>(42.f, "DISCOUNT_10");
  order = make_shared<GiftWrapDecorator>(order, 5.f);
  order = make_shared<ExpressDeliveryDecorator>(order);
  pair<bool, vector<string>> orderReturn;
  Price total = order->calculateTotal();

  shared_ptr<Order> order2 = make_shared<BasicOrder>(42.f, "DISCOUNT_20");
  order2 = make_shared<GiftWrapDecorator>(order2, 5.f);
  order2 = make_shared<ExpressDeliveryDecorator>(order2);
  pair<bool, vector<string>> orderReturn2;
  Price total2 = order2->calculateTotal();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (IsKeyPressed(KEY_SPACE)) {
      if (!order->isOrderCompleted()) {
        orderReturn = order->placeOrder();
        orderReturn2 = order2->placeOrder();

        total = order->calculateTotal();
        total2 = order2->calculateTotal();
      }
    }

    int posY = 10;
    renderOrderDetail(order, orderReturn, total, posY);

    posY += 50;
    renderOrderDetail(order2, orderReturn2, total2, posY);

    posY += 50;
    vector<Coupon> savedCoupons = CouponSystem::getInstance()->showCoupons();
    for (const Coupon& coupon : savedCoupons) {
      DrawText(("Coupon: " + coupon.code + " - " + to_string(coupon.usageLimit))
                   .c_str(),
               10, posY, 20, DARKGRAY);
      posY += 25;
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
} */

/* // UUIDv4

#include <cmath>
#include <cstring>
#include <unordered_map>
#include <iostream>

#include "uuid_v4.hpp"
#include "raylib.h"

using namespace std;

unordered_map<string, int> uuids;

void generateUUID(char *s) {
  static UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
  static UUIDv4::UUID test;

  test = uuidGenerator.getUUID();
  test.str(s);
  s[strlen(s) - 1] = '\0';

  uuids[s]++;
}

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Test UUIDv4");
  SetTargetFPS(60);

  char txt[37];
  int countDup = 0;
  int countSample = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText(txt, 10, 10, 20, DARKGRAY);
    DrawText(to_string(countDup).c_str(), 10, 30, 20, DARKGRAY);
    DrawText(to_string(countSample).c_str(), 10, 50, 20, DARKGRAY);
    DrawText("Press SPACE to generate a new UUID", 10, 70, 20, DARKGRAY);

    countDup += uuids[txt] > 1;
    countSample++;
    generateUUID(txt);

    EndDrawing();
  }

  CloseWindow();

  return 0;
} */

/* // Confetti

#include "raylib.h"

#include "components/index.hpp"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "Confetti");
  SetTargetFPS(60);

  ConfettiParticles* confettiParticles = nullptr;

  while (!WindowShouldClose()) {
    // Rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (IsKeyPressed(KEY_SPACE)) {
      if (confettiParticles) {
        confettiParticles->generateSetOfConfetties();
      }
    }

    if (!confettiParticles) {
      confettiParticles = new ConfettiParticles(400, screenWidth, screenHeight);
      confettiParticles->setCanonPosition({screenWidth / 2, screenHeight});
    }

    confettiParticles->update();
    confettiParticles->render();

    DrawText(to_string(confettiParticles->isAlive()).c_str(), 10, 10, 20,
             DARKGRAY);

    EndDrawing();
  }

  CloseWindow();

  delete confettiParticles;

  return 0;
} */

/* // Text wrap

#include "raylib.h"

#include "components/index.hpp"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "Text Wrap");
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
