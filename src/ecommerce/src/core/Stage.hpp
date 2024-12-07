#pragma once

#include <iostream>

using std::cout;
using std::move;

// Abstract Order Stage Handler
class OrderStage {
 protected:
  OrderStage* prevStage = nullptr;
  OrderStage* nextStage = nullptr;

 public:
  OrderStage() = default;
  virtual ~OrderStage() {
    delete prevStage;
    delete nextStage;
  }

  void setPrevStage(OrderStage* prev) { nextStage = move(prev); }
  void setNextStage(OrderStage* next) { nextStage = move(next); }
  virtual void handleOrder() const = 0;
};

// Concrete Stage: Payment
class PaymentStage : public OrderStage {
 public:
  void handleOrder() const override {
    cout << "Processing payment stage.\n";
    if (nextStage) nextStage->handleOrder();
  }
};

// Concrete Stage: Packaging
class PackagingStage : public OrderStage {
 public:
  void handleOrder() const override {
    cout << "Processing packaging stage.\n";
    if (nextStage) nextStage->handleOrder();
  }
};

// Concrete Stage: Shipping
class ShippingStage : public OrderStage {
 public:
  void handleOrder() const override {
    cout << "Processing shipping stage.\n";
    if (nextStage) nextStage->handleOrder();
  }
};
