#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <map>

#include "../shared/index.hpp"

using std::cout;
using std::function;
using std::map;
using std::pair;
using std::string;
using std::to_string;
using std::vector;

// Abstract Payment Gateway
class PaymentGateway {
 public:
  virtual ~PaymentGateway() = default;

  virtual const unsigned char& getMethodID() const = 0;
  virtual const string& getDisplayName() const = 0;

  virtual pair<bool, string> processPayment(const double&) = 0;
};

// Concrete Payment Gateways
class CreditCardPayment : public PaymentGateway {
 public:
  const unsigned char& getMethodID() const override {
    static const unsigned char methodID = PaymentMethod::CREDIT_CARD;
    return methodID;
  }
  const string& getDisplayName() const override {
    return PAYMENT_METHODS[getMethodID()];
  }

  pair<bool, string> processPayment(const double& amount) override {
    return {true,
            "Processing Credit Card payment of $" + to_string(amount) + "\n"};
  }
};

class PayPalPayment : public PaymentGateway {
 public:
  const unsigned char& getMethodID() const override {
    static const unsigned char methodID = PaymentMethod::PAYPAL;
    return methodID;
  }
  const string& getDisplayName() const override {
    return PAYMENT_METHODS[getMethodID()];
  }

  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processing Paypal payment of $" + to_string(amount) + "\n"};
  }
};

class StripePayment : public PaymentGateway {
 public:
  const unsigned char& getMethodID() const override {
    static const unsigned char methodID = PaymentMethod::STRIPE;
    return methodID;
  }
  const string& getDisplayName() const override {
    return PAYMENT_METHODS[getMethodID()];
  }

  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processing Stripe payment of $" + to_string(amount) + "\n"};
  }
};

class CODPayment : public PaymentGateway {
 public:
  const unsigned char& getMethodID() const override {
    static const unsigned char methodID = PaymentMethod::COD;
    return methodID;
  }
  const string& getDisplayName() const override {
    return PAYMENT_METHODS[getMethodID()];
  }

  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processing COD payment of $" + to_string(amount) + "\n"};
  }
};

// Abstract Factory
class PaymentGatewayFactory {
 public:
  virtual ~PaymentGatewayFactory() = default;

  virtual PaymentGateway* createPaymentGateway() = 0;
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

class StripePaymentFactory : public PaymentGatewayFactory {
 public:
  PaymentGateway* createPaymentGateway() override {
    return new StripePayment();
  }
};

class CODPaymentFactory : public PaymentGatewayFactory {
 public:
  PaymentGateway* createPaymentGateway() override { return new CODPayment(); }
};

// Payment Gateway Registry - Singleton Design Pattern
class PaymentGatewayRegistry {
 private:
  map<string, function<PaymentGateway*()>> factories;

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
    if (factories.find(type) != factories.end()) return;
    factories[type] = factory;
  }

  PaymentGateway* createPaymentGateway(const string& type) {
    if (factories.find(type) == factories.end()) return nullptr;
    return factories[type]();
  }
};

// Register factories
void registerPaymentGateways();
