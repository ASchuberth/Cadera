#include "pch.hpp"
#include "Cadera.hpp"
#include "gui.hpp"


namespace CADERA_APP_NAMESPACE {
	
	void Cadera::initCallbacks() {
		
		glfwSetWindowUserPointer(Render.mMainCanvas.window, &Render);
		
		glfwSetMouseButtonCallback(Render.mMainCanvas.window, mouse_button_callback);
		glfwSetScrollCallback(Render.mMainCanvas.window, scroll_callback);
		glfwSetFramebufferSizeCallback(Render.mMainCanvas.window, framebuffer_resize_callback);
		glfwSetCursorPosCallback(Render.mMainCanvas.window, cursor_position_callback);
	}

	

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

		// Grid Test DELETE
		std::vector<GridRotationAxis> axii = createGridInstanceAxii();

		Vertex p1, p2;
		p1 = { {0.0f, -100.0f, 0.0f}, {.5f, .5f, .5f} };
		p2 = { {0.0f,  100.0f, 0.0f}, {.5f, .5f, .5f} };

		std::vector<Vertex> line = { p1, p2 };
		 
		Render.createDeviceBuffer(1, line, vk::BufferUsageFlagBits::eVertexBuffer);
		Render.createDeviceBuffer(2, axii, vk::BufferUsageFlagBits::eVertexBuffer);
		// End Grid Test DELETE

		initCallbacks();

		mainLoop();
	}

	void Cadera::mainLoop() {

		while (!glfwWindowShouldClose(Render.mMainCanvas.window)) {
			
			glfwPollEvents();

			Render.runCamera();

			gui::imguiRun(Render);
			Render.createCommandBuffers();


			Render.drawFrame();

		
		}


	}

	
}