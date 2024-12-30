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

string GUI::getStageMessage(OrderStageState stage) const {
  switch (stage) {
    case SELECT_ITEM:
      return "Select items to add to your cart.";
    case ADDRESS_INPUT:
      return "Enter your address details.";
    case PAYMENT_METHOD:
      return "Choose your payment method.";
    case SHIPPING:
      return "Shipping your order...";
    case COMPLETED:
      return "Order completed!";
    default:
      return "Unknown stage";
  }
}

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

void GUI::setShowCTA(bool _) { isShowCTA = _; }

void GUI::setCTARec(const Rectangle& rec) { ctaRec = rec; }

void GUI::setPaymentMethodChanged(bool flag) {
  isPaymentMethodChanged = flag;

  if (flag) loadingStates.set((unsigned int)LoadingState::PAYMENT_QR);
}

void GUI::setItems(const vector<Item>& _) { items = _; }

void GUI::setCoupons(const vector<Coupon>& _) { coupons = _; }

void GUI::init() {
  SetTraceLogCallback(utils::log::CustomLog);
  SetTargetFPS(TARGET_FPS);
  InitWindow(SCREEN_SIZE.width, SCREEN_SIZE.height, APP_TITLE.c_str());
  SetExitKey(KEY_NULL);

  loadingStates.set((unsigned int)LoadingState::PAYMENT_QR);
}

void GUI::stopConfetti() {
  if (confettiParticles) {
    delete confettiParticles;
    confettiParticles = nullptr;
  }
}

void GUI::cursorUpdate(Application* app, const GUIScreen& guiScreen) {
  switch (guiScreen) {
    case GUIScreen::MAIN:
      if (app->getOrderContext().currentStage ==
          OrderStageState::ADDRESS_INPUT) {
        vector<Rectangle> recs = {ADDR_INP_REC, PHONE_INP_REC};

        for (const Rectangle& _ : recs)
          if (CheckCollisionPointRec(GetMousePosition(), _)) {
            cursorBitState.set((unsigned int)MouseCursor::MOUSE_CURSOR_IBEAM);
            break;
          }
      }

      break;

    default:
      break;
  }

  for (GuiButton* btn : GuiButton::getButtons())
    if (btn->isHovered()) {
      cursorBitState.set((unsigned int)MouseCursor::MOUSE_CURSOR_POINTING_HAND);
      break;
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

void GUI::processStageBacking(const OrderStageState& curStage) {
  if (curStage == OrderStageState::SELECT_ITEM) {
    setPaymentMethodChanged(true);
  }
}
