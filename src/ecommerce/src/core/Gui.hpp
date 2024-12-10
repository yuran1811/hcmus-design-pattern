#pragma once

#include <cmath>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <unordered_map>

#include "raylib.h"

#include "../components/index.hpp"
#include "../shared/index.hpp"
#include "../utils/index.hpp"

#include "Payment.hpp"
#include "Order.hpp"
#include "Stage.hpp"

using std::function;
using std::pair;
using std::unordered_map;
using std::vector;

class GUI {
 public:
  const float leftAlign = 50.f;
  const float leftMidAlign = 140.f;

  const Vector2 ADDR_INP_POS = {leftAlign, 140};
  const Vector2 ADDR_INP_SIZE = {440, 40};
  const Rectangle ADDR_INP_REC = {ADDR_INP_POS.x, ADDR_INP_POS.y,
                                  ADDR_INP_SIZE.x, ADDR_INP_SIZE.y};

  const Vector2 PHONE_INP_POS = {ADDR_INP_POS.x, ADDR_INP_POS.y + 100};
  const Vector2 PHONE_INP_SIZE = {160, 40};
  const Rectangle PHONE_INP_REC = {PHONE_INP_POS.x, PHONE_INP_POS.y,
                                   PHONE_INP_SIZE.x, PHONE_INP_SIZE.y};

  const float ORDER_PROG_ITEM_SIZE = 36.f;
  const Vector2 ORDER_PROG_POS = {120.f, 470 + ORDER_PROG_ITEM_SIZE / 2};

 private:
  enum class ItemTexture { PAYMENT_QR };

  enum class LoadingState { PAYMENT_QR };
  BitState loadingStates;

  enum class InputType { ADDRESS, PHONE };
  enum class InputState { DISABLE, INACTIVE, ACTIVE, HOVER };
  vector<BitState> inputActiveStates = vector<BitState>(2);

  BitState cursorBitState;
  Vector2 lastCurPos;

  ConfettiParticles *confettiParticles = nullptr;

  const Rectangle paymentMethodRec = {leftAlign, 100, 150, 40};
  Rectangle ctaRec = {0, 530, 0, 40};

  bool isShowCTA = false;
  bool isShowBackProgress = false;
  bool isPaymentMethodChanged = false;
  bool isConfettiActive = false;

  int frameCounter = 0;
  float frameTimer = 0.f;

 public:
  GUI();
  ~GUI();

  bool getConfettiActive() const;
  int getFrameCounter() const;
  float getFrameTimer() const;
  static vector<Texture2D> &getTextureCollection();

  void incFrameCounter();
  void incFrameTimer();
  void setCTARec(const Rectangle &);
  void setPaymentMethodChanged(bool);

  void init();

  void cursorUpdate(const OrderStageState &);
  void stopConfetti();
  void processStageBacking(const OrderStageState &);

  void render(const OrderStageState &, function<void()>);
  void renderHeader(const string &);
  void renderOrderProgress(const OrderStageState &);
  void renderCTAButton(const string &);
  void renderStageMessage(const string &);

  void renderSelectItem(const vector<Item> &, CartType &, Price &);

  void renderInput(const InputType &, const string &, const string &,
                   const Vector2 &, const Vector2 &);
  void renderAddressInput(const string &);
  void renderPhoneInput(const string &);
  void renderAddressInfo(const string &, const string &);

  void renderPaymentQR(const string &, const PaymentMethod &, const Price &);
  void renderPaymentMethod(const PaymentMethod &, const Price &);

  void renderPayment();

  void renderPackaging();

  void renderShipping();

  void renderOrderInfoText(const string &, const string &, const float &,
                           const Color &, const Color &);
  void renderCompleted(Price &, const string &, const PaymentMethod &);

  bool selectItemHandler(const vector<Item> &, CartType &, Price &);
  bool inputHandler(const InputType &, string &, int, const Range<int> &,
                    const Rectangle &);
  bool addressHandler(string &);
  bool phoneHandler(string &);
  bool paymentMethodHandler(PaymentMethod &, const Price &);

  bool isCTAClicked() const;
  bool isBackProgressClicked() const;
};
