#include "Stage.hpp"

OrderContext::OrderContext()
    : currentStage(SELECT_ITEM),
      totalCost(0),
      discount(0),
      address(""),
      phone(""),
      deliveryProvider(
          ExpressDeliveryDecorator::availableProviders.begin()->first),
      paymentMethod(COD) {}

void SelectItemStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                             GUI* gui, Application* app) {
  if (ctx.currentStage > SELECT_ITEM) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
    return;
  }

  gui->selectItemHandler(ctx.cart, ctx.totalCost);

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    app->updateOrderTotalCost();

    ctx.currentStage = static_cast<OrderStageState>(ctx.currentStage + 1);
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
  }
}

void SelectItemStage::render(OrderContext& ctx, GUI* gui, Application* app) {
  gui->renderSelectItem(ctx.cart, ctx.totalCost, ctx.discount);

  static function<void(const string&)> onClick = [&](const string& code) {
    shared_ptr<Order> order = app->getCurrentOrder();

    std::dynamic_pointer_cast<BasicOrder>(order)->setCouponCode(code);

    CouponSystem* cs = CouponSystem::getInstance();
    ctx.discount =
        cs->applyCoupon(order->getOrderId(), code, ctx.totalCost, true)
            .first.second;
  };

  if (ctx.totalCost > 0) gui->renderCoupons(onClick);
}

void AddressInfoStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                              GUI* gui, Application* app) {
  if (ctx.currentStage > ADDRESS_INPUT) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
    return;
  }

  gui->addressInfoHandler(ctx.address, ctx.phone);

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = static_cast<OrderStageState>(ctx.currentStage + 1);
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
  }
}

void AddressInfoStage::render(OrderContext& ctx, GUI* gui, Application* app) {
  gui->renderAddressInfo(ctx.address, ctx.phone);
}

void ShippingStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                           GUI* gui, Application* app) {
  if (ctx.currentStage > SHIPPING) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
    return;
  }

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    app->applyDeliveryCost();

    ctx.currentStage = static_cast<OrderStageState>(ctx.currentStage + 1);
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
  }
}

void ShippingStage::render(OrderContext& ctx, GUI* gui, Application* app) {
  static function<void(const string&)> onClick = [&](const string& provider) {
    ctx.deliveryProvider = provider;
  };

  gui->renderShipping(ctx, onClick);
}

void PaymentStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                          GUI* gui, Application* app) {
  if (ctx.currentStage > PAYMENT_METHOD) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
    return;
  }

  gui->paymentMethodHandler(ctx.paymentMethod,
                            app->getCurrentOrder()->calculateTotal());

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    if (gui->getConfettiActive()) gui->stopConfetti();

    ctx.currentStage = static_cast<OrderStageState>(ctx.currentStage + 1);
    if (nextStage) nextStage->handle(stageSystem, ctx, gui, app);
  }
}

void PaymentStage::render(OrderContext& ctx, GUI* gui, Application* app) {
  static function<Price()> getPaymentAmount = [&]() {
    return app->getCurrentOrder()->calculateTotal();
  };

  static function<void()> onPaymentSuccess = [&]() {
    PaymentGatewayRegistry& registry = PaymentGatewayRegistry::getInstance();
    ctx.paymentInfo = make_pair(
        registry.createPaymentGateway(PAYMENT_METHODS[ctx.paymentMethod])
            ->processPayment(double(getPaymentAmount()))
            .second,
        ctx.totalCost);

    app->addArchivedOrder();
  };

  gui->renderPaymentMethod(app->getCurrentOrder()->getOrderId(),
                           getPaymentAmount(), ctx, onPaymentSuccess);
}

void CompletionStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                             GUI* gui, Application* app) {
  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = SELECT_ITEM;
    ctx.cart.clear();
    ctx.totalCost = 0;
    app->resetOrder();
  }
}

void CompletionStage::render(OrderContext& ctx, GUI* gui, Application* app) {
  gui->renderCompleted(app->getArchivedOrders().back()->getOrderId(),
                       app->getArchivedOrders().back()->getOrderDate(),
                       app->getArchivedOrders().back()->getTotalCost(),
                       ctx.address, ctx.paymentMethod);
}

OrderStageSystem::OrderStageSystem() {
  // Instantiate handlers
  selectItemStage = make_shared<SelectItemStage>();
  addressInfoStage = make_shared<AddressInfoStage>();
  shippingStage = make_shared<ShippingStage>();
  paymentStage = make_shared<PaymentStage>();
  completionStage = make_shared<CompletionStage>();

  // Chain handlers
  selectItemStage->setNext(addressInfoStage);
  addressInfoStage->setNext(shippingStage);
  shippingStage->setNext(paymentStage);
  paymentStage->setNext(completionStage);
}

void OrderStageSystem::stageHandler(OrderContext& ctx, GUI* gui,
                                    Application* app) {
  if (gui->isBackProgressClicked() && SELECT_ITEM < ctx.currentStage &&
      !app->getCurrentOrder()->isOrderCompleted()) {
    ctx.currentStage = static_cast<OrderStageState>(ctx.currentStage - 1);

    gui->processStageBacking(ctx.currentStage);
    if (ctx.currentStage == SELECT_ITEM) app->unwrapOrderDecorator();

    return;
  }

  selectItemStage->handle(*this, ctx, gui, app);
}

void OrderStageSystem::renderHandler(OrderContext& ctx, GUI* gui,
                                     Application* app) {
  gui->renderStageMessage(gui->getStageMessage(ctx.currentStage));

  switch (ctx.currentStage) {
    case SELECT_ITEM:
      selectItemStage->render(ctx, gui, app);
      break;
    case ADDRESS_INPUT:
      addressInfoStage->render(ctx, gui, app);
      break;
    case PAYMENT_METHOD:
      paymentStage->render(ctx, gui, app);
      break;
    case SHIPPING:
      shippingStage->render(ctx, gui, app);
      break;
    case COMPLETED:
      completionStage->render(ctx, gui, app);
      break;

    default:
      break;
  }
}
