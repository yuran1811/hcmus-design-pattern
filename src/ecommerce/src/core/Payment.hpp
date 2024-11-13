#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>

using std::cout;
using std::function;
using std::pair;
using std::string;
using std::to_string;
using std::unordered_map;

// Abstract Payment Gateway
class PaymentGateway {
 public:
  virtual pair<bool, string> processPayment(double amount) = 0;
  virtual ~PaymentGateway() = default;
};

// Concrete Payment Gateways
class CreditCardPayment : public PaymentGateway {
 public:
  pair<bool, string> processPayment(double amount) override {
    return {true,
            "Processing Credit Card payment of $" + to_string(amount) + "\n"};
  }
};

class PayPalPayment : public PaymentGateway {
 public:
  pair<bool, string> processPayment(double amount) override {
    return {true, "Processing Paypal payment of $" + to_string(amount) + "\n"};
  }
};

// Abstract Factory
class PaymentGatewayFactory {
 public:
  virtual PaymentGateway* createPaymentGateway() = 0;
  virtual ~PaymentGatewayFactory() = default;
};

// Concrete Factories
class CreditCardPaymentFactory : public PaymentGatewayFactory {
 public:
  PaymentGateway* createPaymentGateway() override {
    return new CreditCardPayment();
  }
};

class PayPalPaymentFactory : public PaymentGatewayFactory {
 public:
  PaymentGateway* createPaymentGateway() override {
    return new PayPalPayment();
  }
};

// Payment Gateway Registry - Singleton Design Pattern
class PaymentGatewayRegistry {
 private:
  unordered_map<string, function<PaymentGateway*()>> factories;

  PaymentGatewayRegistry() = default;
  PaymentGatewayRegistry(const PaymentGatewayRegistry&) = delete;
  ~PaymentGatewayRegistry() = default;

  PaymentGatewayRegistry& operator=(const PaymentGatewayRegistry&) = delete;

 public:
  static PaymentGatewayRegistry& getInstance() {
    static PaymentGatewayRegistry instance;
    return instance;
  }

  void registerFactory(const string& type,
                       function<PaymentGateway*()> factory) {
    factories[type] = factory;
  }

  PaymentGateway* createPaymentGateway(const string& type) {
    if (factories.find(type) != factories.end()) {
      return factories[type]();
    }
    return nullptr;
  }
};

// Register factories
inline void registerPaymentGateways() {
  PaymentGatewayRegistry::getInstance().registerFactory(
      "credit", []() { return new CreditCardPayment(); });
  PaymentGatewayRegistry::getInstance().registerFactory(
      "paypal", []() { return new PayPalPayment(); });
}
