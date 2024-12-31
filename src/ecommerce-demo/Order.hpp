// Abstract Order
class Order {
 public:
  virtual ~Order() = default;
  virtual Price calculateTotal() const = 0;
  virtual pair<bool, vector<string>> placeOrder() = 0;
};

// Concrete Order: Basic Order
class BasicOrder : public Order {
 private:
  Price cartTotal;
  string couponCode = "";

 public:
  BasicOrder() = default;
  BasicOrder(Price, const string&);

  const Price& getCartTotal() const { return cartTotal; };
  const string& getCouponCode() const { return couponCode; };

  void setCartTotal(const Price&);
  void setCouponCode(const string& _);

  Price calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};

// Abstract Decorator
class OrderDecorator : public Order {
 protected:
  shared_ptr<Order> wrappedOrder;

 public:
  OrderDecorator() = delete;
  OrderDecorator(shared_ptr<Order> order) : wrappedOrder(move(order)) {}

  Order* unwrap() override {
    return wrappedOrder ? wrappedOrder->unwrap() : this;
  }
};

// Concrete Decorator: Express Delivery
class ExpressDeliveryDecorator : public OrderDecorator {
 public:
  inline static map<string, Price> availableProviders = {
      {"J&T Express", Price(23'15, 2)},
      {"Viettel Post", Price(25'35, 2)},
      {"Ninja Van", Price(21'55, 2)}
  };

 private:
  string currentProvider;
  Price expressFee;

 public:
  ExpressDeliveryDecorator(shared_ptr<Order>);

  void updateDeliveryProvider(const string&);

  const string& getCurrentDeliveryProvider() const;
  const Price& getExpressFee() const;

  Price calculateTotal() const override;
  pair<bool, vector<string>> placeOrder() override;
};
