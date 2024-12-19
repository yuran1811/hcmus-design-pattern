#include "Order.hpp"

GiftWrapDecorator::GiftWrapDecorator(shared_ptr<Order> order, float fee)
    : OrderDecorator(move(order)), giftWrapFee(fee) {}

float GiftWrapDecorator::calculateTotal() const {
  return wrappedOrder->calculateTotal() + giftWrapFee;
}

pair<bool, vector<string>> GiftWrapDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back("Adding gift wrap. Fee: $" +
                                      std::to_string(giftWrapFee));

  return {true, wrappedOrderReturn.second};
}

ExpressDeliveryDecorator::ExpressDeliveryDecorator(shared_ptr<Order> order)
    : OrderDecorator(move(order)), expressFee(0.0f), isAvailable(false) {
  fetchDeliveryDetails();
}

void ExpressDeliveryDecorator::fetchDeliveryDetails() {
  // Mocking API call to fetch express delivery fee and availability
  expressFee = 24.25f;  // Simulating fee
  isAvailable = true;   // Simulating availability
}

float ExpressDeliveryDecorator::calculateTotal() const {
  if (isAvailable) return wrappedOrder->calculateTotal() + expressFee;

  return wrappedOrder->calculateTotal();
}

pair<bool, vector<string>> ExpressDeliveryDecorator::placeOrder() {
  if (isOrderCompleted()) return {false, {"Order already placed"}};

  auto wrappedOrderReturn = wrappedOrder->placeOrder();
  if (!wrappedOrderReturn.first) return wrappedOrderReturn;

  wrappedOrderReturn.second.push_back(
      isAvailable
          ? "Adding express delivery. Fee: $" + std::to_string(expressFee)
          : "Express delivery is not available for your location.");

  return {isAvailable, wrappedOrderReturn.second};
}
