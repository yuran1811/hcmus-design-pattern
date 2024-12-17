#include "CouponSystem.hpp"

CouponSystem* CouponSystem::instance = nullptr;
std::mutex CouponSystem::mutex_;

CouponSystem* CouponSystem::getInstance() {
  if (!instance) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!instance) instance = new CouponSystem();
  }

  return instance;
}

vector<Coupon> CouponSystem::showCoupons() {
  vector<Coupon> returnCoupons;

  for (const auto& [code, coupon] : coupons) {
    returnCoupons.push_back(coupon);
  }

  return returnCoupons;
}

void CouponSystem::addCoupon(const string& code, float discount,
                             bool isPercentage, time_t expiry, int usageLimit) {
  coupons[code] = {code, discount, isPercentage, expiry, usageLimit};
}

void CouponSystem::importCoupons(const vector<Coupon>& coupons) {
  for (const Coupon& _ : coupons)
    addCoupon(_.code, _.discount, _.isPercentage, _.expiryDate, _.usageLimit);
}

pair<bool, string> CouponSystem::validateCoupon(const string& code,
                                                float cartTotal) {
  if (coupons.find(code) == coupons.end()) return {false, "Coupon not found."};

  Coupon& coupon = coupons[code];
  time_t now = time(nullptr);

  if (coupon.usageLimit <= 0) return {false, "Coupon usage limit exceeded."};
  if (now > coupon.expiryDate) return {false, "Coupon has expired."};
  if (cartTotal <= 0) return {false, "Cart total must be greater than 0."};

  return {true, "Coupon is valid."};
}

pair<float, string> CouponSystem::applyCoupon(const string& orderID,
                                              const string& code,
                                              float cartTotal,
                                              bool isPreviewed) {
  auto validation = validateCoupon(code, cartTotal);
  if (!validation.first) return {cartTotal, validation.second};

  Coupon& coupon = coupons[code];
  const float discountAmount = coupon.isPercentage
                                   ? (cartTotal * (coupon.discount / 100))
                                   : coupon.discount;

  if (!isPreviewed && !appliedOrders[orderID]) {
    appliedOrders[orderID] = true;
    coupon.usageLimit--;
  }

  const float finalPrice = cartTotal - discountAmount;
  return {finalPrice > 0 ? finalPrice : 0, "Coupon applied successfully."};
}
