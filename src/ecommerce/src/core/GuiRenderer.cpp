#include "Gui.hpp"

void GUI::render(const OrderStageState& curStage, function<void()> callback) {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  renderHeader("Interactive E-Commerce Order Processing");
  renderOrderProgress(curStage);
  callback();
  cursorUpdate(curStage);
  GuiButton::releaseButtons();

  EndDrawing();
}

void GUI::renderHeader(const string& s) {
  DrawText(s.c_str(), leftMidAlign, 15, 20, BLUE);
  DrawText(utils::getDateTimeString().c_str(), leftAlign, ctaRec.y + 12, 20,
           DARKGRAY);
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
  if (curStage > 0) {
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

void GUI::renderSelectItem(CartType& cart, Price& totalCost) {
  DrawText("Product:", leftAlign, 100, 20, DARKGRAY);

  int cartYPos = 130;
  for (int i = 0; i < items.size(); i++)
    (new GuiButton(items[i].name,
                   Rectangle{leftAlign, (40.f * i) + cartYPos, 150, 30},
                   DARKGRAY, LIGHTGRAY, 10, 5))
        ->render(GetFontDefault(), 0);

  isShowCTA = !cart.empty();
  if (isShowCTA) renderCTAButton("Fill the address info");

  const int quantityGap = 35;

  DrawText("Cart:", midAlign, 100, 20, DARKGRAY);

  for (auto& entry : cart) {
    DrawText(
        ("x" + to_string(entry.second.second) + " " + entry.first + " - $" +
         (entry.second.first.price * entry.second.second).format())
            .c_str(),
        midAlign + quantityGap * 2 + 10, cartYPos + 7, 20, DARKBLUE);

    // Decrease Handler
    if (utils::ui::mousePressedInBox({midAlign, float(cartYPos), 30, 30},
                                     MOUSE_BUTTON_LEFT)) {
      if (entry.second.second > 0) {
        totalCost -= entry.second.first.price;
        if (--entry.second.second <= 0) cart.erase(entry.first);
      }
    }

    // Increase Handler
    if (utils::ui::mousePressedInBox(
            {midAlign + quantityGap, float(cartYPos), 30, 30},
            MOUSE_BUTTON_LEFT)) {
      entry.second.second++;
      totalCost += entry.second.first.price;
    }

    // Draw the Increase/Decrease buttons
    Rectangle rec = {midAlign, float(cartYPos), 30, 30};
    (new GuiButton("-", rec, GRAY, LIGHTGRAY, 10, 7))
        ->render(GetFontDefault(), 0);

    rec.x = midAlign + quantityGap;
    (new GuiButton("+", rec, GRAY, LIGHTGRAY, 10, 7))
        ->render(GetFontDefault(), 0);

    cartYPos += 40;
  }

  if (cart.size()) {
    const Rectangle clearBtnRec = {midAlign, float(cartYPos), 75, 30};
    (new GuiButton("Clear", clearBtnRec, WHITE, MAROON, 10, 6))
        ->render(GetFontDefault(), 0);

    if (utils::ui::mousePressedInBox({midAlign, float(cartYPos), 75, 30},
                                     MOUSE_BUTTON_LEFT)) {
      cart.clear();
      totalCost = 0;
    }

    cartYPos += 45;
    DrawLine(midAlign, cartYPos, 710, cartYPos, LIGHTGRAY);
    cartYPos += 15;

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
  if (isShowCTA) renderCTAButton("Goto Payment");
}

void GUI::renderPaymentQR(const string& content,
                          const PaymentMethod& paymentMethod,
                          const Price& price) {
  const int __leftAlign = paymentMethodRec.x + paymentMethodRec.width + 50;

  if (paymentMethod == COD) {
    DrawText("Cash on Delivery", __leftAlign, paymentMethodRec.y, 20, DARKGRAY);
    loadingStates.unset((unsigned int)LoadingState::PAYMENT_QR);
    return;
  }

  DrawText("Scan the QR code to make payment", __leftAlign, paymentMethodRec.y,
           20, GRAY);
  DrawText(("Total: $" + price.format()).c_str(), __leftAlign,
           paymentMethodRec.y + 35, 20, DARKGREEN);

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

  if (loadingStates.isSet((unsigned int)LoadingState::PAYMENT_QR))
    DrawText("Loading QR Code...", __leftAlign, paymentMethodRec.y + 70, 20,
             DARKGRAY);
  else
    DrawTexture(itemTextures[(int)ItemTexture::PAYMENT_QR], __leftAlign,
                paymentMethodRec.y + 70, RAYWHITE);
}

void GUI::renderPaymentMethod(const PaymentMethod& paymentMethod,
                              const Price& price) {
  Rectangle rec = paymentMethodRec;

  for (int i = 0; i < PAYMENT_METHOD_COUNT; i++) {
    if (i) rec.y += 50;

    (new GuiButton(PAYMENT_METHODS[i], rec, BLACK,
                   paymentMethod == i ? GRAY : LIGHTGRAY, 15, 10))
        ->render(GetFontDefault(), 0);
  }

  renderPaymentQR(PAYMENT_METHODS[paymentMethod] + "::" + price.format(),
                  paymentMethod, price);

  isShowCTA = true;
  renderCTAButton("Select Shipping Provider");
}

void GUI::renderShipping() {
  isShowCTA = true;
  renderCTAButton("Finish");
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

void GUI::renderCompleted(Price& totalCost, const string& address,
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

  DrawText("Thank you for your order!", leftMidAlign + 50, 150, 25, DARKBLUE);
  DrawText("Your order has been completed!", leftMidAlign + 55, 200, 20, GRAY);

  // renderOrderInfoText("Order ID: ", "123456", 400, LIGHTGRAY, DARKGRAY);
  // renderOrderInfoText("Order Date: ", "2021-09-01", 450, LIGHTGRAY,
  // DARKGRAY);

  renderOrderInfoText("Shipped to: ", address, 300, LIGHTGRAY, DARKGRAY);
  renderOrderInfoText("Payment Method: ", PAYMENT_METHODS[paymentMethod], 350,
                      SKYBLUE, BLUE);
  renderOrderInfoText("Total Cost: ", "$" + totalCost.format(), 400, GREEN,
                      DARKGREEN);

  isShowCTA = true;
  renderCTAButton("Place Another Order");
}
