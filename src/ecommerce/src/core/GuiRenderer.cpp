#include "Gui.hpp"

void GUI::render(const OrderStageState& curStage, function<void()> callback) {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  renderHeader("Interactive E-Commerce Order Processing");
  renderOrderProgress(curStage);
  renderCurrentTime();
  callback();

  cursorUpdate(curStage);
  GuiButton::eventsHandler();
  GuiButton::releaseButtons();

  EndDrawing();
}

void GUI::renderHeader(const string& s) {
  DrawText(s.c_str(), leftMidAlign, 15, 20, BLUE);
  DrawLine(0, 45, SCREEN_SIZE.width, 45, LIGHTGRAY);
}

void GUI::renderOrderProgress(const OrderStageState& curStage) {
  const int gap = (SCREEN_SIZE.width - ORDER_PROG_POS.x * 2 -
                   (ORDER_STAGE_COUNT * ORDER_PROG_ITEM_SIZE / 2)) /
                  (ORDER_STAGE_COUNT + 1);

  for (int i = 0; i < ORDER_STAGE_COUNT; i++) {
    const int x = ORDER_PROG_POS.x + gap + (ORDER_PROG_ITEM_SIZE / 2 + gap) * i;
    const Color color = (i == (int)curStage && curStage ^ COMPLETED) ? SKYBLUE
                        : (curStage == COMPLETED || i < (int)curStage)
                            ? GREEN
                            : LIGHTGRAY;

    if (i && curStage >= i)
      DrawLineEx({float(x - gap), float(ORDER_PROG_POS.y)},
                 {float(x - ORDER_PROG_ITEM_SIZE / 2), float(ORDER_PROG_POS.y)},
                 3, color);

    DrawCircle(x, ORDER_PROG_POS.y, ORDER_PROG_ITEM_SIZE / 2, color);
    DrawText(to_string(i + 1).c_str(), x - 4, ORDER_PROG_POS.y - 8, 20,
             DARKGRAY);
  }

  // Render back arrow button
  if (0 < curStage && curStage < COMPLETED) {
    isShowBackProgress = true;
    (new GuiButton("<",
                   {ORDER_PROG_POS.x - ORDER_PROG_ITEM_SIZE / 2,
                    ORDER_PROG_POS.y - ORDER_PROG_ITEM_SIZE / 2,
                    ORDER_PROG_ITEM_SIZE, ORDER_PROG_ITEM_SIZE},
                   RAYWHITE, DARKGRAY, 14, 10))
        ->render(GetFontDefault(), 0);
  } else {
    isShowBackProgress = false;
  }
}

void GUI::renderCurrentTime() {
  DrawText(utils::getDateTimeString().c_str(), leftAlign, ctaRec.y + 12, 20,
           DARKGRAY);
}

void GUI::renderCTAButton(const string& message) {
  const int padding = 12;
  const float ctaRecWidth = MeasureText(message.c_str(), 20) + (padding << 1);
  const float rightAlign = SCREEN_SIZE.width - ctaRecWidth - 10;

  setCTARec({rightAlign, ctaRec.y, ctaRecWidth, ctaRec.height});

  (new GuiButton(message, ctaRec, RAYWHITE, DARKGRAY, padding))
      ->render(GetFontDefault(), 0);
}

void GUI::renderStageMessage(const string& s) {
  DrawText((">>> " + s).c_str(), leftAlign, 60, 20,
           utils::color::calcBreathColor(LIME, frameTimer * 3.75f));
}

void GUI::renderCoupons(function<void(const string&)>& onClick) {
  int couponYPos = 165 + 40 * items.size();

  DrawText("Coupons:", leftAlign, couponYPos - 30, 20, DARKGRAY);

  for (int i = 0; i < coupons.size(); i++) {
    const Coupon& coupon = coupons[i];

    (new GuiButton(coupon.code,
                   Rectangle{leftAlign, (40.f * i) + couponYPos,
                             20.f + MeasureText(coupon.code.c_str(), 20), 30},
                   LIGHTGRAY, DARKGRAY, 10, 5))
        ->setEvent(GuiButton::EVENT::CLICK, [&]() { onClick(coupon.code); })
        ->render(GetFontDefault(), 0);
  }
}

