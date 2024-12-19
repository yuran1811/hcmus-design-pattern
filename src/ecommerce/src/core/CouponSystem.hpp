#pragma once

#include <ctime>
#include <mutex>
#include <string>
#include <vector>
#include <map>

#include "../shared/index.hpp"

using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::time;
using std::time_t;
using std::vector;

class CouponSystem {
 private:
  map<string, Coupon> coupons;
  map<string, pair<bool, float>> appliedOrders;

  static std::mutex mutex_;
  static CouponSystem* instance;

  CouponSystem() = default;
  CouponSystem(const CouponSystem&) = delete;
  ~CouponSystem() = default;

  CouponSystem& operator=(const CouponSystem&) = delete;

 public:
  static CouponSystem* getInstance();

  vector<Coupon> showCoupons();

  void addCoupon(const string& code, float discount, bool isPercentage,
                 time_t expiry, int usageLimit);
  void importCoupons(const vector<Coupon>&);

  bool checkExpired(const string&);
  bool checkExceedUsageLimit(const string&);
  pair<bool, string> validateCoupon(const string&, float);
  
  pair<float, string> applyCoupon(const string&, const string&, float,
                                  bool isPreviewd = false);
};
