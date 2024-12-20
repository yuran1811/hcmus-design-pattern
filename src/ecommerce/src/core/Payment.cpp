#include "Payment.hpp"

void registerPaymentGateways() {
  PaymentGatewayRegistry& registry = PaymentGatewayRegistry::getInstance();

  registry.registerFactory(PAYMENT_METHODS[PaymentMethod::CREDIT_CARD],
                           make_unique<CreditCardPaymentFactory>());
  registry.registerFactory(PAYMENT_METHODS[PaymentMethod::PAYPAL],
                           make_unique<PayPalPaymentFactory>());
  registry.registerFactory(PAYMENT_METHODS[PaymentMethod::STRIPE],
                           make_unique<StripePaymentFactory>());
  registry.registerFactory(PAYMENT_METHODS[PaymentMethod::COD],
                           make_unique<CODPaymentFactory>());
}
