#include "Gui.hpp"

void GUI::render(const OrderStageState& curStage, function<void()> callback) {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  cursorUpdate(curStage);
  renderHeader("Interactive E-Commerce Order Processing");
  callback();

  EndDrawing();
}

void GUI::renderHeader(const string& s) {
  DrawText(s.c_str(), leftMidAlign, 15, 20, BLUE);
  DrawLine(0, 45, SCREEN_SIZE.width, 45, LIGHTGRAY);
}

void GUI::renderCTAButton(const string& message) {
  const float ctaRecWidth = 13.f * message.length();
  const float rightAlign = SCREEN_SIZE.width - ctaRecWidth - 10;
  setCTARec({rightAlign, ctaRec.y, ctaRecWidth, ctaRec.height});

  Color ctaColor = GRAY;

  if (CheckCollisionPointRec(GetMousePosition(), ctaRec)) {
    ctaColor = DARKGRAY;

    cursorBitState.set((unsigned int)MouseCursor::MOUSE_CURSOR_POINTING_HAND);
  } else {
    cursorBitState.unset((unsigned int)MouseCursor::MOUSE_CURSOR_POINTING_HAND);
  }

  DrawRectangleRounded(ctaRec, .25f, 40, ctaColor);
  DrawText(message.c_str(), ctaRec.x + 12, ctaRec.y + 12, 20, RAYWHITE);
}

void GUI::renderStageMessage(const string& s) {
  DrawText((">>> " + s).c_str(), leftAlign, 60, 20,
           utils::color::calcBreathColor(LIME, frameTimer * 3.75f));
}

void GUI::renderSelectItem(const vector<Item>& items, CartType& cart,
                           Price& totalCost) {
  for (int i = 0; i < items.size(); i++) {
    DrawRectangle(leftAlign, 100 + (i * 50), 150, 30, LIGHTGRAY);
    DrawText(items[i].name.c_str(), leftAlign + 10, 105 + (i * 50), 20, BLACK);
  }

  isShowCTA = !cart.empty();
  if (isShowCTA) renderCTAButton("Proceed to Address");

  DrawText("Cart:", 300, 100, 20, DARKGRAY);

  int cartYPos = 130;
  const int incButtonX = 300;
  const int quantityGap = 35;
  for (auto& entry : cart) {
    DrawText(
        ("x" + to_string(entry.second.second) + " " + entry.first + " - $" +
         (entry.second.first.price * entry.second.second).format())
            .c_str(),
        incButtonX + quantityGap * 2 + 10, cartYPos + 7, 20, DARKBLUE);

    // Draw Increase and Decrease buttons for the item quantity
    if (utils::ui::mousePressedInBox({incButtonX, float(cartYPos), 30, 30},
                                     MOUSE_BUTTON_LEFT)) {
      if (entry.second.second >= 1) {
        if (entry.second.second == 1)
          cart.erase(entry.first);
        else
          entry.second.second--;

        totalCost -= entry.second.first.price;
      }
    }

    if (utils::ui::mousePressedInBox(
            {incButtonX + quantityGap, float(cartYPos), 30, 30},
            MOUSE_BUTTON_LEFT)) {
      // Increase quantity
      entry.second.second++;
      totalCost += entry.second.first.price;
    }

    // Draw the Increase/Decrease buttons
    Rectangle rec = {incButtonX, float(cartYPos), 30, 30};
    DrawRectangleRounded(rec, 1.f, 0, LIGHTGRAY);
    DrawText("-", incButtonX + 10, cartYPos + 7, 20, BLACK);

    rec.x = incButtonX + quantityGap;
    DrawRectangleRounded(rec, 1.f, 0, LIGHTGRAY);
    DrawText("+", incButtonX + quantityGap + 10, cartYPos + 7, 20, BLACK);

    cartYPos += 40;
  }

  if (cart.size()) {
    const Rectangle clearBtnRec = {incButtonX, float(cartYPos), 75, 30};
    DrawRectangleRounded(clearBtnRec, .25f, 40, RED);
    DrawText("Clear", incButtonX + 10, cartYPos + 6, 20, WHITE);

    if (utils::ui::mousePressedInBox({incButtonX, float(cartYPos), 75, 30},
                                     MOUSE_BUTTON_LEFT)) {
      cart.clear();
      totalCost = 0;
    }

    cartYPos += 45;
    DrawLine(incButtonX, cartYPos, 710, cartYPos, LIGHTGRAY);
    cartYPos += 15;

    DrawText(("Total: $" + totalCost.format()).c_str(), incButtonX, cartYPos,
             20, DARKGREEN);
  } else {
    DrawText("Cart is empty!", incButtonX, cartYPos, 20, GRAY);
  }
}

