#pragma once

#include "RenderUtil.hpp"
#include "Camera.hpp"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"

namespace CADERA_APP_NAMESPACE {

	class CADRender : public pcs::PecosRender {
		
		

		vk::DescriptorPool mGuiDescriptorPool;
		VkAllocationCallbacks* mGuiAllocator;

		struct {

			vk::Pipeline SketchPoint;
			vk::Pipeline SketchLine;
			vk::Pipeline SketchGrid;

		} Pipelines;

	public:

		bool isWait;

		pcs::ubo u;

		cam::Camera Cam;

		sel::Selector Sel;

		void initImgui();

		void imguiRun();

		void createSketchPointPipeline();
		void createSketchLinePipeline();

		void createSketchGridPipeline();

		void preparePipelines();

		void destroyPipelines();

		void createCommandBuffers();

		void updateUniformBuffer(uint32_t currentImage);

		void destroy();

		void runCameraScroll(float yoffset);

		void runCamera();

	};
}

