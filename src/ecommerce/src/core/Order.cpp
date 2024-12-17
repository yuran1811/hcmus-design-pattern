#include "Order.hpp"

BasicOrder::BasicOrder(float total, const string& code)
    : cartTotal(total), couponCode(code) {}

float BasicOrder::calculateTotal() const {
  if (couponCode.empty()) return cartTotal;

  return CouponSystem::getInstance()
      ->applyCoupon(getOrderId(), couponCode, cartTotal, !isCompleted)
      .first;
}

pair<bool, vector<string>> BasicOrder::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  markOrderCompleted();

  CouponSystem* couponSystem = CouponSystem::getInstance();
  auto validation = couponSystem->validateCoupon(couponCode, cartTotal);

  return {
      true,
      {!validation.first ? "Order placed without coupon: " + validation.second
                         : "Order placed with coupon. New price: $" +
                               std::to_string(calculateTotal())}};
}

GiftWrapDecorator::GiftWrapDecorator(Order* order, float fee)
    : OrderDecorator(move(order)), giftWrapFee(fee) {}

float GiftWrapDecorator::calculateTotal() const {
  return wrappedOrder->calculateTotal() + giftWrapFee;
}

pair<bool, vector<string>> GiftWrapDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back("Adding gift wrap. Fee: $" +
                                      std::to_string(giftWrapFee));

  return {true, wrappedOrderReturn.second};
}

ExpressDeliveryDecorator::ExpressDeliveryDecorator(Order* order)
    : OrderDecorator(move(order)), expressFee(0.0f), isAvailable(false) {
  fetchDeliveryDetails();
}

float ExpressDeliveryDecorator::calculateTotal() const {
  if (isAvailable) return wrappedOrder->calculateTotal() + expressFee;

  return wrappedOrder->calculateTotal();
}

pair<bool, vector<string>> ExpressDeliveryDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back(
      isAvailable
          ? "Adding express delivery. Fee: $" + std::to_string(expressFee)
          : "Express delivery is not available for your location.");

  return {isAvailable, wrappedOrderReturn.second};
}
