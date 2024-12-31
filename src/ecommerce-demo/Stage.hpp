// Abstract Order Stage Handler
class OrderHandler {
 protected:
  shared_ptr<OrderHandler> nextStage;

 public:
  OrderHandler() = default;
  virtual ~OrderHandler() = default;

  void setNext(const shared_ptr<OrderHandler> &next) { nextStage = next; }

  virtual void handle(OrderStageSystem &, OrderContext &, GUI *, Application *) = 0;
  virtual void render(OrderContext &, GUI *, Application *) = 0;
};

// Order Stage Context
class OrderContext {
 public:
  OrderStageState currentStage;

  CartType cart;
  Price totalCost;
  Price discount;

  string address;
  string phone;

  string deliveryProvider;

  PaymentMethod paymentMethod;
  pair<string, Price> paymentInfo;

 public:
  OrderContext();
  ~OrderContext() = default;

  Price getFinalCost() const {
    return totalCost > discount ? totalCost - discount : Price(0, 0);
  }
};

// Concrete Order Stages
class SelectItemStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *, Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class AddressInfoStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *, Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class ShippingStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *, Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class PaymentStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *, Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};

class CompletionStage : public OrderHandler {
 public:
  void handle(OrderStageSystem &, OrderContext &, GUI *, Application *) override;
  void render(OrderContext &, GUI *, Application *) override;
};
