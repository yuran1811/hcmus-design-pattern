#pragma once

#include <cmath>
#include <vector>

#include "raylib.h"

using std::vector;

struct Confetti {
  Color color;
  Vector2 position;
  Vector2 velocity;
  float lifetime;
  float rotation;
  float rotationSpeed;
  float flipProgress;
  float flipSpeed;
};

class ConfettiParticles {
 public:
  static const float GRAVITY;

 private:
  vector<Confetti*> confetti;

  int numParticles;
  int screenWidth;
  int screenHeight;

  Vector2 canonPosition;

 public:
  ConfettiParticles() = delete;
  ConfettiParticles(int, int, int);
  ~ConfettiParticles();

  void setCanonPosition(Vector2);

  bool isAlive();
  bool isAlive(Confetti*);
  bool outBounds(Confetti*);

  Confetti* generateConfetti();
  Confetti* generateConfetti(Vector2);
  void generateSetOfConfetties();
  void cleanup();

  void update();
  void update(float);
  void render();
};
