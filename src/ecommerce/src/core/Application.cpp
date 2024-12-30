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
  orderContext.totalCost =
      std::dynamic_pointer_cast<BasicOrder>(currentOrder)->getCartTotal();
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
    if (!dynamic_cast<ExpressDeliveryDecorator*>(currentOrder.get()))
      currentOrder = make_shared<ExpressDeliveryDecorator>(currentOrder);

    std::dynamic_pointer_cast<ExpressDeliveryDecorator>(currentOrder)
        ->updateDeliveryProvider(orderContext.deliveryProvider);
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
  archivedOrders.push_back(make_unique<ArchivedOrder>(
      currentOrder->getOrderId(), orderContext, currentOrder));
  currentOrder->markOrderCompleted();
}

void Application::init() {
  if (!gui) return void(TraceLog(LOG_ERROR, "GUI is not set."));

  registerPaymentGateways();

  gui->init();
  initScreens();
}

void Application::initScreens() {
  ScreenManager& instance = ScreenManager::getInstance();

  instance.addScreen(
      GUIScreen::NOT_FOUND,
      [&]() {
        gui->renderHeader("Interactive E-Commerce Order Processing");

        const float text404Width = MeasureText("404", 124);
        DrawText("404", SCREEN_SIZE.width / 2 - text404Width / 2,
                 SCREEN_SIZE.height / 3 - 50, 124, VIOLET);

        const float textNotFoundWidth = MeasureText("Page Not Found", 28);
        DrawText("Page Not Found",
                 SCREEN_SIZE.width / 2 - textNotFoundWidth / 2,
                 SCREEN_SIZE.height / 2, 28, VIOLET);
      },
      [&]() {});

  instance.addScreen(
      GUIScreen::MAIN,
      [&]() {
        gui->renderHeader("Interactive E-Commerce Order Processing");
        gui->renderOrderProgress(orderContext.currentStage,
                                 currentOrder->isOrderCompleted());

        orderStageSystem.renderHandler(orderContext, gui.get(), this);
      },
      [&]() { orderStageSystem.stageHandler(orderContext, gui.get(), this); });

  instance.addScreen(
      GUIScreen::ARCHIVED,
      [&]() {
        gui->renderHeader("Archived Orders");
        gui->renderStageMessage("Viewing archived orders");
        gui->renderArchivedOrderList(archivedOrders);
      },
      [&]() { gui->archivedOrderHandler(); });
}

void Application::run() {
  isRunning = true;

  while (!WindowShouldClose() && isRunning) {
    gui->incFrameTimer();
    gui->incFrameCounter();

    gui->render(
        this, [&]() { ScreenManager::getInstance().renderCurrentScreen(); },
        [&]() { ScreenManager::getInstance().handleCurrentScreen(); });
  }
}
