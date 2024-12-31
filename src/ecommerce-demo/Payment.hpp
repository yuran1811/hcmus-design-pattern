// Abstract Payment Gateway
class PaymentGateway {
 public:
  virtual ~PaymentGateway() = default;
  virtual pair<bool, string> processPayment(const double&) = 0;
};

// Concrete Payment Gateways
class CreditCardPayment : public PaymentGateway {
 public:
  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processed Credit Card payment of $" +
                      utils::toStringWithPrecision(amount, 2) + "\n"};
  }
};

class PayPalPayment : public PaymentGateway {
 public:
  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processed Paypal payment of $" +
                      utils::toStringWithPrecision(amount, 2) + "\n"};
  }
};

class StripePayment : public PaymentGateway {
 public:
  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processed Stripe payment of $" +
                      utils::toStringWithPrecision(amount, 2) + "\n"};
  }
};

class CODPayment : public PaymentGateway {
 public:
  pair<bool, string> processPayment(const double& amount) override {
    return {true, "Processed COD payment of $" +
                      utils::toStringWithPrecision(amount, 2) + "\n"};
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
