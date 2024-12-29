#include "ArchivedOrder.hpp"

ArchivedOrder::ArchivedOrder(const string &id, const OrderContext &ctx,
                             shared_ptr<Order> order)
    : ORDER_ID(id),
      ORDER_DATE(utils::getDateTimeString()),
      totalCost(order->calculateTotal()),
      address(ctx.address),
      phone(ctx.phone),
      paymentMethod(ctx.paymentMethod) {}

string ArchivedOrder::toString() const {
  return "Order ID: " + ORDER_ID
          + "\nOrder Date: " + ORDER_DATE
          + "\nTotal Cost: " + totalCost.format()
          + "\nAddress: " + address
          + "\nPhone: " + phone
          + "\nPayment Method: " + PAYMENT_METHODS[paymentMethod];
}
