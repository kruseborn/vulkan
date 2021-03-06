#include "rendering.h"

#include "mg/mgSystem.h"
#include "vulkan/pipelineContainer.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

namespace mg {

static mg::Pipeline createSolidRendering(const mg::RenderContext &renderContext) {
  using namespace mg::shaders::solid;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;
  pipelineStateDesc.rasterization.rasterization.cullMode = VK_CULL_MODE_NONE;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  createPipelineInfo.vertexInputState = InputAssembler::vertexInputState;
  createPipelineInfo.vertexInputStateCount = mg::countof(InputAssembler::vertexInputState);

  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  return pipeline;
}

void renderSolidBoxes(const mg::RenderContext &renderContext, const float *xPositions, const float *yPositions,
                      const glm::vec4 *colors, glm::vec2 size, uint32_t count, bool useSameColor) {
  const auto solidPipeline = createSolidRendering(renderContext);
  using namespace mg::shaders::solid;

  using VertexInputData = InputAssembler::VertexInputData;

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *ubo =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);
  ubo->mvp = glm::ortho(0.0f, float(vkContext.screen.width), 0.0f, float(vkContext.screen.height), -10.0f, 10.0f);
  VkBuffer storageBuffer;
  uint32_t storageOffset;
  VkDescriptorSet storageSet;
  Storage::StorageData *storage = (Storage::StorageData *)mg::mgSystem.linearHeapAllocator.allocateStorage(
      sizeof(Storage::StorageData) * count, &storageBuffer, &storageOffset, &storageSet);

  for (uint32_t i = 0; i < count; i++) {
    storage[i].position.x = xPositions[i];
    storage[i].position.y = yPositions[i];
    storage[i].color = useSameColor ? colors[0] : colors[i];
  }

  VkBuffer vertexBuffer;
  VkDeviceSize vertexBufferOffset;
  VertexInputData *vertices = (VertexInputData *)mg::mgSystem.linearHeapAllocator.allocateBuffer(
      sizeof(VertexInputData) * 4, &vertexBuffer, &vertexBufferOffset);

  constexpr uint32_t indicesInputData[6] = {0, 1, 2, 2, 3, 0};
  VkBuffer indexBuffer;
  VkDeviceSize indexBufferOffset;
  uint32_t *indices = (uint32_t *)mg::mgSystem.linearHeapAllocator.allocateBuffer(
      mg::sizeofArrayInBytes(indicesInputData), &indexBuffer, &indexBufferOffset);
  memcpy(indices, indicesInputData, sizeof(indicesInputData));

  // render boxes
  glm::vec3 vertexInputData[4] = {
      {0, 0, 0},
      {0 + size.x, 0, 0},
      {0 + size.x, 0 + size.y, 0},
      {0, 0 + size.y, 0},
  };
  memcpy(vertices, vertexInputData, sizeof(vertexInputData));

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;

  uint32_t offsets[] = {uniformOffset, storageOffset};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, solidPipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(offsets), offsets);

  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, solidPipeline.pipeline);

  vkCmdBindVertexBuffers(mg::vkContext.commandBuffer, 0, 1, &vertexBuffer, &vertexBufferOffset);
  vkCmdBindIndexBuffer(mg::vkContext.commandBuffer, indexBuffer, indexBufferOffset, VK_INDEX_TYPE_UINT32);
  vkCmdDrawIndexed(mg::vkContext.commandBuffer, mg::countof(indicesInputData), count, 0, 0, 0);
} // namespace mg

static mg::Pipeline createTextureRenderingPipeline(const mg::RenderContext &renderContext) {
  using namespace mg::shaders::textureRendering;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  createPipelineInfo.vertexInputState = InputAssembler::vertexInputState;
  createPipelineInfo.vertexInputStateCount = mg::countof(InputAssembler::vertexInputState);

  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  return pipeline;
}

void renderBoxWithTexture(mg::RenderContext &renderContext, const glm::vec4 &position, TextureId textureId) {
  const auto texturePipeline = createTextureRenderingPipeline(renderContext);
  using namespace mg::shaders::textureRendering;

  using VertexInputData = InputAssembler::VertexInputData;

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *dynamic =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);
  dynamic->mvp = glm::mat4(1);

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;
  descriptorSets.textures = mg::getTextureDescriptorSet();

  TextureIndices textureIndices = {};
  textureIndices.textureIndex = mg::getTexture2DDescriptorIndex(textureId);
  vkCmdPushConstants(mg::vkContext.commandBuffer, texturePipeline.layout, VK_SHADER_STAGE_ALL, 0,
                     sizeof(TextureIndices), &textureIndices);

  glm::vec4 vertexInputData[4] = {
      {position.x, position.y, 0, 0},
      {position.x + position.z, position.y, 1, 0},
      {position.x + position.z, position.y + position.w, 1, 1},
      {position.x, position.y + position.w, 0, 1},
  };
  VkBuffer vertexBuffer;
  VkDeviceSize vertexBufferOffset;
  VertexInputData *vertices = (VertexInputData *)mg::mgSystem.linearHeapAllocator.allocateBuffer(
      mg::sizeofArrayInBytes(vertexInputData), &vertexBuffer, &vertexBufferOffset);
  memcpy(vertices, vertexInputData, sizeof(vertexInputData));

  constexpr uint32_t indicesInputData[6] = {0, 1, 2, 2, 3, 0};
  VkBuffer indexBuffer;
  VkDeviceSize indexBufferOffset;
  uint32_t *indices = (uint32_t *)mg::mgSystem.linearHeapAllocator.allocateBuffer(
      mg::sizeofArrayInBytes(indicesInputData), &indexBuffer, &indexBufferOffset);
  memcpy(indices, indicesInputData, sizeof(indicesInputData));

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, texturePipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);
  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, texturePipeline.pipeline);

  vkCmdBindVertexBuffers(mg::vkContext.commandBuffer, 0, 1, &vertexBuffer, &vertexBufferOffset);
  vkCmdBindIndexBuffer(mg::vkContext.commandBuffer, indexBuffer, indexBufferOffset, VK_INDEX_TYPE_UINT32);
  vkCmdDrawIndexed(mg::vkContext.commandBuffer, mg::countof(indicesInputData), 1, 0, 0, 0);
}

