#include "fluid_scene.h"

#include "mg/camera.h"
#include "mg/meshUtils.h"
#include "mg/mgSystem.h"
#include "mg/texts.h"
#include "mg/window.h"
#include "navier_stroke.h"
#include "navier_stroke_cpu.h"
#include "rendering/rendering.h"
#include "vulkan/singleRenderpass.h"
#include <glm/gtc/matrix_transform.hpp>

static mg::Camera camera;
static mg::SingleRenderPass singleRenderPass;
static mg::MeshId meshId;
static mg::Storages storages;

static void resizeCallback() {
  mg::resizeSingleRenderPass(&singleRenderPass);
  mg::mgSystem.textureContainer.setupDescriptorSets();
}

void initScene() {
  mg::initSingleRenderPass(&singleRenderPass);

  camera = mg::create3DCamera(glm::vec3{0.0f, 0.0f, -5.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
                              glm::vec3{0.0f, 1.0f, 0.0f});

  storages = mg::createStorages(1024);
  mg::mgSystem.textureContainer.setupDescriptorSets();
  mg::vkContext.swapChain->resizeCallack = resizeCallback;
}

void destroyScene() {
  mg::waitForDeviceIdle();
  mg::destroyStorages(&storages);
  destroySingleRenderPass(&singleRenderPass);
}

void updateScene(const mg::FrameData &frameData) {
  if (frameData.keys.r) {
    mg::mgSystem.pipelineContainer.resetPipelineContainer();
  }
  if (frameData.mouse.left)
    mg::handleTools(frameData, &camera);
  mg::setCameraTransformation(&camera);


  mg::fluidTimes.count++;
  if (mg::fluidTimes.count > 100) {
    mg::fluidTimes.count = 0;
    mg::fluidTimes.diffuse = mg::fluidTimes.diffuseSum / 100.0f;
    mg::fluidTimes.advec = mg::fluidTimes.advecSum / 100.0f;
    mg::fluidTimes.project = mg::fluidTimes.projectSum / 100.0f;

    mg::fluidTimes.diffuseSum = 0;
    mg::fluidTimes.advecSum = 0;
    mg::fluidTimes.projectSum = 0;
  }
}

void renderScene(const mg::FrameData &frameData) {
  mg::Texts texts = {};
  char fps[50];
  char pos[50];
  char beh[50];
  char mov[50];

  snprintf(fps, sizeof(fps), "Fps: %u", uint32_t(frameData.fps));
  snprintf(pos, sizeof(pos), "Diffuse cpu: %.3f us", mg::fluidTimes.diffuse);
  snprintf(beh, sizeof(beh), "Project cpu: %.3f us", mg::fluidTimes.project);
  snprintf(mov, sizeof(mov), "Advec cpu: %.3f us", mg::fluidTimes.advec);

  mg::Text text1 = {fps};
  mg::Text text2 = {pos};
  mg::Text text3 = {beh};
  mg::Text text4 = {mov};

  mg::pushText(&texts, text1);
  mg::pushText(&texts, text2);
  mg::pushText(&texts, text3);
  mg::pushText(&texts, text4);

  mg::beginRendering();
  mg::setFullscreenViewport();

  mg::simulateNavierStroke(storages, frameData);
  //mg::simulateNavierStroke2(frameData);

  mg::beginSingleRenderPass(singleRenderPass);
  {
    mg::RenderContext renderContext = {};
    renderContext.renderPass = singleRenderPass.vkRenderPass;

    //mg::renderNavierStroke2(renderContext);
    mg::renderNavierStroke(renderContext, storages);
    mg::validateTexts(texts);
    mg::renderText(renderContext, texts);
  }
  mg::endSingleRenderPass();

  mg::endRendering();
}