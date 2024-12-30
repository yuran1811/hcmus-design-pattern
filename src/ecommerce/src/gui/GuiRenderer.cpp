#include "Gui.hpp"

void GUI::render(Application* app, function<void()> renderer,
                 function<void()> handler) {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  renderSwitchScreenButton();

  renderer();
  handler();

  cursorUpdate(app, ScreenManager::getInstance().getCurrentScreen());
  GuiButton::eventsHandler();
  GuiButton::releaseButtons();

  EndDrawing();
}

void GUI::renderHeader(const string& s) {
  const float xPos = (SCREEN_SIZE.width - MeasureText(s.c_str(), 20)) / 2;

  DrawText(s.c_str(), xPos, 15, 20, BLUE);
  DrawLine(0, 45, SCREEN_SIZE.width, 45, LIGHTGRAY);
}

void GUI::renderOrderProgress(const OrderStageState& curStage,
                              bool isCompleted) {
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
  if (0 < curStage && curStage < COMPLETED && !isCompleted) {
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

void GUI::renderSwitchScreenButton() {
  ScreenManager& sm = ScreenManager::getInstance();
  const string btnText = sm.getCurrentScreen() == GUIScreen::MAIN
                             ? "View Archived Order"
                             : "Back to Main";

  const float btnWidth = MeasureText(btnText.c_str(), 20) + 20;
  const float btnHeight = 40;
  const float btnX = leftAlign;
  const float btnY = ctaRec.y;

  (new GuiButton(btnText, {btnX, btnY, btnWidth, btnHeight}, RAYWHITE, DARKGRAY,
                 10))
      ->setEvent(GuiButton::EVENT::CLICK,
                 [&]() {
                   sm.switchScreen(sm.getCurrentScreen() == GUIScreen::MAIN
                                       ? GUIScreen::ARCHIVED
                                       : GUIScreen::MAIN);
                 })
      ->render(GetFontDefault(), 0);
}

void GUI::renderArchivedOrder(ArchivedOrder* order, const Vector2& pos) {
  DrawRectangleLinesEx({pos.x, pos.y, SCREEN_SIZE.width - 2 * pos.x, 110}, 2,
                       DARKGRAY);

  const char* orderID = (order->getOrderId().substr(0, 28)).c_str();
  const char* orderDate = (order->getOrderDate()).c_str();
  const char* orderAddress = (order->getAddress().substr(0, 58)).c_str();
  const char* orderPaymentMethod =
      (PAYMENT_METHODS[order->getPaymentMethod()]).c_str();

  const float originTopOffset = pos.y + 5;
  const float originLeftOffset = pos.x + 5;

  float topOffset = originTopOffset;
  float leftOffset = originLeftOffset;
  float segmentWidth = MeasureText(orderID, 20) + 20;

  DrawRectangleRounded({leftOffset, topOffset, segmentWidth, 30}, 0.25f, 40,
                       PURPLE);
  DrawText(orderID, leftOffset + 10, topOffset + 5, 20, DARKPURPLE);
  leftOffset += segmentWidth + 20;

  segmentWidth = MeasureText(orderDate, 20) + 20;
  DrawRectangleRounded({SCREEN_SIZE.width - originLeftOffset - segmentWidth,
                        topOffset, segmentWidth, 30},
                       0.25f, 40, LIGHTGRAY);
  DrawText(orderDate, SCREEN_SIZE.width - originLeftOffset - segmentWidth + 10,
           topOffset + 5, 20, DARKGRAY);

  leftOffset = originLeftOffset;
  topOffset += 40;

  segmentWidth = MeasureText(orderAddress, 20) + 20;
  DrawText(orderAddress, leftOffset + 10, topOffset, 20, DARKGRAY);

  topOffset += 30;

  segmentWidth = MeasureText(orderPaymentMethod, 20) + 20;
  DrawRectangleRounded({leftOffset, topOffset, segmentWidth, 30}, 0.25f, 40,
                       SKYBLUE);
  DrawText(orderPaymentMethod, leftOffset + 10, topOffset + 5, 20, DARKBLUE);
  leftOffset += segmentWidth + 5;

  segmentWidth =
      MeasureText(("$" + order->getTotalCost().format()).c_str(), 20) + 20;
  DrawRectangleRounded({leftOffset, topOffset, segmentWidth, 30}, 0.25f, 40,
                       GOLD);
  DrawText(("$" + order->getTotalCost().format()).c_str(), leftOffset + 10,
           topOffset + 5, 20, DARKBROWN);
}

void GUI::renderArchivedOrderList(
    const vector<unique_ptr<ArchivedOrder>>& list) {
  if (list.empty())
    return DrawText("No archived orders found!", leftAlign, 100, 20, GRAY);

  float yPos = 0.f;
  archivedOrdersContainer->updateContentHeight(125 * list.size());
  archivedOrdersContainer->render([&]() {
    const Vector2 offset = archivedOrdersContainer->getRenderPosition();

    for (auto it = list.rbegin(); it != list.rend(); ++it) {
      renderArchivedOrder((*it).get(), {leftAlign, yPos + offset.y});
      yPos += 125.f;
    }
  });
}

void GUI::renderCoupons(function<void(const string&)>& onClick) {
  int couponYPos = 165 + 40 * items.size();

  DrawText("Coupons:", leftAlign, couponYPos - 30, 20, DARKGRAY);

  for (int i = 0; i < coupons.size(); i++) {
    const Coupon& coupon = coupons[i];

    CouponSystem* cs = CouponSystem::getInstance();
    if (!cs->validateCoupon(coupon.code, Price(1, 0)).first) continue;

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
  Rectangle iconButtonRec = {midAlign, float(cartYPos), 30, 30};

  DrawText("Cart:", midAlign, cartYPos - 30, 20, DARKGRAY);

  for (auto& entry : cart) {
    DrawText(
        ("x" + to_string(entry.second.second) + " " + entry.first + " - $" +
         (entry.second.first.price * entry.second.second).format())
            .c_str(),
        midAlign + quantityGap * 2 + 10, cartYPos + 7, 20, DARKBLUE);

    (new GuiButton("-", iconButtonRec, GRAY, LIGHTGRAY, 10, 7))
        ->setEvent(GuiButton::EVENT::CLICK,
                   [&]() {
                     if (entry.second.second > 0) {
                       totalCost -= entry.second.first.price;
                       if (--entry.second.second <= 0) cart.erase(entry.first);
                     }
                   })
        ->render(GetFontDefault(), 0);

    iconButtonRec.x = midAlign + quantityGap;
    (new GuiButton("+", iconButtonRec, GRAY, LIGHTGRAY, 10, 7))
        ->setEvent(GuiButton::EVENT::CLICK,
                   [&]() {
                     entry.second.second++;
                     totalCost += entry.second.first.price;
                   })
        ->render(GetFontDefault(), 0);

    cartYPos += 40;
    iconButtonRec.x = midAlign;
    iconButtonRec.y = cartYPos;
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

      iconButtonRec.x = midAlign - 35;
      iconButtonRec.y = cartYPos - 5;
      (new GuiButton("-", iconButtonRec, GRAY, LIGHTGRAY, 10, 7))
          ->setEvent(GuiButton::EVENT::CLICK, [&]() { discount = 0; })
          ->render(GetFontDefault(), 0);
      cartYPos += 30;

      DrawText(("Total: $" + totalCost.format()).c_str(), midAlign, cartYPos,
               20, BLUE);
      cartYPos += 30;
    }

    DrawText(("Cost: $" +
              (totalCost > discount ? totalCost - discount : Price(0)).format())
                 .c_str(),
             midAlign, cartYPos, 20, DARKGREEN);
  } else {
    DrawText("Cart is empty!", midAlign, cartYPos, 20, GRAY);
  }
}

void GUI::renderInput(const InputType& type, const string& info,
                      const string& value, const Vector2& pos,
                      const Vector2& size) {
  DrawText(info.c_str(), pos.x, pos.y - 30, 20, DARKGRAY);

  const int maxDisplayLength = size.x / MeasureText("X", 20) - 3;
  const char* displayValue = (value.size() > maxDisplayLength
                                  ? value.substr(0, maxDisplayLength) + "..."
                                  : value)
                                 .c_str();

  const float inputWidth =
      std::min(size.x, (value.size() ? 35.f + MeasureText(displayValue, 20)
                                     : 20.f + MeasureText("_", 30)));

  if (inputActiveStates[int(type)].isSet((unsigned int)InputState::ACTIVE)) {
    DrawRectangleV(pos, {inputWidth, size.y}, SKYBLUE);
    DrawText(displayValue, pos.x + 10, pos.y + 10, 20, DARKBLUE);

    if ((getFrameCounter() / 30) % 2)
      DrawText("_", pos.x + 10 + MeasureText(displayValue, 20), pos.y + 6, 30,
               DARKBLUE);
  } else {
    DrawText(displayValue, pos.x + 10, pos.y + 10, 20, GRAY);

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

void GUI::renderShippingProvider(
    const string& curProvider,
    function<void(const string&)>& shippingProviderHandler) {
  map<string, Price> providers = ExpressDeliveryDecorator::availableProviders;

  Rectangle rec = {leftAlign, 150, 0, 40};

  for (const auto& [provider, price] : providers) {
    const string msg = provider + " - " + price.format();

    rec.width = 15.f * 2 + MeasureText(msg.c_str(), 20);
    (new GuiButton(msg, rec, curProvider == provider ? DARKBLUE : BLACK,
                   curProvider == provider ? SKYBLUE : LIGHTGRAY, 15, 10))
        ->setEvent(GuiButton::EVENT::CLICK,
                   [&]() { shippingProviderHandler(provider); })
        ->render(GetFontDefault(), 0);

    rec.y += 50;
  }
}

void GUI::renderShipping(
    const OrderContext& ctx,
    function<void(const string&)>& shippingProviderHandler) {
  DrawText("Select Shipping Provider:", leftAlign, 100, 20, DARKGRAY);

  renderShippingProvider(ctx.deliveryProvider, shippingProviderHandler);

  isShowCTA = true;
  renderCTAButton("Goto Payment");
}

void GUI::renderPaymentQR(const string& content,
                          const PaymentMethod& paymentMethod,
                          const Price& price,
                          function<void()>& onPaymentSuccess) {
  const float __leftAlign = paymentMethodRec.x + paymentMethodRec.width + 50;

  DrawText(("Total: $" + price.format()).c_str(), int(__leftAlign),
           paymentMethodRec.y + 35, 20, DARKGREEN);

  if (paymentMethod == COD) {
    DrawText("Cash on Delivery", __leftAlign, paymentMethodRec.y, 20, DARKGRAY);
    loadingStates.unset((unsigned int)LoadingState::PAYMENT_QR);

    if (!isShowCTA)
      (new GuiButton(
           "Confirm",
           {__leftAlign, paymentMethodRec.y + 70,
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
           {__leftAlign +
                MeasureText(("Total: $" + price.format()).c_str(), 20) + 20.f,
            paymentMethodRec.y + 25,
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
  const float __w = SCREEN_SIZE.width - __l - leftAlign;
  const float __h = utils::calcTextWrapHeight(msg, __w);

  GuiTextWrap(GuiText(msg, {0, 0}, 20, DARKGRAY),
              {__l, paymentMethodRec.y, __w, __h}, LIGHTGRAY,
              {__w, __h, __w, __h})
      .rawRender(GetFontDefault());

  DrawText("Payment Successful!", __l, paymentMethodRec.y + __h + 32, 20,
           DARKGREEN);
  DrawText("Thank you for shopping with us!", __l,
           paymentMethodRec.y + __h + 52, 20, DARKGREEN);
}

void GUI::renderPaymentMethod(const string& orderID, const Price& paymentAmount,
                              const OrderContext& ctx,
                              function<void()>& onPaymentSuccess) {
  Rectangle rec = paymentMethodRec;

  for (int i = 0; i < PAYMENT_METHOD_COUNT; i++) {
    if (i) rec.y += 50;

    (new GuiButton(PAYMENT_METHODS[i], rec,
                   ctx.paymentMethod == i ? DARKBLUE : BLACK,
                   ctx.paymentMethod == i ? SKYBLUE : LIGHTGRAY, 15, 10))
        ->render(GetFontDefault(), 0);
  }

  if (!isShowCTA)
    renderPaymentQR(orderID + "::" + PAYMENT_METHODS[ctx.paymentMethod] +
                        "::" + paymentAmount.format(),
                    ctx.paymentMethod, paymentAmount, onPaymentSuccess);
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
                          const Price& totalCost, const string& address,
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

  renderOrderInfoText(
      "Shipped to: ",
      address.size() > 16 ? address.substr(0, 16) + "..." : address, 330,
      LIGHTGRAY, DARKGRAY);
  renderOrderInfoText("Payment Method: ", PAYMENT_METHODS[paymentMethod], 370,
                      SKYBLUE, BLUE);
  renderOrderInfoText("Total Cost: ", "$" + totalCost.format(), 410, GREEN,
                      DARKGREEN);

  isShowCTA = true;
  renderCTAButton("Place Another Order");
}
