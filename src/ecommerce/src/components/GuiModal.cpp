#include "GuiComponent.hpp"

map<string, GuiModal *> GuiModal::modals;

GuiModal::GuiModal(const string &id, const string &title, const string &message,
                   Rectangle bounds)
    : modalID(id),
      title(title),
      message(message),
      bounds(bounds),
      messageContainerHeight(
          utils::calcTextWrapHeight(message, getMessageBounds().width)),
      msgText(GuiTextWrap(GuiText(message, {0, 0}, 20, DARKGRAY),
                          getMessageContainer(), LIGHTGRAY,
                          getMessageWrapperClamp())),
      scrollableMessageFrame(
          GuiScrollableFrame(getMessageBounds(), getMessageContainer())) {
  if (modals.find(id) == modals.end()) modals[id] = this;
}

Rectangle GuiModal::getTitleBounds() const {
  return {bounds.x + 12.f, bounds.y + 12.f, bounds.width - 2 * 12.f, 60.f};
}

Rectangle GuiModal::getTitleContainer() const {
  const Rectangle titleBounds = getTitleBounds();
  return {titleBounds.x, titleBounds.y, titleBounds.width, titleBounds.height};
}

Rectangle GuiModal::getTitleWrapperClamp() const {
  const Rectangle container = getTitleContainer();
  return {container.width, container.height, container.width,
          max(container.height,
              MeasureText(title.c_str(), 20) / (container.width / 20.f))};
}

Rectangle GuiModal::getMessageBounds() const {
  const Rectangle titleBounds = getTitleBounds();
  return {titleBounds.x, titleBounds.y + 60.f, titleBounds.width,
          bounds.height - 2 * 12.f - titleBounds.height};
}

Rectangle GuiModal::getMessageContainer() const {
  const Rectangle msgBounds = getMessageBounds();

  return {msgBounds.x, msgBounds.y, msgBounds.width,
          max(msgBounds.height, messageContainerHeight)};
}

Rectangle GuiModal::getMessageWrapperClamp() const {
  const Rectangle container = getMessageContainer();
  return {container.width, container.height, container.width, container.height};
}

Rectangle GuiModal::getCloseBtnBounds() const {
  const Rectangle titleBounds = getTitleBounds();
  const float closeBtnSize = 23.f;
  static const Rectangle closeBtnBounds = {
      titleBounds.x + titleBounds.width - closeBtnSize - 12.f,
      titleBounds.y + 3.f, closeBtnSize, closeBtnSize};
  return closeBtnBounds;
}

void GuiModal::releaseModals() {
  for (auto &modal : modals) delete modal.second;
  modals.clear();
}

void GuiModal::eventsHandler() {
  for (auto &modal : modals) {
    if (!modal.second) {
      modals.erase(modal.first);
      continue;
    }

    modal.second->scrollableMessageFrame.handleScrolling();

    const Rectangle closeBtnBounds = modal.second->getCloseBtnBounds();
    if (CheckCollisionPointRec(GetMousePosition(), closeBtnBounds) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      modal.second->getEvent(EVENT::CLOSE)();
    }
  }
}

GuiModal *GuiModal::render(const Font &font, bool withEffect, ...) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
  DrawRectangleRounded(bounds, .065f, 40, LIGHTGRAY);
  DrawLine(bounds.x, bounds.y + 2 * 12.f + 60.f / 2, bounds.x + bounds.width,
           bounds.y + 2 * 12.f + 60.f / 2, BLUE);

  const Rectangle titleBounds = getTitleBounds();
  GuiTextWrap(GuiText(title, {titleBounds.x, titleBounds.y}, 20, DARKBLUE),
              titleBounds, SKYBLUE, getTitleWrapperClamp())
      .rawRender(font);

  const Rectangle closeBtnBounds = getCloseBtnBounds();
  DrawRectangleRounded(closeBtnBounds, 1.f, 10, RED);
  DrawText("x", closeBtnBounds.x + 7.f, closeBtnBounds.y, 20, WHITE);

  scrollableMessageFrame.render([&]() {
    msgText.updatePosition(scrollableMessageFrame.getRenderPosition());
    msgText.rawRender(font);
  });

  return this;
}

GuiModal *GuiModal::setEvent(EVENT e, function<void()> _) {
  events[e] = _;
  return this;
}
