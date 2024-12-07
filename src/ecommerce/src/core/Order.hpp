#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "CouponSystem.hpp"

using std::cout;
using std::move;
using std::shared_ptr;
using std::string;

class Order {
 public:
  virtual void placeOrder() const = 0;
  virtual float calculateTotal() const = 0;
  virtual ~Order() = default;
};

class BasicOrder : public Order {
 private:
  float cartTotal;
  string couponCode;

 public:
  BasicOrder(float total, const string& code = "")
      : cartTotal(total), couponCode(code) {}

  float calculateTotal() const override {
    if (couponCode.empty()) return cartTotal;

    CouponSystem* couponSystem = CouponSystem::getInstance();
    return couponSystem->applyCoupon(couponCode, cartTotal).first;
  }

  void placeOrder() const override {
    CouponSystem* couponSystem = CouponSystem::getInstance();
    auto validation = couponSystem->validateCoupon(couponCode, cartTotal);

    if (!validation.first) {
      cout << "Order could not be placed. " << validation.second << "\n";
      return;
    }

    float finalTotal = calculateTotal();
    cout << "Order placed successfully. Final total: $" << finalTotal << "\n";
  }
};

// Abstract Decorator
class OrderDecorator : public Order {
 protected:
  shared_ptr<Order> wrappedOrder;

 public:
  OrderDecorator(shared_ptr<Order> order) : wrappedOrder(move(order)) {}
};

// Concrete Decorator: Gift Wrap
class GiftWrapDecorator : public OrderDecorator {
 private:
  float giftWrapFee;

 public:
  GiftWrapDecorator(shared_ptr<Order> order, float fee = 5.0f)
      : OrderDecorator(move(order)), giftWrapFee(fee) {}

  float calculateTotal() const override {
    return wrappedOrder->calculateTotal() + giftWrapFee;
  }

  void placeOrder() const override {
    wrappedOrder->placeOrder();
    cout << "Adding gift wrap. Fee: $" << giftWrapFee << "\n";
  }
};

// Concrete Decorator: Express Delivery
class ExpressDeliveryDecorator : public OrderDecorator {
 private:
  float expressFee;
  bool isAvailable;

  void fetchDeliveryDetails() {
    // Mocking API call to fetch express delivery fee and availability
    expressFee = 20.0f;  // Simulating fee
    isAvailable = true;  // Simulating availability
  }

 public:
  ExpressDeliveryDecorator(shared_ptr<Order> order)
      : OrderDecorator(move(order)), expressFee(0.0f), isAvailable(false) {
    fetchDeliveryDetails();
  }

  float calculateTotal() const override {
    if (isAvailable) {
      return wrappedOrder->calculateTotal() + expressFee;
    }
    return wrappedOrder->calculateTotal();
  }

  void placeOrder() const override {
    if (!isAvailable) {
      cout << "Express delivery is not available for your location.\n";
      return;
    }
    wrappedOrder->placeOrder();
    cout << "Adding express delivery. Fee: $" << expressFee << "\n";
  }
};
