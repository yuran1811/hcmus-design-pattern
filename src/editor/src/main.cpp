#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>

#define RAYGUI_IMPLEMENTATION

#include "core/index.hpp"
#include "shared/index.hpp"
#include "utils/index.hpp"

namespace DemoRayGUI {
Rectangle panelRec = {20, 40, 200, 150};
Rectangle panelContentRec = {0, 0, 340, 340};
Rectangle panelView = {0};
Vector2 panelScroll = {99, -20};

bool showContentArea = true;

void RenderModal(bool &showMessageBox) {
  if (GuiButton((Rectangle){24, 24, 120, 30}, "#191#Show Message"))
    showMessageBox = true;

  if (showMessageBox) {
    int result =
        GuiMessageBox((Rectangle){85, 70, 250, 100}, "#191#Message Box",
                      "Hi! This is a message!", "Nice;Cool;Ok;Awesome");

    if (result >= 0) showMessageBox = false;
  }
}

static void DrawStyleEditControls(void) {
  // ScrollPanel style controls
  //----------------------------------------------------------
  GuiGroupBox((Rectangle){550, 170, 220, 205}, "SCROLLBAR STYLE");

  int style = GuiGetStyle(SCROLLBAR, BORDER_WIDTH);
  GuiLabel((Rectangle){555, 195, 110, 10}, "BORDER_WIDTH");
  GuiSpinner((Rectangle){670, 190, 90, 20}, NULL, &style, 0, 6, false);
  GuiSetStyle(SCROLLBAR, BORDER_WIDTH, style);

  style = GuiGetStyle(SCROLLBAR, ARROWS_SIZE);
  GuiLabel((Rectangle){555, 220, 110, 10}, "ARROWS_SIZE");
  GuiSpinner((Rectangle){670, 215, 90, 20}, NULL, &style, 4, 14, false);
  GuiSetStyle(SCROLLBAR, ARROWS_SIZE, style);

  style = GuiGetStyle(SCROLLBAR, SLIDER_PADDING);
  GuiLabel((Rectangle){555, 245, 110, 10}, "SLIDER_PADDING");
  GuiSpinner((Rectangle){670, 240, 90, 20}, NULL, &style, 0, 14, false);
  GuiSetStyle(SCROLLBAR, SLIDER_PADDING, style);

  bool scrollBarArrows = GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE);
  GuiCheckBox((Rectangle){565, 280, 20, 20}, "ARROWS_VISIBLE",
              &scrollBarArrows);
  GuiSetStyle(SCROLLBAR, ARROWS_VISIBLE, scrollBarArrows);

  style = GuiGetStyle(SCROLLBAR, SLIDER_PADDING);
  GuiLabel((Rectangle){555, 325, 110, 10}, "SLIDER_PADDING");
  GuiSpinner((Rectangle){670, 320, 90, 20}, NULL, &style, 0, 14, false);
  GuiSetStyle(SCROLLBAR, SLIDER_PADDING, style);

  style = GuiGetStyle(SCROLLBAR, SLIDER_WIDTH);
  GuiLabel((Rectangle){555, 350, 110, 10}, "SLIDER_WIDTH");
  GuiSpinner((Rectangle){670, 345, 90, 20}, NULL, &style, 2, 100, false);
  GuiSetStyle(SCROLLBAR, SLIDER_WIDTH, style);

  const char *text =
      GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE
          ? "SCROLLBAR: LEFT"
          : "SCROLLBAR: RIGHT";
  bool toggleScrollBarSide = GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE);
  GuiToggle((Rectangle){560, 110, 200, 35}, text, &toggleScrollBarSide);
  GuiSetStyle(LISTVIEW, SCROLLBAR_SIDE, toggleScrollBarSide);
  //----------------------------------------------------------

  // ScrollBar style controls
  //----------------------------------------------------------
  GuiGroupBox((Rectangle){550, 20, 220, 135}, "SCROLLPANEL STYLE");

  style = GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);
  GuiLabel((Rectangle){555, 35, 110, 10}, "SCROLLBAR_WIDTH");
  GuiSpinner((Rectangle){670, 30, 90, 20}, NULL, &style, 6, 30, false);
  GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, style);

  style = GuiGetStyle(DEFAULT, BORDER_WIDTH);
  GuiLabel((Rectangle){555, 60, 110, 10}, "BORDER_WIDTH");
  GuiSpinner((Rectangle){670, 55, 90, 20}, NULL, &style, 0, 20, false);
  GuiSetStyle(DEFAULT, BORDER_WIDTH, style);
  //----------------------------------------------------------
}

void RenderScrollPanel() {
  GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

  BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
  GuiGrid((Rectangle){panelRec.x + panelScroll.x, panelRec.y + panelScroll.y,
                      panelContentRec.width, panelContentRec.height},
          NULL, 16, 3, NULL);
  EndScissorMode();

  if (showContentArea)
    DrawRectangle(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y,
                  panelContentRec.width, panelContentRec.height,
                  Fade(RED, 0.1));

  DrawStyleEditControls();

  GuiCheckBox((Rectangle){565, 80, 20, 20}, "SHOW CONTENT AREA",
              &showContentArea);

  GuiSliderBar((Rectangle){590, 385, 145, 15}, "WIDTH",
               TextFormat("%i", (int)panelContentRec.width),
               &panelContentRec.width, 1, 600);
  GuiSliderBar((Rectangle){590, 410, 145, 15}, "HEIGHT",
               TextFormat("%i", (int)panelContentRec.height),
               &panelContentRec.height, 1, 400);
}
}  // namespace DemoRayGUI

int main() {
  Application app;

  app.createEditor();

  std::vector<Button *> buttons;
  buttons.push_back(
      new Button(10, 5, 150, 25, GuiIconText(ICON_FILE_SAVE_CLASSIC, "Save")));
  buttons.push_back(new Button(100, 5, 150, 25, "I"));

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    app.eventHandler();
    app.render();

    // Draw buttons
    for (auto &button : buttons) button->render();

    // Draw toolbar buttons
    // DrawRectangle(10, buttonY, buttonWidth, buttonHeight, LIGHTGRAY);
    // DrawText("B", 20, buttonY + 5, app.fontSize, BLACK);

    // DrawRectangle(100, buttonY, buttonWidth, buttonHeight, LIGHTGRAY);
    // DrawText("I", 115, buttonY + 5, app.fontSize, BLACK);

    // DrawRectangle(190, buttonY, buttonWidth, buttonHeight, LIGHTGRAY);
    // DrawText("U", 195, buttonY + 5, app.fontSize, BLACK);

    // Action handlers
    // if (app.fileSaved) {
    //   DrawText("File saved!", SCREEN_SIZE.width - 150, SCREEN_SIZE.height
    //   - 30,
    //            20, GREEN);
    // }

    EndDrawing();
  }

  for (auto &button : buttons) delete button;

  return 0;
}
