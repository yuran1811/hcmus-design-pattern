#pragma once

#include <memory>

#include "../utils/index.hpp"

#include "Order.hpp"

using std::shared_ptr;

class OrderContext;

class ArchivedOrder {
 private:
  const string ORDER_ID;
  const string ORDER_DATE;

  const Price totalCost;

  const string address;
  const string phone;

  const PaymentMethod paymentMethod;

 public:
  ArchivedOrder() = delete;
  ArchivedOrder(const string&, const OrderContext&, shared_ptr<Order>);
  ~ArchivedOrder() = default;

  const string& getOrderId() const { return ORDER_ID; };
  const string& getOrderDate() const { return ORDER_DATE; };
  const Price& getTotalCost() const { return totalCost; };

  string toString() const;
};

#include "Stage.hpp"
