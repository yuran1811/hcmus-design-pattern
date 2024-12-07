#include "Payment.hpp"

void registerPaymentGateways() {
  PaymentGatewayRegistry::getInstance().registerFactory(
      "credit", []() { return new CreditCardPayment(); });
  PaymentGatewayRegistry::getInstance().registerFactory(
      "paypal", []() { return new PayPalPayment(); });
  PaymentGatewayRegistry::getInstance().registerFactory(
      "stripe", []() { return new StripePayment(); });
  PaymentGatewayRegistry::getInstance().registerFactory(
      "cod", []() { return new CODPayment(); });
}