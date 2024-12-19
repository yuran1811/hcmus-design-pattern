#include "Order.hpp"

BasicOrder::BasicOrder(float total, const string& code)
    : cartTotal(total), couponCode(code) {}

void BasicOrder::setCouponCode(const string& _) { couponCode = _; }

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