void GUI::renderSelectItem(CartType& cart, Price& totalCost, Price& discount) {
  int cartYPos = 130;

  DrawText("Product:", leftAlign, cartYPos - 30, 20, DARKGRAY);
  for (int i = 0; i < items.size(); i++)
    (new GuiButton(items[i].name,
                   Rectangle{leftAlign, (40.f * i) + cartYPos, 150, 30},
                   DARKGRAY, LIGHTGRAY, 10, 5))
        ->render(GetFontDefault(), 0);

  isShowCTA = !cart.empty();
  if (isShowCTA) renderCTAButton("Fill the address info");

  const int quantityGap = 35;

  DrawText("Cart:", midAlign, cartYPos - 30, 20, DARKGRAY);

  for (auto& entry : cart) {
    DrawText(
        ("x" + to_string(entry.second.second) + " " + entry.first + " - $" +
         (entry.second.first.price * entry.second.second).format())
            .c_str(),
        midAlign + quantityGap * 2 + 10, cartYPos + 7, 20, DARKBLUE);

    Rectangle rec = {midAlign, float(cartYPos), 30, 30};
    (new GuiButton("-", rec, GRAY, LIGHTGRAY, 10, 7))
        ->render(GetFontDefault(), 0)
        ->setEvent(GuiButton::EVENT::CLICK, [&]() {
          if (entry.second.second > 0) {
            totalCost -= entry.second.first.price;
            if (--entry.second.second <= 0) cart.erase(entry.first);
          }
        });

    rec.x = midAlign + quantityGap;
    (new GuiButton("+", rec, GRAY, LIGHTGRAY, 10, 7))
        ->render(GetFontDefault(), 0)
        ->setEvent(GuiButton::EVENT::CLICK, [&]() {
          entry.second.second++;
          totalCost += entry.second.first.price;
        });

    cartYPos += 40;
  }

  if (cart.size()) {
    const Rectangle clearBtnRec = {midAlign, float(cartYPos), 75, 30};
    (new GuiButton("Clear", clearBtnRec, WHITE, MAROON, 10, 6))
        ->setEvent(GuiButton::EVENT::CLICK,
                   [&]() {
                     cart.clear();
                     totalCost = 0;
                   })
        ->render(GetFontDefault(), 0);

    cartYPos += 45;
    DrawLine(midAlign, cartYPos, 710, cartYPos, LIGHTGRAY);
    cartYPos += 15;

    if (discount > 0) {
      DrawText(("Discount: $" + discount.format()).c_str(), midAlign, cartYPos,
               20, GRAY);
      cartYPos += 30;
    }

    DrawText(("Total: $" + totalCost.format()).c_str(), midAlign, cartYPos, 20,
             DARKGREEN);
  } else {
    DrawText("Cart is empty!", midAlign, cartYPos, 20, GRAY);
  }
}

void GUI::renderInput(const InputType& type, const string& info,
                      const string& value, const Vector2& pos,
                      const Vector2& size) {
  DrawText(info.c_str(), pos.x, pos.y - 30, 20, DARKGRAY);

  const float inputWidth =
      std::min(size.x, (value.size() ? 35.f + MeasureText(value.c_str(), 20)
                                     : 20.f + MeasureText("_", 30)));

  if (inputActiveStates[int(type)].isSet((unsigned int)InputState::ACTIVE)) {
    DrawRectangleV(pos, {inputWidth, size.y}, SKYBLUE);
    DrawText(value.c_str(), pos.x + 10, pos.y + 10, 20, DARKBLUE);

    if ((getFrameCounter() / 30) % 2)
      DrawText("_", pos.x + 10 + MeasureText(value.c_str(), 20), pos.y + 6, 30,
               DARKBLUE);
  } else {
    DrawText(value.c_str(), pos.x + 10, pos.y + 10, 20, GRAY);

    if (value.empty())
      DrawText("Click to type", pos.x + 10, pos.y + 10, 20, GRAY);
  }
}

