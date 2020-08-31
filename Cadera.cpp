#include "pch.hpp"
#include "Cadera.hpp"

namespace CADERA_APP_NAMESPACE {

	Cadera::Cadera() {
		
	}

	Cadera::~Cadera() {

		Render.destroy();
	}

	void Cadera::run() {
		
		Render.setup();
		Render.preparePipelines();

		Render.initImgui();

		Render.createDeviceBuffer(0, ptVertices, vk::BufferUsageFlagBits::eVertexBuffer);

		mainLoop();
	}

	void Cadera::mainLoop() {

		while (!glfwWindowShouldClose(Render.mMainCanvas.window)) {
			
			

			Render.imguiRun();
			Render.createCommandBuffers();


			Render.drawFrame();

			glfwPollEvents();
		}


	}

}