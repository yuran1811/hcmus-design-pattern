#include "Gui.hpp"

GUI::GUI() {
  for (BitState& state : inputActiveStates) state.reset();
}

GUI::~GUI() {
  vector<Texture2D>& itemTextures = getTextureCollection();
  for (Texture2D& texture : itemTextures) UnloadTexture(texture);

  CloseWindow();

  delete confettiParticles;
}

bool GUI::getConfettiActive() const { return isConfettiActive; }

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

void GUI::setPaymentMethodChanged(bool flag) {
  isPaymentMethodChanged = flag;

  if (flag) loadingStates.set((unsigned int)LoadingState::PAYMENT_QR);
}

void GUI::init() {
  SetTraceLogCallback(utils::log::CustomLog);
  SetTargetFPS(TARGET_FPS);
  InitWindow(SCREEN_SIZE.width, SCREEN_SIZE.height, APP_TITLE.c_str());
  SetExitKey(KEY_NULL);

  loadingStates.set((unsigned int)LoadingState::PAYMENT_QR);
}

void GUI::cursorUpdate(const OrderStageState& curStage) {
  for (GuiButton* btn : GuiButton::getButtons())
    if (btn->isHovered()) {
      cursorBitState.set((unsigned int)MouseCursor::MOUSE_CURSOR_POINTING_HAND);
      break;
    }

  if (curStage == OrderStageState::ADDRESS_INPUT) {
    vector<Rectangle> recs = {ADDR_INP_REC, PHONE_INP_REC};

    for (const Rectangle& _ : recs)
      if (CheckCollisionPointRec(GetMousePosition(), _)) {
        cursorBitState.set((unsigned int)MouseCursor::MOUSE_CURSOR_IBEAM);
        break;
      }
  }

  // Handling cursor state
  if (!cursorBitState.value)
    SetMouseCursor(MouseCursor::MOUSE_CURSOR_DEFAULT);
  else {
    const int loopCount = sizeof(cursorBitState.value) * 8;
    for (int i = 0; i < loopCount; i++)
      if (cursorBitState.isSet(i)) {
        SetMouseCursor((MouseCursor)i);
        break;
      }

    cursorBitState.reset();
  }
}

void GUI::stopConfetti() {
  if (confettiParticles) {
    delete confettiParticles;
    confettiParticles = nullptr;
  }
}

void GUI::processStageBacking(const OrderStageState& curStage) {
  if (curStage == OrderStageState::SELECT_ITEM) {
    setPaymentMethodChanged(true);
  }
}
