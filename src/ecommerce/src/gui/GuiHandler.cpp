#include "Gui.hpp"

bool GUI::archivedOrderHandler() {
  archivedOrdersContainer->handleScrolling();
  return true;
}

bool GUI::selectItemHandler(CartType& cart, Price& price) {
  for (int i = 0; i < items.size(); i++) {
    if (utils::ui::mousePressedInBox({leftAlign, (40.f * i) + 130, 150, 30},
                                     MOUSE_BUTTON_LEFT)) {
      if (cart.find(items[i].name) != cart.end()) {
        // Increment quantity if item is already in cart
        cart[items[i].name].second++;
      } else {
        // Add new item to cart
        cart[items[i].name] = {items[i], 1};
      }

      price += items[i].price;
    }
  }

  return true;
}

bool GUI::inputHandler(const InputType& inputType, string& value, int maxLen,
                       const Range<int>& range, const Rectangle& rec) {
  if (utils::ui::mousePressedInBox(rec, MOUSE_BUTTON_LEFT)) {
    for (BitState& state : inputActiveStates)
      state.unset((unsigned int)InputState::ACTIVE);

    inputActiveStates[(int)inputType].set((unsigned int)InputState::ACTIVE);
  }

  if (inputActiveStates[(int)inputType].isSet(
          (unsigned int)InputState::ACTIVE)) {
    int key = GetCharPressed();

    if (utils::range::is_in_range(range, key) && value.length() < maxLen) {
      value.push_back((char)key);
    } else if ((IsKeyPressed(KEY_BACKSPACE) ||
                IsKeyPressedRepeat(KEY_BACKSPACE)) &&
               value.length() > 0) {
      value.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER))
      inputActiveStates[(int)inputType].unset((unsigned int)InputState::ACTIVE);

    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
        IsKeyPressed(KEY_V)) {
      const char* clipboard = GetClipboardText();
      if (clipboard) {
        value += clipboard;
        if (value.length() > maxLen) value = value.substr(0, maxLen);
      }
    }
  }

  return true;
}

bool GUI::addressHandler(string& address) {
  return inputHandler(InputType::ADDRESS, address, 100, {32, 125},
                      ADDR_INP_REC);
}

bool GUI::phoneHandler(string& phone) {
  return inputHandler(InputType::PHONE, phone, 10, {48, 57}, PHONE_INP_REC);
}

bool GUI::addressInfoHandler(string& address, string& phone) {
  addressHandler(address);
  phoneHandler(phone);

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      !(utils::ui::mousePressedInBox(ADDR_INP_REC, MOUSE_BUTTON_LEFT) ||
        utils::ui::mousePressedInBox(PHONE_INP_REC, MOUSE_BUTTON_LEFT))) {
    bool flag = false;
    for (BitState& state : inputActiveStates)
      if (state.isSet((unsigned int)InputState::ACTIVE)) {
        flag = true;
        break;
      }

    if (flag) {
      for (BitState& state : inputActiveStates)
        state.unset((unsigned int)InputState::ACTIVE);
    }
  }

  return true;
}

bool GUI::paymentMethodHandler(PaymentMethod& paymentMethod,
                               const Price& price) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Rectangle rec = paymentMethodRec;

    for (int i = 0; i < PAYMENT_METHOD_COUNT; i++) {
      if (i) rec.y += 50;

      if (CheckCollisionPointRec(GetMousePosition(), rec)) {
        setPaymentMethodChanged(true);

        paymentMethod = (PaymentMethod)i;
      }
    }
  }

  return true;
}

bool GUI::isCTAClicked() const {
  return isShowCTA && utils::ui::mousePressedInBox(ctaRec, MOUSE_BUTTON_LEFT);
}

bool GUI::isBackProgressClicked() const {
  return isShowBackProgress &&
         utils::ui::mousePressedInCircle(
             ORDER_PROG_POS, ORDER_PROG_ITEM_SIZE / 2, MOUSE_BUTTON_LEFT);
}
