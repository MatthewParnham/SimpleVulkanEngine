#include "first_app.hpp"

#include "lve_camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"

#include <stdexcept>
#include <array>
#include <chrono>
#include <cassert>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

	struct GlobalUbo {
		glm::mat4 projectionView{1.f};
		glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f,-3.f,-1.f});
	};

	FirstApp::FirstApp() {
		loadGameObjects();
	}
	FirstApp::~FirstApp() {}

	void FirstApp::run() {

		LveBuffer globalUboBuffer{
			lveDevice,
			sizeof(GlobalUbo),
			LveSwapChain::MAX_FRAMES_IN_FLIGHT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			lveDevice.properties.limits.minUniformBufferOffsetAlignment,
		};
		globalUboBuffer.map();

		SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
		LveCamera camera{};
		camera.setViewTarget(glm::vec3(-1.f,-2.f,2.f), glm::vec3(0.f,0.f,1.5f));

		auto viewerObject = LveGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();
		
		while (!lveWindow.shouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = lveRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
			
			if(auto commandBuffer = lveRenderer.beginFrame()) {
				int frameIndex = lveRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera
				};
				//update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				globalUboBuffer.writeToIndex(&ubo, frameIndex);
				globalUboBuffer.flushIndex(frameIndex);

				//render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo,gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(lveDevice.device());
	}

	

	void FirstApp::loadGameObjects() {
		std::shared_ptr<LveModel> lveModel =
			LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");
		auto flatVase = LveGameObject::createGameObject();
		flatVase.model = lveModel;
		flatVase.transform.translation = {-.5f, .5f, 2.5f};
		flatVase.transform.scale = {3.f, 1.5f, 3.f};
		gameObjects.push_back(std::move(flatVase));

		lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
		auto smoothVase = LveGameObject::createGameObject();
		smoothVase.model = lveModel;
		smoothVase.transform.translation = {.5f, .5f, 2.5f};
		smoothVase.transform.scale = {3.f, 1.5f, 3.f};
		gameObjects.push_back(std::move(smoothVase));

		lveModel = LveModel::createModelFromFile(lveDevice, "models/stanford-dragon.obj");
		auto dragon = LveGameObject::createGameObject();
		dragon.model = lveModel;
		dragon.transform.translation = {1.5f, .5f, 2.5f};
		dragon.transform.scale = {.05f,.05f,.05f};
		gameObjects.push_back(std::move(dragon));
	}
}

