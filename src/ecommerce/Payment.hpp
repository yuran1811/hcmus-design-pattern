#pragma once

#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Abstract Payment Gateway
class PaymentGateway {
 public:
  virtual void processPayment(double amount) = 0;
  virtual ~PaymentGateway() = default;
};

// Concrete Payment Gateway: Credit Card
class CreditCardPayment : public PaymentGateway {
 public:
  void processPayment(double amount) override {
    cout << "Processing credit card payment of $" << amount << "\n";
  }
};

// Concrete Payment Gateway: PayPal
class PayPalPayment : public PaymentGateway {
 public:
  void processPayment(double amount) override {
    cout << "Processing PayPal payment of $" << amount << "\n";
  }
};

// Payment Gateway Factory
class PaymentGatewayFactory {
 public:
  static PaymentGateway* createPaymentGateway(const string& type) {
    if (type == "credit") return new CreditCardPayment();
    if (type == "paypal") return new PayPalPayment();
    return nullptr;
  }
};
