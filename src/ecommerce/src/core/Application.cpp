#include "Application.hpp"

Application::Application()
    : isRunning(false),
      gui(new GUI()),
      currentStage(SELECT_ITEM),
      totalCost(0),
      address(""),
      phone(""),
      paymentMethod(CREDIT_CARD) {
  init();
}

Application::~Application() {
  delete gui;
  gui = nullptr;
}

void Application::setItems(const vector<Item>& _) { items = _; }

void Application::setVouchers(const vector<Voucher>& _) { vouchers = _; }

void Application::switchStage(const OrderStageState& stage) {
  currentStage = stage;
}

void Application::switchPaymentMethod(const PaymentMethod& method) {
  paymentMethod = method;
}

string Application::getStageMessage(OrderStageState stage) {
  switch (stage) {
    case SELECT_ITEM:
      return "Select items to add to your cart.";
    case ADDRESS_INPUT:
      return "Enter your address details.";
    case PAYMENT_METHOD:
      return "Choose your payment method.";
    case PAYMENT:
      return "Processing payment...";
    case PACKAGING:
      return "Packaging your items...";
    case SHIPPING:
      return "Shipping your order...";
    case COMPLETED:
      return "Order completed!";
    default:
      return "Unknown stage";
  }
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
    stageHandler();

    gui->render([this]() {
      gui->renderStageMessage(getStageMessage(currentStage));
      renderHandler();
    });

    gui->incFrameCounter();
  }
}

void Application::stageHandler() {
  bool phoneHandler = false;
  bool addressHandler = false;

  switch (currentStage) {
    case SELECT_ITEM:
      gui->selectItemHandler(items, cart, totalCost);
      if (gui->isCTAClicked()) {
        currentStage = ADDRESS_INPUT;
      }
      break;

    case ADDRESS_INPUT:
      gui->phoneHandler(phone) && gui->addressHandler(address);
      if (gui->isCTAClicked()) {
        currentStage = PAYMENT_METHOD;
      }
      break;

    case PAYMENT_METHOD:
      gui->paymentMethodHandler(paymentMethod, totalCost);
      if (gui->isCTAClicked()) {
        currentStage = PAYMENT;
      }
      break;

    case PAYMENT:
      if (gui->isCTAClicked()) {
        currentStage = PACKAGING;
      }
      break;

    case PACKAGING:
      if (gui->isCTAClicked()) {
        currentStage = SHIPPING;
      }
      break;

    case SHIPPING:
      if (gui->isCTAClicked()) {
        currentStage = COMPLETED;
      }
      break;

    case COMPLETED:
      if (gui->isCTAClicked()) {
        currentStage = SELECT_ITEM;
        totalCost = 0;
        cart.clear();
      }
      break;

    default:
      break;
  }
}

void Application::renderHandler() {
  switch (currentStage) {
    case SELECT_ITEM:
      gui->renderSelectItem(items, cart, totalCost);
      break;

    case ADDRESS_INPUT:
      gui->renderAddressInfo(address, phone);
      break;

    case PAYMENT_METHOD:
      gui->renderPaymentMethod(paymentMethod, totalCost);
      break;

    case PAYMENT:
      gui->renderPayment();
      break;

    case PACKAGING:
      gui->renderPackaging();
      break;

    case SHIPPING:
      gui->renderShipping();
      break;

    case COMPLETED:
      gui->renderCompleted(totalCost, address, paymentMethod);
      break;

    default:
      break;
  }
}