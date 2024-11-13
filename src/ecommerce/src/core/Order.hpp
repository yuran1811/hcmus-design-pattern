#pragma once

#include <iostream>

using std::cout;
using std::move;

// Abstract Order class
class Order {
 public:
  virtual void placeOrder() const = 0;
  virtual ~Order() = default;
};

// Concrete Basic Order
class BasicOrder : public Order {
 public:
  void placeOrder() const override { cout << "Placing a basic order.\n"; }
};

// Abstract Decorator
class OrderDecorator : public Order {
 protected:
  Order* wrappedOrder;

 public:
  OrderDecorator(Order* order) : wrappedOrder(move(order)) {}
};

// Concrete Decorator: Gift Wrap
class GiftWrapDecorator : public OrderDecorator {
 public:
  GiftWrapDecorator(Order* order) : OrderDecorator(move(order)) {}

  void placeOrder() const override {
    wrappedOrder->placeOrder();
    cout << "Adding gift wrap.\n";
  }
};

// Concrete Decorator: Express Delivery
class ExpressDeliveryDecorator : public OrderDecorator {
 public:
  ExpressDeliveryDecorator(Order* order) : OrderDecorator(move(order)) {}

  void placeOrder() const override {
    wrappedOrder->placeOrder();
    cout << "Adding express delivery.\n";
  }
};
