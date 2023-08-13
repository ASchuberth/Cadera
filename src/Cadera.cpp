#include "pch.hpp"
#include "Cadera.hpp"



namespace CADERA_APP_NAMESPACE {
	
	void Cadera::initCallbacks() {
		
		glfwSetWindowUserPointer(Render.mMainWindow, this);
		
		glfwSetMouseButtonCallback(Render.mMainWindow, mouse_button_callback);
		glfwSetScrollCallback(Render.mMainWindow, scroll_callback);
		glfwSetFramebufferSizeCallback(Render.mMainWindow, framebuffer_resize_callback);
		glfwSetCursorPosCallback(Render.mMainWindow, cursor_position_callback);
		glfwSetKeyCallback(Render.mMainWindow, key_callback);

		
	}

	void Cadera::switchCallbacks() {
		
		
		
		if (ImGui::IsWindowHovered()) { 
			glfwSetScrollCallback(Render.mMainWindow, ImGui_ImplGlfw_ScrollCallback);
			glfwSetKeyCallback(Render.mMainWindow, ImGui_ImplGlfw_KeyCallback);
		}
		else {
			glfwSetScrollCallback(Render.mMainWindow, scroll_callback);
			glfwSetKeyCallback(Render.mMainWindow, key_callback);
		}
	}

	Cadera::Cadera() {
		modelIdCounter = 0;
		//Sketch.setType(cad_sketch);
	}

	Cadera::~Cadera() {

		Render.destroy();
	}


	void Cadera::run() {
		


		Render.setBGColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));

		Render.Cam.flags.set(cam::ortho);
		Render.setup();

		initCallbacks();

		Render.initImgui();

		Render.SktSolver.setActiveSketch(&Sketch);



		// Grid Test DELETE
		std::vector<GridRotationAxis> axii = createGridInstanceAxii();

		Vertex p1, p2;
		p1 = { {0.0f, -1000.0f, 0.0f}, {.0f, .0f, .0f} };
		p2 = { {0.0f,  1000.0f, 0.0f}, {.0f, .0f, .0f} };


		std::vector<Vertex> line = { p1, p2 };
		
		Render.createDeviceBuffer(1, line, vk::BufferUsageFlagBits::eVertexBuffer);
		Render.createDeviceBuffer(2, axii, vk::BufferUsageFlagBits::eVertexBuffer);
		// End Grid Test DELETE

		
	
		mainLoop();
	}

	/**
	 * @brief Main loop of the Cadera App
	 * 
	 * 
	 */
	void Cadera::mainLoop() {


		while (!glfwWindowShouldClose(Render.mMainWindow)) {  
			
		 	//switchCallbacks();

			glfwPollEvents();


		 	gui::imguiRun(Sketch, Render, Render.Sel);
			

			if (flags.test(cadera_delete) && !Render.Sel.selectedPoints.empty()) {
				Sketch.deletion(Render.Sel.getSelectedPointIds());
				Render.Sel.clear();
				Render.Sel.setFlags();
				Render.flags.set(render_update_sketch);
				flags.reset(cadera_delete);
			}
			else {
				flags.reset(cadera_delete);
			}

			if (Render.flags.test(render_update_sketch)) {
				Render.render(Sketch);
			}

			
			
		Render.createCommandBuffers();
		Render.drawFrame();
		Render.runCamera();

			glfwWaitEvents();
			


		}
	}

}