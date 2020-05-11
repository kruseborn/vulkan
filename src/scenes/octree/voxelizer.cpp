#include "voxelizer.h"

#include "empty_render_pass.h"
#include "mg/mgSystem.h"
#include "rendering/rendering.h"
#include <iostream>
#include <stack>

namespace mg {

struct Compute {
  VkFence fence;
  VkCommandBuffer commandBuffer;
};

static Compute createCompute() {
  Compute compute = {};

  // create and begin command buffer recording
  VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
  commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.commandPool = vkContext.commandPool;
  commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount = 1;

  checkResult(vkAllocateCommandBuffers(vkContext.device, &commandBufferAllocateInfo, &compute.commandBuffer));

  VkCommandBufferBeginInfo vkCommandBufferBeginInfo = {};
  vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  vkCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  checkResult(vkBeginCommandBuffer(compute.commandBuffer, &vkCommandBufferBeginInfo));

  // create and reset fence
  VkFenceCreateInfo vkFenceCreateInfo = {};
  vkFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  vkFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  checkResult(vkCreateFence(vkContext.device, &vkFenceCreateInfo, nullptr, &compute.fence));
  checkResult(vkResetFences(vkContext.device, 1, &compute.fence));

  return compute;
}

static void destroyCompute(Compute *compute) {
  vkFreeCommandBuffers(vkContext.device, vkContext.commandPool, 1, &compute->commandBuffer);
  vkDestroyFence(vkContext.device, compute->fence, nullptr);
}

std::vector<glm::uvec2> voxelizeMesh(const RenderContext &renderContext, const EmptyRenderPass emptyRenderPass,
                                     const MeshId &id, uint32_t octreeLevel) {
  using namespace mg::shaders::voxelizer;

  auto compute = createCompute();

  beginEmptyRenderPass(emptyRenderPass, 1 << octreeLevel, compute.commandBuffer);

  mg::PipelineStateDesc pipelineStateDesc = {};
  pipelineStateDesc.rasterization.vkRenderPass = renderContext.renderPass;
  pipelineStateDesc.rasterization.vkPipelineLayout = mg::vkContext.pipelineLayouts.pipelineLayout;
  pipelineStateDesc.rasterization.graphics.subpass = renderContext.subpass;
  pipelineStateDesc.rasterization.rasterization.cullMode = VK_CULL_MODE_NONE;
  pipelineStateDesc.rasterization.depth.TestEnable = VK_FALSE;
  pipelineStateDesc.rasterization.depth.writeEnable = VK_FALSE;
  pipelineStateDesc.rasterization.multisample.minSampleShading = 0.0f;
  pipelineStateDesc.rasterization.multisample.rasterizationSamples = VK_SAMPLE_COUNT_8_BIT;

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

  ubo->resolution = 1 << octreeLevel;

  VkBuffer storageBuffer;
  uint32_t storageOffset;
  VkDescriptorSet storageSet;
  Voxels *voxels = (Voxels *)mg::mgSystem.linearHeapAllocator.allocateStorage(sizeof(Voxels), &storageBuffer,
                                                                              &storageOffset, &storageSet);
  memset(voxels, 0, sizeof(voxels));


  DescriptorSets descriptorSets = {.ubo = uboSet};

  uint32_t dynamicOffsets[] = {uniformOffset, storageOffset};
  vkCmdBindDescriptorSets(compute.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0,
                          mg::countof(descriptorSets.values), descriptorSets.values, mg::countof(dynamicOffsets),
                          dynamicOffsets);

  const auto mesh = mg::getMesh(id);
  vkCmdBindPipeline(compute.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(compute.commandBuffer, 0, 1, &mesh.buffer, &offset);
  vkCmdDraw(compute.commandBuffer, mesh.indexCount, 1, 0, 0);

  endRenderEmptyRenderPass(compute.commandBuffer);

  vkEndCommandBuffer(compute.commandBuffer);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &compute.commandBuffer;

  checkResult(vkQueueSubmit(vkContext.queue, 1, &submitInfo, compute.fence));
  checkResult(vkWaitForFences(vkContext.device, 1, &compute.fence, VK_TRUE, UINT64_MAX));
  checkResult(vkDeviceWaitIdle(vkContext.device));

  Voxels cpuVoxels;
  memcpy(&cpuVoxels, voxels, sizeof(Voxels));
  std::vector<glm::uvec2> res;
  for (size_t i = 1; i <= cpuVoxels.values[0].x; i++) {
    res.push_back(cpuVoxels.values[i]);
  }
  destroyCompute(&compute);
  return res;
}

} // namespace mg