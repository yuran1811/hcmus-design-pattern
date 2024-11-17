#include "Gui.hpp"

GUI::GUI() {
  for (BitState& state : inputActiveStates) state.reset();
}

GUI::~GUI() {
  vector<Texture2D>& itemTextures = getTextureCollection();
  for (Texture2D& texture : itemTextures) UnloadTexture(texture);

  CloseWindow();
}

int GUI::getFrameCounter() const { return frameCounter; }

float GUI::getFrameTimer() const { return frameTimer; }

vector<Texture2D>& GUI::getTextureCollection() {
  static vector<Texture2D> textures(1);
  return textures;
}

void GUI::incFrameCounter() {
  frameCounter++;
  if (frameCounter >= TARGET_FPS) frameCounter = 0;
}

void GUI::incFrameTimer() {
  frameTimer += GetFrameTime();
  if (frameTimer >= 1e6) frameTimer -= 1e6;
}

void GUI::setCTARec(const Rectangle& rec) { ctaRec = rec; }

void GUI::init() {
  SetTraceLogCallback(utils::log::CustomLog);
  SetTargetFPS(TARGET_FPS);
  InitWindow(SCREEN_SIZE.width, SCREEN_SIZE.height, APP_TITLE.c_str());
  SetExitKey(KEY_NULL);

  loadingStates.set((unsigned int)LoadingState::PAYMENT_QR);
}

void GUI::cursorUpdate(const OrderStageState& curStage) {
  if (curStage == OrderStageState::ADDRESS_INPUT) {
    bool isHovered = false;
    vector<Rectangle> recs = {ADDR_INP_REC, PHONE_INP_REC};
    for (int i = 0; i < recs.size(); i++)
      if (CheckCollisionPointRec(GetMousePosition(), recs[i])) {
        cursorBitState.set((unsigned int)MouseCursor::MOUSE_CURSOR_IBEAM);
        isHovered = true;
        break;
      }
    if (!isHovered)
      cursorBitState.unset((unsigned int)MouseCursor::MOUSE_CURSOR_IBEAM);
  }

  // Handling cursor state
  if (!cursorBitState.value)
    SetMouseCursor(MouseCursor::MOUSE_CURSOR_DEFAULT);
  else {
    const int loopCount = sizeof(cursorBitState.value) * 8;
    for (int i = loopCount - 1; i >= 0; i--)
      if (cursorBitState.isSet(i)) {
        SetMouseCursor((MouseCursor)i);
      }

    cursorBitState.reset();
  }
}
