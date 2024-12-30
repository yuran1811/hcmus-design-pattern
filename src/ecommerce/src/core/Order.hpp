#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <map>

#include "../shared/index.hpp"
#include "../utils/index.hpp"

#include "CouponSystem.hpp"

using std::cout;
using std::make_shared;
using std::map;
using std::move;
using std::pair;
using std::shared_ptr;
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

  virtual Price calculateTotal() const = 0;
  virtual pair<bool, vector<string>> placeOrder() = 0;
  virtual Order* unwrap() { return this; }
};

// Concrete Order: Basic Order
class BasicOrder : public Order {
 private:
  Price cartTotal;
  string couponCode = "";

 public:
  BasicOrder() = default;
  BasicOrder(Price, const string&);

  const Price& getCartTotal() const { return cartTotal; };
  const string& getCouponCode() const { return couponCode; };

  void setCartTotal(const Price&);
  void setCouponCode(const string& _);

  Price calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};

// Abstract Decorator
class OrderDecorator : public Order {
 protected:
  shared_ptr<Order> wrappedOrder;

 public:
  OrderDecorator() = delete;
  OrderDecorator(shared_ptr<Order> order) : wrappedOrder(move(order)) {}

  Order* unwrap() override {
    return wrappedOrder ? wrappedOrder->unwrap() : this;
  }
};

// Concrete Decorator: Gift Wrap
class GiftWrapDecorator : public OrderDecorator {
 private:
  Price giftWrapFee;

 public:
  GiftWrapDecorator(shared_ptr<Order>, Price);

  Price calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};

// Concrete Decorator: Express Delivery
class ExpressDeliveryDecorator : public OrderDecorator {
 public:
  inline static map<string, Price> availableProviders = {
      {"J&T Express", Price(23'15, 2)},
      {"Viettel Post", Price(25'35, 2)},
      {"Ninja Van", Price(21'55, 2)}};

 private:
  string currentProvider;
  Price expressFee;

  bool isAvailable;

 public:
  ExpressDeliveryDecorator(shared_ptr<Order>);

  void updateDeliveryProvider(const string&);

  const string& getCurrentDeliveryProvider() const;
  const Price& getExpressFee() const;

  Price calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};
