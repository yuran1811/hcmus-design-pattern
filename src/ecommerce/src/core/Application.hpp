#pragma once

#include <memory>

#include "Gui.hpp"
#include "Stage.hpp"

using std::make_shared;
using std::shared_ptr;

class Application {
 private:
  vector<Item> items;
  vector<Coupon> coupons;

  bool isRunning;
  shared_ptr<GUI> gui;

  OrderContext orderContext;
  OrderStageSystem orderStageSystem;

 public:
  Application();
  ~Application() = default;

  void setItems(const vector<Item>&);
  void setCoupons(const vector<Coupon>&);

  void init();
  void run();
};
