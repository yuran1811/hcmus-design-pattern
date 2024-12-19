#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <stack>

#include "../shared/index.hpp"
#include "Gui.hpp"

using std::cout;
using std::function;
using std::make_shared;
using std::shared_ptr;
using std::stack;

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

  virtual void handle(OrderStageSystem &, OrderContext &, GUI *) = 0;
  virtual void render(OrderContext &, GUI *) = 0;
};

class OrderContext {
 public:
  OrderStageState currentStage;

  CartType cart;
  Price totalCost;

  string address;
  string phone;

  PaymentMethod paymentMethod;

 public:
  OrderContext();
  ~OrderContext() = default;
};

class SelectItemStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *) override;
  void render(OrderContext &, GUI *) override;
};

class AddressInfoStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *) override;
  void render(OrderContext &, GUI *) override;
};

class PaymentStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *) override;
  void render(OrderContext &, GUI *) override;
};

class ShippingStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *) override;
  void render(OrderContext &, GUI *) override;
};

class CompletionStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *) override;
  void render(OrderContext &, GUI *) override;
};

class OrderStageSystem {
 private:
  shared_ptr<SelectItemStage> selectItemStage;
  shared_ptr<AddressInfoStage> addressInfoStage;
  shared_ptr<PaymentStage> paymentStage;
  shared_ptr<ShippingStage> shippingStage;
  shared_ptr<CompletionStage> completionStage;

 public:
  OrderStageSystem();
  ~OrderStageSystem() = default;

  void stageHandler(OrderContext &, GUI *);
  void renderHandler(OrderContext &, GUI *);
};