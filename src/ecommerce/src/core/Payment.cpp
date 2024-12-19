#include "Payment.hpp"

void registerPaymentGateways() {
  PaymentGatewayRegistry& registry = PaymentGatewayRegistry::getInstance();

  registry.registerFactory("credit", make_unique<CreditCardPaymentFactory>());
  registry.registerFactory("paypal", make_unique<PayPalPaymentFactory>());
  registry.registerFactory("stripe", make_unique<StripePaymentFactory>());
  registry.registerFactory("cod", make_unique<CODPaymentFactory>());
}
