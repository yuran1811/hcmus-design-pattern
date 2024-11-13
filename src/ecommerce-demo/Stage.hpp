#pragma once

#include <iostream>
#include <memory>

using namespace std;

// Abstract Order Stage Handler
class OrderStage {
 protected:
  OrderStage* nextStage;

 public:
  OrderStage() : nextStage(nullptr) {}
  virtual ~OrderStage() = default;

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