void GUI::renderAddressInput(const string& address) {
  renderInput(InputType::ADDRESS, "Your Address:", address, ADDR_INP_POS,
              ADDR_INP_SIZE);
}

void GUI::renderPhoneInput(const string& phone) {
  renderInput(InputType::PHONE, "Your Phone Number:", phone, PHONE_INP_POS,
              PHONE_INP_SIZE);
}

void GUI::renderAddressInfo(const string& address, const string& phone) {
  renderAddressInput(address);
  renderPhoneInput(phone);

  isShowCTA = !address.empty() && phone.length() == 10;
  if (isShowCTA) renderCTAButton("Shipping Info");
}

void GUI::renderShipping() {
  DrawText(("Delivery Cost is default = " + Price(2425, 2).format()).c_str(),
           leftAlign, 100, 20, DARKGRAY);

  isShowCTA = true;
  renderCTAButton("Goto Payment");
}

void GUI::renderPaymentQR(const string& content,
                          const PaymentMethod& paymentMethod,
                          const Price& price,
                          function<void()>& onPaymentSuccess) {
  const int __leftAlign = paymentMethodRec.x + paymentMethodRec.width + 50;

  DrawText(("Total: $" + price.format()).c_str(), __leftAlign,
           paymentMethodRec.y + 35, 20, DARKGREEN);

  if (paymentMethod == COD) {
    DrawText("Cash on Delivery", __leftAlign, paymentMethodRec.y, 20, DARKGRAY);
    loadingStates.unset((unsigned int)LoadingState::PAYMENT_QR);

    if (!isShowCTA)
      (new GuiButton(
           "Confirm",
           {paymentMethodRec.x + paymentMethodRec.width + 50,
            paymentMethodRec.y + 70,
            15.f * 2 + MeasureText((char*)"Confirm", 20), 20.f + 20.f},
           BLACK, LIGHTGRAY, 15, 10))
          ->setEvent(GuiButton::EVENT::CLICK,
                     [&]() {
                       isShowCTA = true;
                       onPaymentSuccess();
                     })
          ->render(GetFontDefault(), 0);
    return;
  }

  DrawText("Scan the QR code to make payment", __leftAlign, paymentMethodRec.y,
           20, GRAY);

  const string paymentQRFileName = "payment";
  const string qrPath =
      isPaymentMethodChanged
          ? utils::qrcode::saveQRCode(paymentQRFileName, content, 270)
          : utils::getResourcePath(AssetFolder::QRCODE, paymentQRFileName,
                                   AssetType::PNG);
  setPaymentMethodChanged(false);

  vector<Texture2D>& itemTextures = getTextureCollection();

  if (frameCounter == 0) {
    loadingStates.unset((unsigned int)LoadingState::PAYMENT_QR);

    Image qrImage = LoadImage(qrPath.c_str());

    if (itemTextures[(int)ItemTexture::PAYMENT_QR].id)
      UnloadTexture(itemTextures[(int)ItemTexture::PAYMENT_QR]);
    itemTextures[(int)ItemTexture::PAYMENT_QR] = LoadTextureFromImage(qrImage);

    UnloadImage(qrImage);
  }

  if (loadingStates.isSet((unsigned int)LoadingState::PAYMENT_QR)) {
    DrawText("Loading QR Code...", __leftAlign, paymentMethodRec.y + 70, 20,
             DARKGRAY);
  } else {
    DrawTexture(itemTextures[(int)ItemTexture::PAYMENT_QR], __leftAlign,
                paymentMethodRec.y + 70, RAYWHITE);

    if (!isShowCTA)
      (new GuiButton(
           "Confirm",
           {paymentMethodRec.x + paymentMethodRec.width + 50,
            paymentMethodRec.y + 270.f + 90.f,
            15.f * 2 + MeasureText((char*)"Confirm", 20), 20.f + 20.f},
           BLACK, LIGHTGRAY, 15, 10))
          ->setEvent(GuiButton::EVENT::CLICK,
                     [&]() {
                       isShowCTA = true;
                       onPaymentSuccess();
                     })
          ->render(GetFontDefault(), 0);
  }
}

