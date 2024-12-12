#pragma once

#include <ctime>
#include <mutex>
#include <map>
#include <string>

#include "../shared/index.hpp"

using std::map;
using std::pair;
using std::string;
using std::time;
using std::time_t;

class CouponSystem {
 private:
  map<string, Coupon> coupons;

  static std::mutex mutex_;
  static CouponSystem* instance; 

  CouponSystem() = default;
  CouponSystem(const CouponSystem&) = delete;
  ~CouponSystem() = default;

  CouponSystem& operator=(const CouponSystem&) = delete;

 public:
  static CouponSystem* getInstance();

  void addCoupon(const string& code, float discount, bool isPercentage,
                 time_t expiry, int usageLimit);

  pair<bool, string> validateCoupon(const string& code, float cartTotal);

  pair<float, string> applyCoupon(const string& code, float cartTotal);
};
