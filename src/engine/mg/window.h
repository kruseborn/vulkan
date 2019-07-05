#pragma once

#include "tools.h"
#include <cstdint>
#include <glm/glm.hpp>

namespace mg {

struct FrameData {
  uint32_t width, height;
  bool resize;

  struct {
    glm::vec2 xy, prevXY;
    bool left, middle, right;
  } mouse;
  struct {
    bool r,n,m;
  } keys;
  mg::Tool tool;
};

void initWindow(uint32_t width, uint32_t height);
void destroyWindow();

bool startFrame();
void endFrame();
FrameData getFrameData();

} // namespace mg