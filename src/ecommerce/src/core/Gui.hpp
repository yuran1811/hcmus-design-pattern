#pragma once

#include <cmath>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>

#include "raylib.h"

#include "../components/index.hpp"
#include "../shared/index.hpp"
#include "../utils/index.hpp"

#include "Payment.hpp"
#include "Order.hpp"

using std::function;
using std::pair;
using std::vector;

class OrderContext;

class GUI {
 public:
  const float leftAlign = 50.f;
  const float leftMidAlign = 140.f;
  const float midAlign = 300.f;

  const Vector2 ADDR_INP_POS = {leftAlign, 140};
  const Vector2 ADDR_INP_SIZE = {440, 40};
  const Rectangle ADDR_INP_REC = {ADDR_INP_POS.x, ADDR_INP_POS.y,
                                  ADDR_INP_SIZE.x, ADDR_INP_SIZE.y};

  const Vector2 PHONE_INP_POS = {ADDR_INP_POS.x, ADDR_INP_POS.y + 100};
  const Vector2 PHONE_INP_SIZE = {160, 40};
  const Rectangle PHONE_INP_REC = {PHONE_INP_POS.x, PHONE_INP_POS.y,
                                   PHONE_INP_SIZE.x, PHONE_INP_SIZE.y};

  const float ORDER_PROG_ITEM_SIZE = 36.f;
  const Vector2 ORDER_PROG_POS = {
      120.f, SCREEN_SIZE.height - 110 + ORDER_PROG_ITEM_SIZE / 2};

 private:
  vector<Item> items;
  vector<Coupon> coupons;

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
  Rectangle ctaRec = {0, SCREEN_SIZE.height - 50, 0, 40};

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
  string getStageMessage(OrderStageState) const;
  static vector<Texture2D> &getTextureCollection();

  void incFrameCounter();
  void incFrameTimer();
  void setShowCTA(bool);
  void setCTARec(const Rectangle &);
  void setPaymentMethodChanged(bool);
  void setItems(const vector<Item> &);
  void setCoupons(const vector<Coupon> &);

  void init();
  void stopConfetti();
  void cursorUpdate(const OrderStageState &);
  void processStageBacking(const OrderStageState &);

  void render(const OrderStageState &, function<void()>);
  void renderHeader(const string &);
  void renderOrderProgress(const OrderStageState &);
  void renderCurrentTime();
  void renderCTAButton(const string &);
  void renderStageMessage(const string &);

  void renderCoupons(function<void(const string &)> &);
  void renderSelectItem(CartType &, Price &, Price &);

  void renderInput(const InputType &, const string &, const string &,
                   const Vector2 &, const Vector2 &);
  void renderAddressInput(const string &);
  void renderPhoneInput(const string &);
  void renderAddressInfo(const string &, const string &);

  void renderPaymentQR(const string &, const PaymentMethod &, const Price &,
                       function<void()> &);
  void renderPaymentSuccessInfo(const string &, const Price &);
  void renderPaymentMethod(const OrderContext &, function<void()> &);

  void renderShipping();

  void renderOrderInfoText(const string &, const string &, const float &,
                           const Color &, const Color &);
  void renderCompleted(const string &, const string &, Price &, const string &,
                       const PaymentMethod &);

  bool selectItemHandler(CartType &, Price &);

  bool inputHandler(const InputType &, string &, int, const Range<int> &,
                    const Rectangle &);
  bool addressHandler(string &);
  bool phoneHandler(string &);
  bool addressInfoHandler(string &, string &);

  bool paymentMethodHandler(PaymentMethod &, const Price &);

  bool isCTAClicked() const;
  bool isBackProgressClicked() const;
};

#include "Stage.hpp"