void GUI::renderAddressInfo(const string& address, const string& phone) {
  renderAddressInput(address);
  renderPhoneInput(phone);

  isShowCTA = !address.empty() && phone.length() == 10;
  if (isShowCTA) renderCTAButton("Proceed to Payment Method");
}

void GUI::renderInput(const InputType& type, const string& info,
                      const string& value, const Vector2& pos,
                      const Vector2& size) {
  DrawText(info.c_str(), pos.x, pos.y - 30, 20, DARKGRAY);

  if (inputActiveStates[int(type)].isSet((unsigned int)InputState::ACTIVE)) {
    DrawRectangleV(pos, size, SKYBLUE);
    DrawText(value.c_str(), pos.x + 10, pos.y + 10, 20, DARKBLUE);

    if ((getFrameCounter() / 30) % 2)
      DrawText("_", pos.x + 10 + MeasureText(value.c_str(), 20), pos.y + 6, 30,
               DARKBLUE);
  } else {
    DrawRectangleV(pos, size, LIGHTGRAY);
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

void GUI::renderPaymentQR(const string& content,
                          const PaymentMethod& paymentMethod,
                          const Price& price) {
  if (paymentMethod == COD) return;

  const int __leftAlign = paymentMethodRec.x + paymentMethodRec.width + 50;

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
  isPaymentMethodChanged = false;

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

  DrawRectangleRec(rec, paymentMethod == CREDIT_CARD ? GRAY : LIGHTGRAY);
  DrawText("Credit Card", rec.x + 15, rec.y + 10, 20, BLACK);

  rec.y += 50;
  DrawRectangleRec(rec, paymentMethod == PAYPAL ? GRAY : LIGHTGRAY);
  DrawText("PayPal", rec.x + 15, rec.y + 10, 20, BLACK);

  rec.y += 50;
  DrawRectangleRec(rec, paymentMethod == COD ? GRAY : LIGHTGRAY);
  DrawText("COD", rec.x + 15, rec.y + 10, 20, BLACK);

  renderPaymentQR(PAYMENT_METHODS[paymentMethod] + "::" + price.format(),
                  paymentMethod, price);

  isShowCTA = true;
  renderCTAButton("Proceed to Payment");
}

void GUI::renderPayment() {
  isShowCTA = true;
  renderCTAButton("Processing Payment");
}

void GUI::renderPackaging() {
  isShowCTA = true;
  renderCTAButton("Packaging Order");
}

void GUI::renderShipping() {
  isShowCTA = true;
  renderCTAButton("Shipping Order");
}

void GUI::renderCompleted(Price& totalCost, const string& address,
                          const PaymentMethod& paymentMethod) {
  DrawText("Thank you for your order!", leftMidAlign + 50, 150, 25, DARKBLUE);
  DrawText("Your order has been completed!", leftMidAlign + 55, 200, 20, GRAY);

  const int infoOffset = 78;
  DrawText(("Shipped to: " + address).c_str(), leftMidAlign + infoOffset, 300,
           20, DARKGRAY);
  DrawText(
      ("Payment Method: " + string(PAYMENT_METHODS[paymentMethod])).c_str(),
      leftMidAlign + infoOffset, 350, 20, BLUE);
  DrawText(("Total Cost: $" + totalCost.format()).c_str(),
           leftMidAlign + infoOffset, 400, 20, DARKGREEN);

  isShowCTA = true;
  renderCTAButton("Place Another Order");
}
