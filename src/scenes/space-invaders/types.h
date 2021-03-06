#pragma once
#include "invaders_utils.h"
#include <glm/glm.hpp>

#if defined(_WIN32)
//structure was padded due to alignment specifier
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

static constexpr uint32_t MAX_BULLETS = 1000;
struct Bullets {
  alignas(SIMD_ALIGNMENT) float x[MAX_BULLETS];
  alignas(SIMD_ALIGNMENT) float y[MAX_BULLETS];
  uint32_t nrBullets;
  vec2 direction;
  float speed;
};

static constexpr uint32_t ALIEN_ROWS = 10;
static constexpr uint32_t ALIEN_COLS = 21;
static constexpr uint32_t MAX_ALIENS = ALIEN_ROWS * ALIEN_COLS;
static constexpr uint32_t ALIEN_DIR_SIZE = 4;
static constexpr int32_t AlienXDirs[ALIEN_DIR_SIZE] = {1, 0, -1, 0};
static constexpr int32_t AlienYDirs[ALIEN_DIR_SIZE] = {0, -1, 0, -1};
struct Aliens {
  alignas(SIMD_ALIGNMENT) float x[MAX_ALIENS];
  alignas(SIMD_ALIGNMENT) float y[MAX_ALIENS];
  glm::vec4 colors[MAX_ALIENS];
  uint32_t nrAliens;
  float speed;
  float currentDistanceMoveInY;
  uint32_t dirIndex;
};
#if defined(_WIN32)
#pragma warning(pop)
#endif

struct Player {
  vec2 position;
  int32_t health;
  float speed;
  float msSinceLastFire;
  float weaponColdDown;
};