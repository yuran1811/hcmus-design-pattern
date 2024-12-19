#include "Order.hpp"

Order::Order() : ORDER_ID(utils::uuid_v4()) {}

const string& Order::getOrderId() const { return ORDER_ID; }

const bool Order::isOrderCompleted() const { return isCompleted; }

const pair<bool, vector<string>>& Order::getPlaceOrderReturn() const {
  return placeOrderReturn;
}

void Order::markOrderCompleted() { isCompleted = true; }
