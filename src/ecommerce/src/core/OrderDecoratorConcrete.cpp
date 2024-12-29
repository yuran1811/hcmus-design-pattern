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

  wrappedOrderReturn.second.push_back("Added gift wrap. Fee: $" +
                                      giftWrapFee.format());

  return {true, wrappedOrderReturn.second};
}

ExpressDeliveryDecorator::ExpressDeliveryDecorator(shared_ptr<Order> order)
    : OrderDecorator(move(order)),
      currentProvider(availableProviders.begin()->first),
      expressFee(availableProviders[currentProvider]),
      isAvailable(true) {}

void ExpressDeliveryDecorator::updateDeliveryProvider(const string& provider) {
  if (availableProviders.find(provider) != availableProviders.end()) {
    currentProvider = provider;
    expressFee = availableProviders[provider];
  }
}

Price ExpressDeliveryDecorator::calculateTotal() const {
  return wrappedOrder->calculateTotal() + expressFee;
}

const string& ExpressDeliveryDecorator::getCurrentDeliveryProvider() const {
  return currentProvider;
}

const Price& ExpressDeliveryDecorator::getExpressFee() const {
  return expressFee;
}

pair<bool, vector<string>> ExpressDeliveryDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back(
      isAvailable ? "Added express delivery. Fee: $" + expressFee.format()
                  : "Express delivery is not available for your location.");

  return {isAvailable, wrappedOrderReturn.second};
}
