#pragma once

#include "../utils/index.hpp"

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
  ArchivedOrder(const string&, const OrderContext&);
  ~ArchivedOrder() = default;

  string toString() const;
};

#include "Stage.hpp"
