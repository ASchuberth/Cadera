#pragma once

#include "RenderUtil.hpp"
#include "Camera.hpp"
#include "SketchSolver.hpp"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"
#include <freetype/freetype.h>
#include FT_FREETYPE_H


namespace CADERA_APP_NAMESPACE {


	enum RenderFlags {
		render_update_sketch,
		render_num_flags
	};

	class CADRender : public pcs::PecosRender {
		
		

		vk::DescriptorPool mGuiDescriptorPool;
		VkAllocationCallbacks* mGuiAllocator;

		struct {

			vk::Pipeline SketchPoint;
			vk::Pipeline SketchLine;
			vk::Pipeline SketchGrid;

		} Pipelines;

	public:


		std::bitset<render_num_flags> flags;

		pcs::ubo u;

		cam::Camera Cam;

		sel::Selector Sel;

		sketch::SketchSolver SktSolver;

		void initImgui();

		void imguiRun();

		void createSketchPointPipeline();
		void createSketchLinePipeline();

		void createSketchGridPipeline();

		void preparePipelines();

		void destroyPipelines();

		void createDescriptorSetLayout();

		void createDescriptorPool();

		void createDescriptorSets();

		void createCommandBuffers();

		void updateUniformBuffer(uint32_t currentImage);

		void createTextureImage();

		void destroy();

		void runCamera();

		void render(Model &M);

		void renderSketchPoints(Model &S);

	};
}

