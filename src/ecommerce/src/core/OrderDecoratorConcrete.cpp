#include "Order.hpp"

GiftWrapDecorator::GiftWrapDecorator(shared_ptr<Order> order, Price fee)
    : OrderDecorator(move(order)), giftWrapFee(fee) {}

Price GiftWrapDecorator::calculateTotal() const {
  return wrappedOrder->calculateTotal() + giftWrapFee;
}

pair<bool, vector<string>> GiftWrapDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back("Adding gift wrap. Fee: $" +
                                      giftWrapFee.format());

  return {true, wrappedOrderReturn.second};
}

ExpressDeliveryDecorator::ExpressDeliveryDecorator(shared_ptr<Order> order)
    : OrderDecorator(move(order)), expressFee(Price(0)), isAvailable(false) {
  fetchDeliveryDetails();
}

void ExpressDeliveryDecorator::fetchDeliveryDetails() {
  // Mocking API call to fetch express delivery fee and availability
  expressFee = Price(2425, 2);  // Simulating fee
  isAvailable = true;           // Simulating availability
}

Price ExpressDeliveryDecorator::calculateTotal() const {
  if (isAvailable) return wrappedOrder->calculateTotal() + expressFee;

  return wrappedOrder->calculateTotal();
}

pair<bool, vector<string>> ExpressDeliveryDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back(
      isAvailable ? "Adding express delivery. Fee: $" + expressFee.format()
                  : "Express delivery is not available for your location.");

  return {isAvailable, wrappedOrderReturn.second};
}
