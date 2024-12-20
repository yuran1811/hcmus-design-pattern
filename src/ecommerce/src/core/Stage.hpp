#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <stack>

#include "../shared/index.hpp"

using std::cout;
using std::function;
using std::make_shared;
using std::shared_ptr;
using std::stack;

class GUI;
class Application;
class OrderContext;
class OrderStageSystem;

// Abstract Order Stage Handler
class OrderHandler : public std::enable_shared_from_this<OrderHandler> {
 protected:
  shared_ptr<OrderHandler> nextStage;

 public:
  OrderHandler() = default;
  virtual ~OrderHandler() = default;

  void setNext(const shared_ptr<OrderHandler> &next) { nextStage = next; }

  virtual void handle(OrderStageSystem &, OrderContext &, GUI *,
                      Application *) = 0;
  virtual void render(OrderContext &, GUI *, Application *) = 0;
};

// Order Stage Context
class OrderContext {
 public:
  OrderStageState currentStage;

  CartType cart;
  Price totalCost;

  string address;
  string phone;

  PaymentMethod paymentMethod;
  pair<string, Price> paymentInfo;

 public:
  OrderContext();
  ~OrderContext() = default;
};

// Concrete Order Stages
class SelectItemStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *,
              Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class AddressInfoStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *,
              Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class ShippingStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *,
              Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class PaymentStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *,
              Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class CompletionStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *,
              Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

// Order Stage System
class OrderStageSystem {
 private:
  shared_ptr<SelectItemStage> selectItemStage;
  shared_ptr<AddressInfoStage> addressInfoStage;
  shared_ptr<ShippingStage> shippingStage;
  shared_ptr<PaymentStage> paymentStage;
  shared_ptr<CompletionStage> completionStage;

 public:
  OrderStageSystem();
  ~OrderStageSystem() = default;

  void renderHandler(OrderContext &, GUI *, Application *);
  void stageHandler(OrderContext &, GUI *, Application *);
};

#include "Application.hpp"
