#include "Confetti.hpp"

const float ConfettiParticles::GRAVITY = 0.98f * 1.175f;

ConfettiParticles::ConfettiParticles(int numParticles, int screenWidth,
                                     int screenHeight)
    : numParticles(numParticles),
      screenWidth(screenWidth),
      screenHeight(screenHeight) {}

ConfettiParticles::~ConfettiParticles() { cleanup(); }

void ConfettiParticles::setCanonPosition(Vector2 _) { canonPosition = _; }

bool ConfettiParticles::isFiring() const { return isFired; }

bool ConfettiParticles::isAlive() {
  for (auto& c : confetti)
    if (isAlive(c)) return true;
  return false;
}

bool ConfettiParticles::isAlive(Confetti* c) {
  return c->lifetime > 0 && !outBounds(c);
}

bool ConfettiParticles::outBounds(Confetti* c) {
  return c->position.x < 0 || c->position.x > screenWidth ||
         c->position.y < 0 || c->position.y > screenHeight;
}

Confetti* ConfettiParticles::generateConfetti() {
  return generateConfetti(canonPosition);
}

Confetti* ConfettiParticles::generateConfetti(Vector2 position) {
  const Color color = {(unsigned char)GetRandomValue(15, 255),
                       (unsigned char)GetRandomValue(15, 255),
                       (unsigned char)GetRandomValue(15, 255), 255};

  float angle = (float)GetRandomValue(60, 120);
  float speed = (float)GetRandomValue(250, 450) / 100.0f;
  const Vector2 velocity = {speed * cosf(angle * DEG2RAD),
                            -speed * sinf(angle * DEG2RAD)};

  // lifetime in seconds
  const float lifetime = (float)GetRandomValue(50, 200) / 10.0f;
  const float rotation = (float)GetRandomValue(0, 360);
  const float rotationSpeed = (float)GetRandomValue(-200, 200) / 1.0f;
  // Start with no flip
  const float flipProgress = 0.0f;
  // Speed of the flip (higher = faster)
  const float flipSpeed = (float)GetRandomValue(0.5f, 1.5f);

  return new Confetti{color,    position,      velocity,     lifetime,
                      rotation, rotationSpeed, flipProgress, flipSpeed};
}

void ConfettiParticles::generateSetOfConfetties() {
  for (int i = 0; i < numParticles; i++) {
    confetti.push_back(generateConfetti());
  }
}

void ConfettiParticles::cleanup() {
  for (auto& c : confetti) delete c;
  confetti.clear();
}

void ConfettiParticles::update() { update(GetFrameTime()); }

void ConfettiParticles::update(float deltaTime) {
  for (auto& c : confetti) {
    if (c->lifetime <= 0) continue;
    c->lifetime -= deltaTime;

    c->velocity.y += GRAVITY * deltaTime;
    c->position.x += c->velocity.x;
    c->position.y += c->velocity.y;
    c->rotation += c->rotationSpeed * deltaTime;

    c->flipProgress += c->flipSpeed * deltaTime;
    if (c->flipProgress > 2 * PI) {
      // Reset flipping cycle
      c->flipProgress -= 2 * PI;
    }
  }

  if (!isAlive()) cleanup();
}

void ConfettiParticles::render() {
  for (auto& c : confetti) {
    if (!isAlive(c)) continue;

    // Flip effect (0 to 1)
    const float flipFactor = fabs(sin(c->flipProgress));
    const float width = 10.0f * (1.0f - flipFactor);

    Rectangle rec = {c->position.x, c->position.y, width, 10};
    Vector2 origin = {width / 2, 5};
    DrawRectanglePro(rec, origin, c->rotation, c->color);
  }
}
