#include "GuiComponent.hpp"

map<string, GuiModal *> GuiModal::modals;

GuiModal::GuiModal(const string &id, const string &title, const string &message,
                   Rectangle bounds)
    : modalID(id),
      title(title),
      message(message),
      bounds(bounds),
      messageContainerHeight(calculateMessageHeight()),
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
      modal.second->getEvent(MODAL_EVENT::CLOSE)();
    }
  }
}

float GuiModal::calculateMessageHeight() {
  const Font &font = GetFontDefault();
  const float spacing = 2.f;
  const float scaleFactor = 20.f / 10.f;

  const string &text = message;
  const int length = text.size();
  const Rectangle msgBounds = getMessageBounds();

  float textOffsetY = 0;
  float textOffsetX = 0.0f;

  enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
  bool wordWrap = true;
  int state = MEASURE_STATE;

  int startLine = -1;  // Index where to begin drawing (where a line begins)
  int endLine = -1;    // Index where to stop drawing (where a line ends)
  int selectStart = 0;
  int selectLength = 0;
  int lastk = -1;  // Holds last value of the character position

  for (int i = 0, k = 0; i < length; i++, k++) {
    // Get next codepoint from byte string and glyph index in font
    int codepointByteCount = 0;
    int codepoint = GetCodepoint(&text[i], &codepointByteCount);
    int index = GetGlyphIndex(font, codepoint);

    // NOTE: Normally we exit the decoding sequence as soon as a bad byte is
    // found (and return 0x3f) but we need to draw all of the bad bytes using
    // the '?' symbol moving one byte
    if (codepoint == 0x3f) codepointByteCount = 1;
    i += (codepointByteCount - 1);

    float glyphWidth = 0;
    if (codepoint != '\n') {
      glyphWidth = (font.glyphs[index].advanceX == 0)
                       ? font.recs[index].width * scaleFactor
                       : font.glyphs[index].advanceX * scaleFactor;

      if (i + 1 < length) glyphWidth = glyphWidth + spacing;
    }

    if (state == MEASURE_STATE) {
      if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
        endLine = i;

      if ((textOffsetX + glyphWidth) > msgBounds.width) {
        endLine = (endLine < 1) ? i : endLine;
        if (i == endLine) endLine -= codepointByteCount;
        if ((startLine + codepointByteCount) == endLine)
          endLine = (i - codepointByteCount);

        state = !state;
      } else if ((i + 1) == length) {
        endLine = i;
        state = !state;
      } else if (codepoint == '\n')
        state = !state;

      if (state == DRAW_STATE) {
        textOffsetX = 0;
        i = startLine;
        glyphWidth = 0;

        // Save character position when we switch states
        int tmp = lastk;
        lastk = k - 1;
        k = tmp;
      }
    } else {
      if (codepoint == '\n') {
        if (!wordWrap) {
          textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
          textOffsetX = 0;
        }
      } else {
        if (!wordWrap && ((textOffsetX + glyphWidth) > msgBounds.width)) {
          textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
          textOffsetX = 0;
        }

        if ((selectStart >= 0) && (k >= selectStart) &&
            (k < (selectStart + selectLength))) {
        }
      }

      if (wordWrap && (i == endLine)) {
        textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
        textOffsetX = 0;
        startLine = endLine;
        endLine = -1;
        glyphWidth = 0;
        selectStart += lastk - k;
        k = lastk;

        state = !state;
      }
    }

    if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;
  }

  return textOffsetY;
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

void GuiModal::setEvent(MODAL_EVENT e, function<void()> _) { events[e] = _; }