static mg::Pipeline createDepthTextureRenderingPipeline(const mg::RenderContext &renderContext) {
  using namespace mg::shaders::depth;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  createPipelineInfo.vertexInputState = InputAssembler::vertexInputState;
  createPipelineInfo.vertexInputStateCount = mg::countof(InputAssembler::vertexInputState);

  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  return pipeline;
}

void renderBoxWithDepthTexture(mg::RenderContext &renderContext, const glm::vec4 &position, TextureId textureId) {
  const auto texturePipeline = createDepthTextureRenderingPipeline(renderContext);
  using namespace mg::shaders::depth;

  using VertexInputData = InputAssembler::VertexInputData;

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *dynamic =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);
  dynamic->mvp = glm::mat4(1);
  dynamic->nearAndFar = {0.1f, 1000.0f};

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;
  descriptorSets.textures = mg::getTextureDescriptorSet();

  TextureIndices textureIndices = {};
  textureIndices.textureIndex = mg::getTexture2DDescriptorIndex(textureId);
  vkCmdPushConstants(mg::vkContext.commandBuffer, texturePipeline.layout, VK_SHADER_STAGE_ALL, 0,
                     sizeof(TextureIndices), &textureIndices);

  glm::vec4 vertexInputData[4] = {
      {position.x, position.y, 0, 0},
      {position.x + position.z, position.y, 1, 0},
      {position.x + position.z, position.y + position.w, 1, 1},
      {position.x, position.y + position.w, 0, 1},
  };
  VkBuffer vertexBuffer;
  VkDeviceSize vertexBufferOffset;
  VertexInputData *vertices = (VertexInputData *)mg::mgSystem.linearHeapAllocator.allocateBuffer(
      mg::sizeofArrayInBytes(vertexInputData), &vertexBuffer, &vertexBufferOffset);
  memcpy(vertices, vertexInputData, sizeof(vertexInputData));

  constexpr uint32_t indicesInputData[6] = {0, 1, 2, 2, 3, 0};
  VkBuffer indexBuffer;
  VkDeviceSize indexBufferOffset;
  uint32_t *indices = (uint32_t *)mg::mgSystem.linearHeapAllocator.allocateBuffer(
      mg::sizeofArrayInBytes(indicesInputData), &indexBuffer, &indexBufferOffset);
  memcpy(indices, indicesInputData, sizeof(indicesInputData));

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, texturePipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);
  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, texturePipeline.pipeline);

  vkCmdBindVertexBuffers(mg::vkContext.commandBuffer, 0, 1, &vertexBuffer, &vertexBufferOffset);
  vkCmdBindIndexBuffer(mg::vkContext.commandBuffer, indexBuffer, indexBufferOffset, VK_INDEX_TYPE_UINT32);
  vkCmdDrawIndexed(mg::vkContext.commandBuffer, mg::countof(indicesInputData), 1, 0, 0, 0);
}

void renderCube(const mg::RenderContext &renderContext, mg::MeshId cubeId, glm::mat4 model, glm::vec4 color) {
  using namespace mg::shaders::solidColor;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  createPipelineInfo.vertexInputState = InputAssembler::vertexInputState;
  createPipelineInfo.vertexInputStateCount = mg::countof(InputAssembler::vertexInputState);

  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *ubo =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);

  ubo->mvp = renderContext.projection * renderContext.view * model;
  ubo->color = color;

  DescriptorSets descriptorSets = {.ubo = uboSet};

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  const auto mesh = mg::getMesh(cubeId);
  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(mg::vkContext.commandBuffer, 0, 1, &mesh.buffer, &offset);
  vkCmdBindIndexBuffer(mg::vkContext.commandBuffer, mesh.buffer, mesh.indicesOffset, VK_INDEX_TYPE_UINT32);

  vkCmdDrawIndexed(mg::vkContext.commandBuffer, mesh.indexCount, 1, 0, 0, 0);
}

void renderFluid(const mg::RenderContext &renderContext, mg::StorageId density) {
  using namespace mg::shaders::fluid;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayoutStorage;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *ubo =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);

  ubo->screenSize = glm::uvec4{vkContext.screen.width, vkContext.screen.height, 0, 0};

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;
  descriptorSets.d = mg::mgSystem.storageContainer.getStorage(density).descriptorSet;

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
  vkCmdDraw(mg::vkContext.commandBuffer, 3, 1, 0, 0);
}

void renderFluid2(const mg::RenderContext &renderContext, float *density, size_t size) {
  using namespace mg::shaders::fluid2;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);

  VkBuffer storageBuffer;
  uint32_t storageOffset;
  VkDescriptorSet storageSet;
  Storage::StorageData *storage = (Storage::StorageData *)mg::mgSystem.linearHeapAllocator.allocateStorage(
      sizeof(float) * size, &storageBuffer, &storageOffset, &storageSet);

  memcpy(storage, density, sizeof(float) * size);

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *ubo =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);

  ubo->screenSize = glm::uvec4{vkContext.screen.width, vkContext.screen.height, 0, 0};

  DescriptorSets descriptorSets = {.ubo = uboSet};

  uint32_t dynamicOffsets[] = {uniformOffset, storageOffset};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
  vkCmdDraw(mg::vkContext.commandBuffer, 3, 1, 0, 0);
}

} // namespace mg