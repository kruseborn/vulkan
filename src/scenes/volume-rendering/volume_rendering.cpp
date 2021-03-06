#include "volume_rendering.h"
#include "mg/camera.h"
#include "mg/geometryUtils.h"
#include "mg/mgSystem.h"
#include "mg/window.h"
#include "rendering/rendering.h"
#include "volume_renderpass.h"
#include "volume_utils.h"

static mg::Pipeline createFrontAndBackPipeline(const mg::RenderContext &renderContext) {
  using namespace mg::shaders::frontAndBack;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.rasterization.cullMode = VK_CULL_MODE_NONE;
  pipelineStateDesc.rasterization.rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.graphics.nrOfColorAttachments = 2;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;
  createPipelineInfo.vertexInputState = InputAssembler::vertexInputState;
  createPipelineInfo.vertexInputStateCount = mg::countof(InputAssembler::vertexInputState);

  const auto mrtPipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  return mrtPipeline;
}

void drawFrontAndBack(const mg::RenderContext &renderContext, const VolumeInfo &volumeInfo) {
  using namespace mg::shaders::frontAndBack;

  const auto frontAndBackPipeline = createFrontAndBackPipeline(renderContext);
  const auto cubeMesh = mg::createVolumeCube(volumeInfo.corner, volumeInfo.size);

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *dynamic =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);
  dynamic->mvp = renderContext.projection * renderContext.view;
  dynamic->worldToBox = worldToBoxMatrix(volumeInfo);

  VkBuffer buffer;
  VkDeviceSize bufferOffset;
  auto vertices = mg::mgSystem.linearHeapAllocator.allocateBuffer(mg::sizeofArrayInBytes(cubeMesh.vertices), &buffer,
                                                                  &bufferOffset);
  memcpy(vertices, cubeMesh.vertices, mg::sizeofArrayInBytes(cubeMesh.vertices));

  VkBuffer indexBuffer;
  VkDeviceSize indexBufferOffset;
  auto indices = mg::mgSystem.linearHeapAllocator.allocateBuffer(mg::sizeofArrayInBytes(cubeMesh.indices), &indexBuffer,
                                                                 &indexBufferOffset);
  memcpy(indices, cubeMesh.indices, mg::sizeofArrayInBytes(cubeMesh.indices));

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frontAndBackPipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  vkCmdBindVertexBuffers(mg::vkContext.commandBuffer, 0, 1, &buffer, &bufferOffset);
  vkCmdBindIndexBuffer(mg::vkContext.commandBuffer, indexBuffer, indexBufferOffset, VK_INDEX_TYPE_UINT32);
  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frontAndBackPipeline.pipeline);
  vkCmdDrawIndexed(mg::vkContext.commandBuffer, mg::countof(cubeMesh.indices), 1, 0, 0, 0);
}

static mg::Pipeline createVolumePipeline(const mg::RenderContext &renderContext) {
  using namespace mg::shaders::volume;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.rasterization.cullMode = VK_CULL_MODE_NONE;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;

  const auto pipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  return pipeline;
}

void drawVolume(const mg::RenderContext &renderContext, const mg::Camera &camera, const VolumeInfo &volumeInfo,
                const mg::FrameData &frameData, const VolumeRenderPass &volumeRenderPass) {
  using namespace mg::shaders::volume;

  const auto volumePipeline = createVolumePipeline(renderContext);

  static float isoValue = 50;
  if (frameData.keys.n) {
    isoValue -= 0.01f;
  }
  if (frameData.keys.m) {
    isoValue += 0.01f;
  }

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *dynamic =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);
  dynamic->color = glm::vec4{1, 0, 0, 1};
  dynamic->minMaxIsoValue = glm::vec4{volumeInfo.min, volumeInfo.max, isoValue, 0.0f};
  dynamic->boxToWorld = boxToWorldMatrix(volumeInfo);
  dynamic->worldToBox = worldToBoxMatrix(volumeInfo);
  dynamic->mv = renderContext.view;
  dynamic->cameraPosition = glm::vec4{camera.position.x, camera.position.y, camera.position.z, 1.0f};

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;
  descriptorSets.textures = mg::getTextureDescriptorSet();
  descriptorSets.volumeTexture = mg::getTextureDescriptorSet3D();

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, volumePipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  TextureIndices textureIndices = {};
  textureIndices.backIndex = mg::getTexture2DDescriptorIndex(volumeRenderPass.back);
  textureIndices.frontIndex = mg::getTexture2DDescriptorIndex(volumeRenderPass.front);
  textureIndices.volumeIndex = mg::getTexture3DDescriptorIndex(volumeInfo.textureId);

  vkCmdPushConstants(mg::vkContext.commandBuffer, volumePipeline.layout, VK_SHADER_STAGE_ALL, 0, sizeof(TextureIndices),
                     &textureIndices);

  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, volumePipeline.pipeline);
  vkCmdDraw(mg::vkContext.commandBuffer, 3, 1, 0, 0);
}

static mg::Pipeline createImageStoragePipeline(const mg::RenderContext &renderContext) {
  using namespace mg::shaders::denoise;

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.rasterization.cullMode = VK_CULL_MODE_NONE;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;
  pipelineStateDesc.rasterization.depth.writeEnable = VK_FALSE;

  mg::CreatePipelineInfo createPipelineInfo = {};
  createPipelineInfo.shaderName = shader;

  const auto mrtPipeline = mg::mgSystem.pipelineContainer.createPipeline(pipelineStateDesc, createPipelineInfo);
  return mrtPipeline;
}

void drawDenoise(const mg::RenderContext &renderContext, const VolumeRenderPass &volumeRenderPass) {
  using namespace mg::shaders::denoise;

  const auto denoisePipeline = createImageStoragePipeline(renderContext);

  VkBuffer uniformBuffer;
  uint32_t uniformOffset;
  VkDescriptorSet uboSet;
  Ubo *dynamic =
      (Ubo *)mg::mgSystem.linearHeapAllocator.allocateUniform(sizeof(Ubo), &uniformBuffer, &uniformOffset, &uboSet);
  dynamic->color = glm::vec4{1, 0, 0, 1};

  DescriptorSets descriptorSets = {};
  descriptorSets.ubo = uboSet;
  descriptorSets.textures = mg::getTextureDescriptorSet();

  uint32_t dynamicOffsets[] = {uniformOffset, 0};
  vkCmdBindDescriptorSets(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, denoisePipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  TextureIndices textureIndices = {};
  textureIndices.textureIndex = mg::getTexture2DDescriptorIndex(volumeRenderPass.color);
  vkCmdPushConstants(mg::vkContext.commandBuffer, denoisePipeline.layout, VK_SHADER_STAGE_ALL, 0,
                     sizeof(TextureIndices), &textureIndices);

  vkCmdBindPipeline(mg::vkContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, denoisePipeline.pipeline);
  vkCmdDraw(mg::vkContext.commandBuffer, 3, 1, 0, 0);
}
