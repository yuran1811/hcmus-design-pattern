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

  for (const auto& [code, coupon] : coupons)
    if (!checkExceedUsageLimit(code) && !checkExpired(code))
      returnCoupons.push_back(coupon);

  return returnCoupons;
}

void CouponSystem::addCoupon(const string& code, Price discount,
                             bool isPercentage, time_t expiry, int usageLimit) {
  coupons[code] = {code, discount, isPercentage, expiry, usageLimit};
}

void CouponSystem::importCoupons(const vector<Coupon>& coupons) {
  for (const Coupon& _ : coupons)
    addCoupon(_.code, _.discount, _.isPercentage, _.expiryDate, _.usageLimit);
}

bool CouponSystem::checkExpired(const string& _) {
  const time_t now = time(nullptr);

  if (now > coupons[_].expiryDate) {
    coupons.erase(_);
    return true;
  }

  return false;
}

bool CouponSystem::checkExceedUsageLimit(const string& _) {
  if (coupons[_].usageLimit <= 0) {
    coupons.erase(_);
    return true;
  }

  return false;
}

pair<bool, string> CouponSystem::validateCoupon(const string& code,
                                                Price cartTotal) {
  if (coupons.find(code) == coupons.end()) return {false, "Coupon not found."};
  if (checkExpired(code)) return {false, "Coupon has expired."};
  if (checkExceedUsageLimit(code))
    return {false, "Coupon usage limit exceeded."};

  if (cartTotal <= 0) return {false, "Cart total must be greater than 0."};

  return {true, "Coupon is valid."};
}

pair<pair<Price, Price>, string> CouponSystem::applyCoupon(
    const string& orderID, const string& code, Price cartTotal,
    bool isPreviewed) {
  const auto& orderIDStatus = appliedOrders.find(orderID) == appliedOrders.end()
                                  ? make_pair(false, Price(0))
                                  : appliedOrders[orderID];
  const bool isOrderApplied = orderIDStatus.first;

  auto validation = validateCoupon(code, cartTotal);
  if (!validation.first && !isOrderApplied)
    return {{cartTotal, Price(0)}, validation.second};

  Coupon& coupon = coupons[code];
  const Price discountAmount =
      isOrderApplied        ? orderIDStatus.second
      : coupon.isPercentage ? Price(cartTotal * int(coupon.discount / 100.f))
                            : Price(coupon.discount);

  if (!isPreviewed && !isOrderApplied) {
    appliedOrders[orderID] = make_pair(true, discountAmount);
    coupon.usageLimit--;
  }

  const Price finalPrice = cartTotal - discountAmount;
  return {{finalPrice > 0 ? finalPrice : Price(0), discountAmount},
          "Coupon applied successfully."};
}
