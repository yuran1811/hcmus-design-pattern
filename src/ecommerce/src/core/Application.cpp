#include "Application.hpp"

Application::Application()
    : isRunning(false),
      gui(make_shared<GUI>()),
      currentOrder(make_shared<BasicOrder>(Price(0), "")) {
  init();
}

void Application::setItems(const vector<Item>& _) {
  items = _;
  if (gui) gui->setItems(_);
}

void Application::setCoupons(const vector<Coupon>& _) {
  coupons = _;
  if (gui) gui->setCoupons(_);
}

void Application::updateTotalCost() {
  orderContext.totalCost = currentOrder->calculateTotal();
}

void Application::updateOrderTotalCost() {
  if (currentOrder)
    std::dynamic_pointer_cast<BasicOrder>(currentOrder)
        ->setCartTotal(orderContext.totalCost);
}

void Application::applyGiftWrapCost() {
  if (currentOrder) {
    currentOrder = make_shared<GiftWrapDecorator>(currentOrder, Price(55, 1));
    updateTotalCost();
  }
}

void Application::applyDeliveryCost() {
  if (currentOrder) {
    currentOrder = make_shared<ExpressDeliveryDecorator>(currentOrder);
    updateTotalCost();
  }
}

void Application::resetOrder() {
  currentOrder = make_shared<BasicOrder>(Price(0), "");
}

void Application::unwrapOrderDecorator() {
  if (currentOrder) {
    BasicOrder* originalOrder =
        dynamic_cast<BasicOrder*>(currentOrder->unwrap());

    currentOrder = make_shared<BasicOrder>(originalOrder->getCartTotal(),
                                           originalOrder->getCouponCode());
    updateTotalCost();
  }
}

void Application::addArchivedOrder() {
  archivedOrders.push_back(
      make_unique<ArchivedOrder>(currentOrder->getOrderId(), orderContext));
  currentOrder->markOrderCompleted();
}

void Application::init() {
  if (!gui)
    TraceLog(LOG_ERROR, "GUI is not set.");
  else
    gui->init();

  registerPaymentGateways();
}

void Application::run() {
  isRunning = true;

  while (!WindowShouldClose() && isRunning) {
    gui->incFrameTimer();
    gui->incFrameCounter();

    gui->render(orderContext.currentStage, [&]() {
      orderStageSystem.renderHandler(orderContext, gui.get(), this);
      orderStageSystem.stageHandler(orderContext, gui.get(), this);
    });
  }
}
