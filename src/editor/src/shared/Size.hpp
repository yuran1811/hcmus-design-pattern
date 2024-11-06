#pragma once

class Size {
 public:
  float width;
  float height;

  Size() : width(0), height(0){};
  Size(float width) : width(width), height(width){};
  Size(float width, float height) : width(width), height(height){};

  Size& operator=(const Size& other) {
    width = other.width;
    height = other.height;
    return *this;
  }
};

class ScreenSize : public Size {
 public:
  ScreenSize() : Size(){};
  ScreenSize(float width) : Size(width){};
  ScreenSize(float width, float height) : Size(width, height){};

  ScreenSize& operator=(const ScreenSize& other) {
    width = other.width;
    height = other.height;
    return *this;
  }
};