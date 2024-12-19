#include "Stage.hpp"

OrderContext::OrderContext()
    : currentStage(SELECT_ITEM),
      totalCost(0),
      address(""),
      phone(""),
      paymentMethod(COD) {}

void SelectItemStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                             GUI* gui) {
  if (ctx.currentStage > SELECT_ITEM) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
    return;
  }

  gui->selectItemHandler(ctx.cart, ctx.totalCost);

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = ADDRESS_INPUT;
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
  }
}

void SelectItemStage::render(OrderContext& ctx, GUI* gui) {
  gui->renderSelectItem(ctx.cart, ctx.totalCost);
}

void AddressInfoStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                              GUI* gui) {
  if (ctx.currentStage > ADDRESS_INPUT) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
    return;
  }

  gui->addressInfoHandler(ctx.address, ctx.phone);

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = PAYMENT_METHOD;
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
  }
}

void AddressInfoStage::render(OrderContext& ctx, GUI* gui) {
  gui->renderAddressInfo(ctx.address, ctx.phone);
}

void PaymentStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                          GUI* gui) {
  if (ctx.currentStage > PAYMENT_METHOD) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
    return;
  }

  gui->paymentMethodHandler(ctx.paymentMethod, ctx.totalCost);

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = SHIPPING;
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
  }
}

void PaymentStage::render(OrderContext& ctx, GUI* gui) {
  gui->renderPaymentMethod(ctx.paymentMethod, ctx.totalCost);
}

void ShippingStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                           GUI* gui) {
  if (ctx.currentStage > SHIPPING) {
    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
    return;
  }

  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = COMPLETED;

    if (gui->getConfettiActive()) gui->stopConfetti();

    if (nextStage) nextStage->handle(stageSystem, ctx, gui);
  }
}

void ShippingStage::render(OrderContext& ctx, GUI* gui) {
  gui->renderShipping();
}

void CompletionStage::handle(OrderStageSystem& stageSystem, OrderContext& ctx,
                             GUI* gui) {
  if (gui->isCTAClicked()) {
    gui->setShowCTA(false);

    ctx.currentStage = SELECT_ITEM;
    ctx.cart.clear();
    ctx.totalCost = 0;
  }
}

void CompletionStage::render(OrderContext& ctx, GUI* gui) {
  gui->renderCompleted(ctx.totalCost, ctx.address, ctx.paymentMethod);
}

OrderStageSystem::OrderStageSystem() {
  // Instantiate handlers
  selectItemStage = make_shared<SelectItemStage>();
  addressInfoStage = make_shared<AddressInfoStage>();
  paymentStage = make_shared<PaymentStage>();
  shippingStage = make_shared<ShippingStage>();
  completionStage = make_shared<CompletionStage>();

  // Chain handlers
  selectItemStage->setNext(addressInfoStage);
  addressInfoStage->setNext(paymentStage);
  paymentStage->setNext(shippingStage);
  shippingStage->setNext(completionStage);
}

void OrderStageSystem::stageHandler(OrderContext& ctx, GUI* gui) {
  if (ctx.currentStage > SELECT_ITEM && gui->isBackProgressClicked()) {
    ctx.currentStage = static_cast<OrderStageState>(ctx.currentStage - 1);
    gui->processStageBacking(ctx.currentStage);
    return;
  }

  selectItemStage->handle(*this, ctx, gui);
}

void OrderStageSystem::renderHandler(OrderContext& ctx, GUI* gui) {
  gui->renderStageMessage(gui->getStageMessage(ctx.currentStage));

  switch (ctx.currentStage) {
    case SELECT_ITEM:
      selectItemStage->render(ctx, gui);
      break;
    case ADDRESS_INPUT:
      addressInfoStage->render(ctx, gui);
      break;
    case PAYMENT_METHOD:
      paymentStage->render(ctx, gui);
      break;
    case SHIPPING:
      shippingStage->render(ctx, gui);
      break;
    case COMPLETED:
      completionStage->render(ctx, gui);
      break;

    default:
      break;
  }
}
