#include "ArchivedOrder.hpp"

ArchivedOrder::ArchivedOrder(const string &id, const OrderContext &ctx)
    : ORDER_ID(id),
      ORDER_DATE(utils::getDateTimeString()),
      totalCost(ctx.totalCost),
      address(ctx.address),
      phone(ctx.phone),
      paymentMethod(ctx.paymentMethod) {}

string ArchivedOrder::toString() const {
  return "Order ID: " + ORDER_ID + "\n" + "Order Date: " + ORDER_DATE + "\n" +
         "Total Cost: " + to_string(totalCost) + "\n" + "Address: " + address +
         "\n" + "Phone: " + phone + "\n" +
         "Payment Method: " + PAYMENT_METHODS[paymentMethod];
}
