#include "Gui.hpp"

GUI::GUI() {
  inputActiveStates.unset((unsigned int)(InputType::ADDRESS));
  inputActiveStates.unset((unsigned int)(InputType::PHONE));
}

GUI::~GUI() {
  vector<Texture2D>& itemTextures = getTextureCollection();
  for (Texture2D& texture : itemTextures) UnloadTexture(texture);

  CloseWindow();
}

int GUI::getFrameCounter() const { return frameCounter; }

vector<Texture2D>& GUI::getTextureCollection() {
  static vector<Texture2D> textures(1);
  return textures;
}

void GUI::incFrameCounter() {
  frameCounter++;
  if (frameCounter >= TARGET_FPS) frameCounter = 0;
}

void GUI::setCTARec(const Rectangle& rec) { ctaRec = rec; }

void GUI::init() {
  SetTraceLogCallback(utils::CustomLog);
  SetTargetFPS(TARGET_FPS);
  InitWindow(SCREEN_SIZE.width, SCREEN_SIZE.height, APP_TITLE.c_str());
  SetExitKey(KEY_NULL);
}
