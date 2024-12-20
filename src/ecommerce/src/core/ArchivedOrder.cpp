#include "ArchivedOrder.hpp"

ArchivedOrder::ArchivedOrder(shared_ptr<Order> order, OrderContext &ctx)
    : ORDER_ID(order->getOrderId()),
      ORDER_DATE(utils::getDateTimeString()),
      totalCost(ctx.totalCost),
      address(ctx.address),
      phone(ctx.phone),
      paymentMethod(ctx.paymentMethod) {}
