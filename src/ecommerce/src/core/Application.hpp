#pragma once

#include <memory>

#include "../gui/index.hpp"

#include "ArchivedOrder.hpp"

using std::make_shared;
using std::shared_ptr;

class Application {
 private:
  vector<Item> items;
  vector<Coupon> coupons;

  bool isRunning;
  shared_ptr<GUI> gui;

  shared_ptr<Order> currentOrder;
  OrderContext orderContext;
  OrderStageSystem orderStageSystem;
  vector<unique_ptr<ArchivedOrder>> archivedOrders;

 public:
  Application();
  ~Application() = default;

  void setItems(const vector<Item>&);
  void setCoupons(const vector<Coupon>&);

  shared_ptr<Order> getCurrentOrder() const { return currentOrder; }
  const OrderContext& getOrderContext() const { return orderContext; }
  const OrderStageSystem& getOrderStageSystem() const {
    return orderStageSystem;
  }
  const vector<unique_ptr<ArchivedOrder>>& getArchivedOrders() const {
    return archivedOrders;
  }

  void updateTotalCost();
  void updateOrderTotalCost();

  void applyGiftWrapCost();
  void applyDeliveryCost();

  void resetOrder();
  void unwrapOrderDecorator();
  void addArchivedOrder();

  void init();
  void initScreens();
  void run();
};
