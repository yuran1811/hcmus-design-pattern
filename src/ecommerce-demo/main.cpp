#include "Payment.hpp"
#include "Order.hpp"
#include "Stage.hpp"

int main() {
  // Payment gateways
  PaymentGateway* paypalGateway =
      PaymentGatewayFactory::createPaymentGateway("paypal");
  PaymentGateway* creditGateway =
      PaymentGatewayFactory::createPaymentGateway("credit");

  if (paypalGateway) paypalGateway->processPayment(100.0);
  if (creditGateway) creditGateway->processPayment(120.0);
  cout << endl;

  // Order with decorators
  Order* order =
      new ExpressDeliveryDecorator(new GiftWrapDecorator(new BasicOrder()));
  order->placeOrder();
  cout << endl;

  // Order stages
  OrderStage* payment = new PaymentStage();
  OrderStage* packaging = new PackagingStage();
  OrderStage* shipping = new ShippingStage();

  packaging->setNextStage(move(shipping));
  payment->setNextStage(move(packaging));

  payment->handleOrder();

  // Clean up
  delete paypalGateway;
  delete creditGateway;
  delete order;
  delete payment;

  return 0;
}
