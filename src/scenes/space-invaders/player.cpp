#include "player.h"
#include "types.h"
#include "mg/window.h"

void transformPlayer(Player *player, uint32_t screenWidth, float spriteSize, const mg::FrameData &frameData, float dt) {
  assert(player);
  player->position.x -= frameData.keys.left * player->speed * dt;
  player->position.x += frameData.keys.right * player->speed * dt;
  player->position.x = player->position.x > 0.0f ? player->position.x : 0.0f;
  const auto rightMargin = float(screenWidth - spriteSize);
  player->position.x = player->position.x < rightMargin ? player->position.x : rightMargin;
}

bool shouldFirePlayerBullet(Player *player, const mg::FrameData &frameData, float dt) {
  assert(player);
  player->msSinceLastFire += dt;
  uint32_t shouldFire = uint32_t(player->msSinceLastFire > player->weaponColdDown);
  shouldFire *= uint32_t(frameData.keys.space);
  player->msSinceLastFire *= !uint32_t(shouldFire);
  return shouldFire;
}