void GUI::renderPaymentSuccessInfo(const string& msg, const Price& price) {
  const float __l = paymentMethodRec.x + paymentMethodRec.width + 50;
  const float __w = SCREEN_SIZE.width - __l;
  const float __h = utils::calcTextWrapHeight(msg, SCREEN_SIZE.width - __l);

  GuiTextWrap(GuiText(msg, {0, 0}, 20, DARKGRAY),
              {__l, paymentMethodRec.y, __w, __h}, LIGHTGRAY,
              {__w, __h, __w, __h})
      .rawRender(GetFontDefault());

  DrawText("Payment Successful!", __l, paymentMethodRec.y + __h + 32, 20,
           DARKGREEN);
  DrawText("Thank you for shopping with us!", __l,
           paymentMethodRec.y + __h + 52, 20, DARKGREEN);
}

void GUI::renderPaymentMethod(const OrderContext& ctx,
                              function<void()>& onPaymentSuccess) {
  Rectangle rec = paymentMethodRec;

  for (int i = 0; i < PAYMENT_METHOD_COUNT; i++) {
    if (i) rec.y += 50;

    (new GuiButton(PAYMENT_METHODS[i], rec, BLACK,
                   ctx.paymentMethod == i ? GRAY : LIGHTGRAY, 15, 10))
        ->render(GetFontDefault(), 0);
  }

  if (!isShowCTA)
    renderPaymentQR(
        PAYMENT_METHODS[ctx.paymentMethod] + "::" + ctx.totalCost.format(),
        ctx.paymentMethod, ctx.totalCost, onPaymentSuccess);
  else {
    renderPaymentSuccessInfo(ctx.paymentInfo.first, ctx.paymentInfo.second);
    renderCTAButton("Finish");
  }
}

void GUI::renderOrderInfoText(const string& label, const string& value,
                              const float& posY, const Color& labelColor,
                              const Color& valueColor) {
  const int infoOffset = 78;

  DrawRectangleRounded(
      {leftMidAlign + infoOffset + MeasureText(label.c_str(), 20), posY - 5,
       20.f + MeasureText(value.c_str(), 20), 30},
      .25f, 40, labelColor);
  DrawText((label + " " + value).c_str(), leftMidAlign + infoOffset, posY, 20,
           valueColor);
}

void GUI::renderCompleted(const string& orderID, const string& timestamp,
                          Price& totalCost, const string& address,
                          const PaymentMethod& paymentMethod) {
  if (!confettiParticles) {
    confettiParticles =
        new ConfettiParticles(400, SCREEN_SIZE.width, SCREEN_SIZE.height);
    confettiParticles->setCanonPosition(
        {SCREEN_SIZE.width / 2, SCREEN_SIZE.height});
    confettiParticles->generateSetOfConfetties();

    isConfettiActive = true;
  }

  confettiParticles->update();
  confettiParticles->render();

  DrawText("Thank you for your order!", leftMidAlign + 50, 120, 25, DARKBLUE);
  DrawText("Your order has been completed!", leftMidAlign + 55, 160, 20, GRAY);

  renderOrderInfoText("Order ID: ", orderID.substr(0, 8), 250, LIGHTGRAY,
                      DARKGRAY);
  renderOrderInfoText("Order Date: ", timestamp, 290, LIGHTGRAY, DARKGRAY);

  renderOrderInfoText("Shipped to: ", address, 330, LIGHTGRAY, DARKGRAY);
  renderOrderInfoText("Payment Method: ", PAYMENT_METHODS[paymentMethod], 370,
                      SKYBLUE, BLUE);
  renderOrderInfoText("Total Cost: ", "$" + totalCost.format(), 410, GREEN,
                      DARKGREEN);

  isShowCTA = true;
  renderCTAButton("Place Another Order");
}
