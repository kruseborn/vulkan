#include "octree_scene.h"

#include "build_octree.h"
#include "empty_render_pass.h"
#include "mg/camera.h"
#include "mg/meshLoader.h"
#include "mg/meshUtils.h"
#include "mg/mgSystem.h"
#include "mg/texts.h"
#include "mg/window.h"
#include "octree_rendering.h"
#include "rendering/rendering.h"
#include "sdf/sdf.h"
#include "voxelizer.h"
#include "vulkan/singleRenderpass.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

// voxilization and octree building
// https://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-SparseVoxelization.pdf
// rendering
// https://research.nvidia.com/publication/efficient-sparse-voxel-octrees
// opengl implementation
// https://github.com/AdamYuan/SparseVoxelOctree
// sdf 
// https://github.com/christopherbatty/SDFGen

static mg::Camera camera;
static mg::SingleRenderPass singleRenderPass;
static mg::EmptyRenderPass emptyRenderPass;
static mg::MeshId cubeId;
static mg::MeshId sphereId;
static mg::TextureId sdfId;
static mg::OctreeStorages storages;
static uint32_t octreeLevel = 4;
static mg::Menu menu = {};

static void resizeCallback() {
  mg::resizeSingleRenderPass(&singleRenderPass);
  mg::mgSystem.textureContainer.setupDescriptorSets();
}

static mg::TextureId uploadVolumeToGpu(const float *data, const glm::uvec3 &size) {
  mg::CreateTextureInfo createTextureInfo = {};
  createTextureInfo.id = "sphere";
  createTextureInfo.type = mg::TEXTURE_TYPE::TEXTURE_3D;
  createTextureInfo.size = {size.x, size.y, size.z};
  createTextureInfo.data = (void *)data;
  createTextureInfo.sizeInBytes = size.x * size.y * size.z * sizeof(float);
  createTextureInfo.format = VK_FORMAT_R32_SFLOAT;

  return mg::mgSystem.textureContainer.createTexture(createTextureInfo);
}

static mg::MeshId uploadCubeToGpu() {
  const auto cube = mg::createVolumeCube({-1.0f, -1.0f, -1.0f}, {2, 2, 2});

  mg::CreateMeshInfo createMeshInfo = {.id = "box",
                                       .vertices = (uint8_t *)cube.vertices,
                                       .indices = (uint8_t *)cube.indices,
                                       .verticesSizeInBytes = mg::sizeofArrayInBytes(cube.vertices),
                                       .indicesSizeInBytes = mg::sizeofArrayInBytes(cube.indices),
                                       .nrOfIndices = mg::countof(cube.indices)};

  return mg::mgSystem.meshContainer.createMesh(createMeshInfo);
}

void initScene() {
  mg::initSingleRenderPass(&singleRenderPass);
  mg::initEmptyRenderPass(&emptyRenderPass, 1 << octreeLevel);

  camera = mg::create3DCamera(glm::vec3{0, 0, -3}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

  cubeId = uploadCubeToGpu();
  sphereId = mg::loadObjFromFile(mg::getDataPath() + "sphereTriangles.obj").meshes.front().id;

  auto sdf = objToSdf(mg::getDataPath() + "sphereTriangles.obj", 0.1f, 2);
  sdfId = uploadVolumeToGpu((float *)(sdf.grid.data()), glm::uvec3{sdf.x, sdf.y, sdf.z});

  mg::mgSystem.textureContainer.setupDescriptorSets();
  mg::vkContext.swapChain->resizeCallack = resizeCallback;
}

void destroyScene() {
  mg::waitForDeviceIdle();
  destroySingleRenderPass(&singleRenderPass);
  mg::destroyEmptyRenderPass(&emptyRenderPass);
  mg::destroyOctreeStorages(&storages);
}

void updateScene(const mg::FrameData &frameData) {
  if (frameData.keys.r) {
    mg::mgSystem.pipelineContainer.resetPipelineContainer();
  }
  if (frameData.mouse.left)
    mg::handleTools(frameData, &camera);
  mg::setCameraTransformation(&camera);
}

void renderScene(const mg::FrameData &frameData) {
  mg::Texts texts = {};
  char fps[50];
  snprintf(fps, sizeof(fps), "Fps: %u", uint32_t(frameData.fps));
  mg::pushText(&texts, {fps});

  // voxilization using empty renderpass
  mg::RenderContext renderContext1 = {.renderPass = emptyRenderPass.vkRenderPass};
  auto voxels = mg::voxelizeMesh(renderContext1, emptyRenderPass, sphereId, octreeLevel);

  // main renderpass
  mg::beginRendering();
  mg::setFullscreenViewport();

  static bool firstFrame = true;
  if (firstFrame) {
    mg::initOctreeStorages(&storages, voxels);
    firstFrame = false;
  }

  mg::buildOctree(storages, octreeLevel, frameData, uint32_t(voxels.size()));

  mg::beginSingleRenderPass(singleRenderPass);
  {
    mg::RenderContext renderContext = {};
    renderContext.renderPass = singleRenderPass.vkRenderPass;

    renderContext.projection = glm::perspective(
        glm::radians(camera.fov), mg::vkContext.screen.width / float(mg::vkContext.screen.height), 0.1f, 1000.f);
    renderContext.view = glm::lookAt(camera.position, camera.aim, camera.up);

    renderContext.renderPass = singleRenderPass.vkRenderPass;

    switch (menu.value) {
    case mg::Menu::Octree:
      mg::renderOctree(storages, renderContext, octreeLevel, frameData, camera.position);
      break;
    case mg::Menu::sdf:
      mg::renderSdf(renderContext, {}, frameData, camera);
      break;
    case mg::Menu::cubes:
      mg::renderVoxels(renderContext, voxels, cubeId);
      break;
    };

    mg::renderText(renderContext, texts);
    mg::mgSystem.imguiOverlay.draw(renderContext, frameData, mg::renderMenu, &menu);
  }
  mg::endSingleRenderPass();

  mg::endRendering();
}