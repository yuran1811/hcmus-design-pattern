#include "Application.hpp"

Application::Application() : isRunning(false), gui(make_shared<GUI>()) {
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

    gui->render(orderContext.currentStage, [this]() {
      orderStageSystem.renderHandler(orderContext, gui.get());
    });
    orderStageSystem.stageHandler(orderContext, gui.get());
  }
}
