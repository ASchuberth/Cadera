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

	Cadera::Cadera() {
		modelIdCounter = 0;
	}

	Cadera::~Cadera() {

		Render.destroy();
	}

	void Cadera::runSelection() {

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

	}

	void Cadera::run() {
		


		Render.setBGColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));

		Render.Cam.flags.set(cam::ortho);
		Render.setup();

		initCallbacks();

		Render.initImgui();

		Render.SktSolver.setActiveSketch(&Sketch);

		
		mainLoop();
	}

	/**
	 * @brief Main loop of the Cadera App
	 * 
	 * 
	 */
	void Cadera::mainLoop() {


		while (!glfwWindowShouldClose(Render.mMainWindow)) {  
			

			glfwPollEvents();


		 	gui::imguiRun(Sketch, Render, Render.Sel);
			

			runSelection();



			
			
			Render.createCommandBuffers();
			Render.drawFrame();
			Render.runCamera();

			glfwWaitEvents();
			


		}
	}

}