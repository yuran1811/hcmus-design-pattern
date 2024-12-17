#pragma once

#include <iostream>
#include <vector>

#include "CouponSystem.hpp"

#include "../utils/index.hpp"

using std::cout;
using std::move;
using std::pair;
using std::string;
using std::vector;

// Abstract Order
class Order {
 protected:
  const string ORDER_ID;

  bool isCompleted = false;
  pair<bool, vector<string>> placeOrderReturn;

 public:
  Order() : ORDER_ID(utils::uuid_v4()) {}
  virtual ~Order() = default;

  Order& operator=(const Order&) = delete;

  const string& getOrderId() const { return ORDER_ID; }
  const bool isOrderCompleted() const { return isCompleted; }
  const pair<bool, vector<string>>& getPlaceOrderReturn() const {
    return placeOrderReturn;
  }

  void markOrderCompleted() { isCompleted = true; };

  virtual float calculateTotal() const = 0;
  virtual pair<bool, vector<string>> placeOrder() = 0;
};

// Concrete Order: Basic Order
class BasicOrder : public Order {
 private:
  float cartTotal = .0f;
  string couponCode = "";

 public:
  BasicOrder() = default;
  BasicOrder(float, const string&);

  void setCouponCode(const string& _) { couponCode = _; }

  float calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};

// Abstract Decorator
class OrderDecorator : public Order {
 protected:
  Order* wrappedOrder;

 public:
  OrderDecorator() = delete;
  OrderDecorator(Order* order) : wrappedOrder(move(order)) {}
};

// Concrete Decorator: Gift Wrap
class GiftWrapDecorator : public OrderDecorator {
 private:
  float giftWrapFee;

 public:
  GiftWrapDecorator(Order*, float);

  float calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};

// Concrete Decorator: Express Delivery
class ExpressDeliveryDecorator : public OrderDecorator {
 private:
  float expressFee;
  bool isAvailable;

  void fetchDeliveryDetails() {
    // Mocking API call to fetch express delivery fee and availability
    expressFee = 24.25f;  // Simulating fee
    isAvailable = true;   // Simulating availability
  }

 public:
  ExpressDeliveryDecorator(Order*);

  float calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};
