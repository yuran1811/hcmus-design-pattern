#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "../shared/index.hpp"

using std::cout;
using std::function;
using std::make_unique;
using std::map;
using std::pair;
using std::string;
using std::to_string;
using std::unique_ptr;
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

  virtual unique_ptr<PaymentGateway> createPaymentGateway() const = 0;
};

// Concrete Factories
class CreditCardPaymentFactory : public PaymentGatewayFactory {
 public:
  unique_ptr<PaymentGateway> createPaymentGateway() const override {
    return make_unique<CreditCardPayment>();
  }
};

class PayPalPaymentFactory : public PaymentGatewayFactory {
 public:
  unique_ptr<PaymentGateway> createPaymentGateway() const override {
    return make_unique<PayPalPayment>();
  }
};

class StripePaymentFactory : public PaymentGatewayFactory {
 public:
  unique_ptr<PaymentGateway> createPaymentGateway() const override {
    return make_unique<StripePayment>();
  }
};

class CODPaymentFactory : public PaymentGatewayFactory {
 public:
  unique_ptr<PaymentGateway> createPaymentGateway() const override {
    return make_unique<CODPayment>();
  }
};

// Payment Gateway Registry - Singleton Design Pattern
class PaymentGatewayRegistry {
 private:
  map<string, unique_ptr<PaymentGatewayFactory>> factories;

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
                       unique_ptr<PaymentGatewayFactory> factory) {
    if (factories.find(type) != factories.end()) return;
    factories[type] = std::move(factory);
  }

  unique_ptr<PaymentGateway> createPaymentGateway(const string& type) {
    return factories.find(type) == factories.end()
               ? nullptr
               : factories[type]->createPaymentGateway();
  }
};

// Register factories
void registerPaymentGateways();
