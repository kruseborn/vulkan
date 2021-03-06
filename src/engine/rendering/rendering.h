#pragma once
#include "mg/meshContainer.h"
#include "mg/textureContainer.h"
#include "mg/storageContainer.h"
#include "vulkan/vkContext.h"
#include <glm/glm.hpp>
#include <string>

namespace mg {
struct Texts;

struct RenderContext {
  VkRenderPass renderPass;
  uint32_t subpass;
  glm::mat4 projection, view;
};

void renderSolidBoxes(const mg::RenderContext &renderContext, const float *xPositions,
                      const float *yPositions, const glm::vec4 *colors, glm::vec2 size,
                      uint32_t count, bool useSameColor);
void renderBoxWithTexture(mg::RenderContext &renderContext, const glm::vec4 &position,
                          TextureId textureId);
void renderBoxWithDepthTexture(mg::RenderContext &renderContext,
                               const glm::vec4 &position, TextureId textureId);
void renderText(const mg::RenderContext &renderContext, const mg::Texts &texts);
void renderCube(const mg::RenderContext &renderContext, mg::MeshId cubeId,
                glm::mat4 model, glm::vec4 color);
void renderFluid(const mg::RenderContext &renderContext, mg::StorageId density);
void renderFluid2(const mg::RenderContext &renderContext, float *density, size_t size);



} // namespace mg