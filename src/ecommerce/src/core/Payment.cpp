#include "Payment.hpp"

void registerPaymentGateways() {
  static vector<pair<string, function<PaymentGateway*()>>> _ = {
      {"credit", []() { return new CreditCardPayment(); }},
      {"paypal", []() { return new PayPalPayment(); }},
      {"stripe", []() { return new StripePayment(); }},
      {"cod", []() { return new CODPayment(); }},
  };

  for (auto& p : _) {
    PaymentGatewayRegistry::getInstance().registerFactory(p.first, p.second);
  }
}