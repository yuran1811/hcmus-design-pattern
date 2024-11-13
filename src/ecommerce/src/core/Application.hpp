#pragma once

#include "Gui.hpp"

class Application {
 private:
  bool isRunning;

  GUI* gui;

  OrderStageState currentStage;

  vector<Item> items;
  vector<Voucher> vouchers;

  CartType cart;
  Price totalCost;

  string address;
  string phone;

  PaymentMethod paymentMethod;

 public:
  Application();
  ~Application();

  void setItems(const vector<Item>&);
  void setVouchers(const vector<Voucher>&);
  void switchStage(const OrderStageState&);
  void switchPaymentMethod(const PaymentMethod&);

  string getStageMessage(OrderStageState);
  OrderStageState getStage() const { return currentStage; }
  PaymentMethod getPaymentMethod() const { return paymentMethod; }

  void init();
  void run();

  void stageHandler();
  void renderHandler();
};
