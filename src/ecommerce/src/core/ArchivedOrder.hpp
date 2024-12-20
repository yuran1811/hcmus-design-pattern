#pragma once

#include <memory>

#include "../utils/index.hpp"

using std::shared_ptr;

class Order;
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
  ArchivedOrder(shared_ptr<Order>, OrderContext&);
  ~ArchivedOrder() = default;
};

#include "Stage.hpp"
