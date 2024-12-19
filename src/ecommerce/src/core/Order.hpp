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
  Order();
  virtual ~Order() = default;

  Order& operator=(const Order&) = delete;

  const string& getOrderId() const;
  const bool isOrderCompleted() const;
  const pair<bool, vector<string>>& getPlaceOrderReturn() const;

  void markOrderCompleted();

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

  void setCouponCode(const string& _);

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

  void fetchDeliveryDetails();

 public:
  ExpressDeliveryDecorator(Order*);

  float calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};
