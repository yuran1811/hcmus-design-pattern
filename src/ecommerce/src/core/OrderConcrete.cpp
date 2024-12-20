#include "Order.hpp"

BasicOrder::BasicOrder(Price total, const string& code)
    : cartTotal(total), couponCode(code) {}

void BasicOrder::setCartTotal(const Price& _) { cartTotal = _; }

void BasicOrder::setCouponCode(const string& _) { couponCode = _; }

Price BasicOrder::calculateTotal() const {
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
      {!validation.first ? "No coupons are applied: " + validation.second
                         : "Order placed with coupon. New price: $" +
                               calculateTotal().format()}};
}
