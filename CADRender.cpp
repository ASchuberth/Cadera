#include "pch.hpp"
#include "CADRender.hpp"
#include "callbacks.hpp"


#include <stb_image.h>


namespace CADERA_APP_NAMESPACE {
	
	void CADRender::initImgui() {

		// Create Descriptor Pool

		if (mGuiDescriptorPool == VK_NULL_HANDLE)
		{
			vk::DescriptorPoolSize pool_sizes[] =
			{
				{ vk::DescriptorType::eSampler, 1000 },
				{ vk::DescriptorType::eCombinedImageSampler, 1000 },
				{ vk::DescriptorType::eSampledImage, 1000 },
				{ vk::DescriptorType::eStorageImage, 1000 },
				{ vk::DescriptorType::eUniformTexelBuffer, 1000 },
				{ vk::DescriptorType::eStorageTexelBuffer, 1000 },
				{ vk::DescriptorType::eUniformBuffer, 1000 },
				{ vk::DescriptorType::eStorageBuffer, 1000 },
				{ vk::DescriptorType::eUniformBufferDynamic, 1000 },
				{ vk::DescriptorType::eStorageBufferDynamic, 1000 },
				{ vk::DescriptorType::eInputAttachment, 1000 }
			};


			vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1000 * IM_ARRAYSIZE(pool_sizes), 
				                                  (uint32_t)IM_ARRAYSIZE(pool_sizes), pool_sizes);

			mGuiDescriptorPool = mDevice->createDescriptorPool(poolInfo);
		}


		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.WantCaptureMouse = true;



		// Setup GLFW binding
		ImGui_ImplGlfw_InitForVulkan(mMainCanvas.window, true);

		
		// Setup Vulkan binding
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = *mInstance;
		init_info.PhysicalDevice = mPhysicalDevice;
		init_info.Device = *mDevice;
		init_info.QueueFamily = mIndices.graphicsFamily;
		init_info.Queue = mGraphicsQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = mGuiDescriptorPool;
		init_info.Allocator = mGuiAllocator;
		init_info.CheckVkResultFn = VK_NULL_HANDLE;
		init_info.ImageCount = static_cast<uint32_t>(mMainCanvas.mImageViews.size());
		init_info.MinImageCount = static_cast<uint32_t>(mMainCanvas.mImageViews.size());
		ImGui_ImplVulkan_Init(&init_info, mRenderPass, vkGetInstanceProcAddr, vkGetDeviceProcAddr);

		// Setup style
		ImGui::StyleColorsDark();

		// Upload Fonts
		{
			// Use any command queue

			vk::CommandBuffer command_buffer =  beginSingleTimeCommands(vk::CommandBufferLevel::ePrimary, {});


			ImGui_ImplVulkan_CreateFontsTexture(command_buffer);


			endSingleTimeCommands(command_buffer);

			
			ImGui_ImplVulkan_DestroyFontUploadObjects();
			
		}

	}

	void CADRender::imguiRun() {

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		
		ImGui::ShowDemoWindow();


		ImGui::Render();
		

		

	}

	void CADRender::createSketchPointPipeline() {

		vk::VertexInputBindingDescription BindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);


		std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
			BindingDescription
		};

		std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(2);


		AttributeDescriptions[0].binding = 0;
		AttributeDescriptions[0].location = 0;
		AttributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[0].offset = offsetof(Vertex, pos);

		AttributeDescriptions[1].binding = 0;
		AttributeDescriptions[1].location = 1;
		AttributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[1].offset = offsetof(Vertex, col);

		vk::PipelineVertexInputStateCreateInfo VertexInputInfo({}, BindingDescriptions.size(), BindingDescriptions.data(),
			static_cast<uint32_t>(AttributeDescriptions.size()), AttributeDescriptions.data());

		auto vertShaderCode = readFile("C:\\Users\\amsch\\Documents\\Programming\\Cpp\\Cadera\\Cadera\\shaders\\vert.spv");
		auto fragShaderCode = readFile("C:\\Users\\amsch\\Documents\\Programming\\Cpp\\Cadera\\Cadera\\shaders\\frag.spv");

		vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		vk::PipelineShaderStageCreateInfo VertShaderInfo({}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
		vk::PipelineShaderStageCreateInfo FragShaderInfo({}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

		vk::PipelineShaderStageCreateInfo ShaderStages[] = { VertShaderInfo, FragShaderInfo };

		vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo({}, vk::PrimitiveTopology::ePointList, VK_FALSE);

		vk::PipelineRasterizationStateCreateInfo RasterizerInfo({}, VK_FALSE, VK_FALSE, vk::PolygonMode::ePoint,
			vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise,
			VK_FALSE);

		vk::Viewport ViewPort(0.0f, 0.0f, mMainCanvas.mExtent.width, mMainCanvas.mExtent.height, 0.0f, 1.0f);

		vk::Rect2D Scissor({ 0, 0 }, mMainCanvas.mExtent);

		vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1, &Scissor);

		vk::PipelineMultisampleStateCreateInfo MultisampleInfo({}, vk::SampleCountFlagBits::e1, VK_FALSE,
			1.0f, nullptr, VK_FALSE, VK_FALSE);

		vk::PipelineColorBlendAttachmentState ColorBlendAttachment(VK_TRUE, vk::BlendFactor::eSrcAlpha,
			vk::BlendFactor::eOneMinusSrcAlpha,
			vk::BlendOp::eAdd, vk::BlendFactor::eOne,
			vk::BlendFactor::eZero, vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA);


		vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo({}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f });


		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo({}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess,
			VK_TRUE, VK_FALSE, {}, {}, 0.0f, 1.0f);


		vk::GraphicsPipelineCreateInfo PipelineCreateInfo;

		PipelineCreateInfo.stageCount = 2;
		PipelineCreateInfo.pStages = ShaderStages;
		PipelineCreateInfo.pVertexInputState = &VertexInputInfo;
		PipelineCreateInfo.pInputAssemblyState = &InputAssemblyInfo;
		PipelineCreateInfo.pViewportState = &ViewportInfo;
		PipelineCreateInfo.pRasterizationState = &RasterizerInfo;
		PipelineCreateInfo.pMultisampleState = &MultisampleInfo;
		PipelineCreateInfo.pDepthStencilState = &depthStencilInfo;
		PipelineCreateInfo.pColorBlendState = &ColorBlendingInfo;

		PipelineCreateInfo.renderPass = mRenderPass;
		PipelineCreateInfo.subpass = 0;

		PipelineCreateInfo.basePipelineIndex = -1;
		PipelineCreateInfo.layout = mPipelineLayout;


		Pipelines.SketchPoint = mDevice->createGraphicsPipeline(mPipelineCache, PipelineCreateInfo, nullptr).value;


		mDevice->destroyShaderModule(vertShaderModule, nullptr);
		mDevice->destroyShaderModule(fragShaderModule, nullptr);

	}

	void CADRender::createSketchLinePipeline() {
		vk::VertexInputBindingDescription BindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);


		std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
			BindingDescription
		};

		std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(2);


		AttributeDescriptions[0].binding = 0;
		AttributeDescriptions[0].location = 0;
		AttributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[0].offset = offsetof(Vertex, pos);

		AttributeDescriptions[1].binding = 0;
		AttributeDescriptions[1].location = 1;
		AttributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[1].offset = offsetof(Vertex, col);

		vk::PipelineVertexInputStateCreateInfo VertexInputInfo({}, BindingDescriptions.size(), BindingDescriptions.data(),
			static_cast<uint32_t>(AttributeDescriptions.size()), AttributeDescriptions.data());

		auto vertShaderCode = readFile("C:\\Users\\amsch\\Documents\\Programming\\Cpp\\Cadera\\Cadera\\shaders\\vert.spv");
		auto fragShaderCode = readFile("C:\\Users\\amsch\\Documents\\Programming\\Cpp\\Cadera\\Cadera\\shaders\\frag.spv");

		vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		vk::PipelineShaderStageCreateInfo VertShaderInfo({}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
		vk::PipelineShaderStageCreateInfo FragShaderInfo({}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

		vk::PipelineShaderStageCreateInfo ShaderStages[] = { VertShaderInfo, FragShaderInfo };

		vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo({}, vk::PrimitiveTopology::eLineList, VK_FALSE);

		vk::PipelineRasterizationStateCreateInfo RasterizerInfo({}, VK_FALSE, VK_FALSE, vk::PolygonMode::eLine,
			                                                    vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise,
			                                                    VK_FALSE, {}, {}, {}, 1.0f);

		vk::Viewport ViewPort(0.0f, 0.0f, mMainCanvas.mExtent.width, mMainCanvas.mExtent.height, 0.0f, 1.0f);

		vk::Rect2D Scissor({ 0, 0 }, mMainCanvas.mExtent);

		vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1, &Scissor);

		vk::PipelineMultisampleStateCreateInfo MultisampleInfo({}, vk::SampleCountFlagBits::e1, VK_FALSE,
			1.0f, nullptr, VK_FALSE, VK_FALSE);

		vk::PipelineColorBlendAttachmentState ColorBlendAttachment(VK_TRUE, vk::BlendFactor::eSrcAlpha,
			vk::BlendFactor::eOneMinusSrcAlpha,
			vk::BlendOp::eAdd, vk::BlendFactor::eOne,
			vk::BlendFactor::eZero, vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA);


		vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo({}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f });


		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo({}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess,
			VK_TRUE, VK_FALSE, {}, {}, 0.0f, 1.0f);


		vk::GraphicsPipelineCreateInfo PipelineCreateInfo;

		PipelineCreateInfo.stageCount = 2;
		PipelineCreateInfo.pStages = ShaderStages;
		PipelineCreateInfo.pVertexInputState = &VertexInputInfo;
		PipelineCreateInfo.pInputAssemblyState = &InputAssemblyInfo;
		PipelineCreateInfo.pViewportState = &ViewportInfo;
		PipelineCreateInfo.pRasterizationState = &RasterizerInfo;
		PipelineCreateInfo.pMultisampleState = &MultisampleInfo;
		PipelineCreateInfo.pDepthStencilState = &depthStencilInfo;
		PipelineCreateInfo.pColorBlendState = &ColorBlendingInfo;

		PipelineCreateInfo.renderPass = mRenderPass;
		PipelineCreateInfo.subpass = 0;

		PipelineCreateInfo.basePipelineIndex = -1;
		PipelineCreateInfo.layout = mPipelineLayout;


		Pipelines.SketchLine = mDevice->createGraphicsPipeline(mPipelineCache, PipelineCreateInfo, nullptr).value;


		mDevice->destroyShaderModule(vertShaderModule, nullptr);
		mDevice->destroyShaderModule(fragShaderModule, nullptr);
	}

	void CADRender::createSketchGridPipeline() {

		vk::VertexInputBindingDescription BindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
		vk::VertexInputBindingDescription BindingDescriptionInstance(1, sizeof(GridRotationAxis), 
			                                                         vk::VertexInputRate::eInstance);


		std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
			BindingDescription,
			BindingDescriptionInstance
		};

		std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(5);


		AttributeDescriptions[0].binding = 0;
		AttributeDescriptions[0].location = 0;
		AttributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[0].offset = offsetof(Vertex, pos);

		AttributeDescriptions[1].binding = 0;
		AttributeDescriptions[1].location = 1;
		AttributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[1].offset = offsetof(Vertex, col);

		AttributeDescriptions[2].binding = 1;
		AttributeDescriptions[2].location = 2;
		AttributeDescriptions[2].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[2].offset = offsetof(GridRotationAxis, pos);

		AttributeDescriptions[3].binding = 1;
		AttributeDescriptions[3].location = 3;
		AttributeDescriptions[3].format = vk::Format::eR32G32B32Sfloat;
		AttributeDescriptions[3].offset = offsetof(GridRotationAxis, axis);

		AttributeDescriptions[4].binding = 1;
		AttributeDescriptions[4].location = 4;
		AttributeDescriptions[4].format = vk::Format::eR32Sfloat;
		AttributeDescriptions[4].offset = offsetof(GridRotationAxis, angle);
		  
		vk::PipelineVertexInputStateCreateInfo VertexInputInfo({}, BindingDescriptions.size(), BindingDescriptions.data(),
			static_cast<uint32_t>(AttributeDescriptions.size()), AttributeDescriptions.data());

		auto vertShaderCode = readFile("C:\\Users\\amsch\\Documents\\Programming\\Cpp\\Cadera\\Cadera\\shaders\\gridvert.spv");
		auto fragShaderCode = readFile("C:\\Users\\amsch\\Documents\\Programming\\Cpp\\Cadera\\Cadera\\shaders\\frag.spv");

		vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		vk::PipelineShaderStageCreateInfo VertShaderInfo({}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
		vk::PipelineShaderStageCreateInfo FragShaderInfo({}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

		vk::PipelineShaderStageCreateInfo ShaderStages[] = { VertShaderInfo, FragShaderInfo };

		vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo({}, vk::PrimitiveTopology::eLineList, VK_FALSE);

		vk::PipelineRasterizationStateCreateInfo RasterizerInfo({}, VK_FALSE, VK_FALSE, vk::PolygonMode::eLine,
			vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise,
			VK_FALSE, {}, {}, {}, 1.0f);

		vk::Viewport ViewPort(0.0f, 0.0f, mMainCanvas.mExtent.width, mMainCanvas.mExtent.height, 0.0f, 1.0f);

		vk::Rect2D Scissor({ 0, 0 }, mMainCanvas.mExtent);

		vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1, &Scissor);

		vk::PipelineMultisampleStateCreateInfo MultisampleInfo({}, vk::SampleCountFlagBits::e1, VK_FALSE,
			1.0f, nullptr, VK_FALSE, VK_FALSE);

		vk::PipelineColorBlendAttachmentState ColorBlendAttachment(VK_TRUE, vk::BlendFactor::eSrcAlpha,
			vk::BlendFactor::eOneMinusSrcAlpha,
			vk::BlendOp::eAdd, vk::BlendFactor::eOne,
			vk::BlendFactor::eZero, vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA);


		vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo({}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f });


		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo({}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess,
			VK_TRUE, VK_FALSE, {}, {}, 0.0f, 1.0f);


		vk::GraphicsPipelineCreateInfo PipelineCreateInfo;

		PipelineCreateInfo.stageCount = 2;
		PipelineCreateInfo.pStages = ShaderStages;
		PipelineCreateInfo.pVertexInputState = &VertexInputInfo;
		PipelineCreateInfo.pInputAssemblyState = &InputAssemblyInfo;
		PipelineCreateInfo.pViewportState = &ViewportInfo;
		PipelineCreateInfo.pRasterizationState = &RasterizerInfo;
		PipelineCreateInfo.pMultisampleState = &MultisampleInfo;
		PipelineCreateInfo.pDepthStencilState = &depthStencilInfo;
		PipelineCreateInfo.pColorBlendState = &ColorBlendingInfo;

		PipelineCreateInfo.renderPass = mRenderPass;
		PipelineCreateInfo.subpass = 0;

		PipelineCreateInfo.basePipelineIndex = -1;
		PipelineCreateInfo.layout = mPipelineLayout;


		Pipelines.SketchGrid = mDevice->createGraphicsPipeline({}, PipelineCreateInfo, nullptr).value;


		mDevice->destroyShaderModule(vertShaderModule, nullptr);
		mDevice->destroyShaderModule(fragShaderModule, nullptr);
	}

	void CADRender::preparePipelines() {

		createSketchPointPipeline();
		createSketchLinePipeline();
		createSketchGridPipeline();
	}

	void CADRender::destroyPipelines() {

	
		mDevice->destroyPipeline(Pipelines.SketchPoint);
		mDevice->destroyPipeline(Pipelines.SketchLine);
		mDevice->destroyPipeline(Pipelines.SketchGrid);

	}

	void CADRender::createDescriptorSetLayout() {


		vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1,
			vk::ShaderStageFlagBits::eVertex, nullptr);

		vk::DescriptorSetLayoutBinding cisLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1,
			vk::ShaderStageFlagBits::eFragment, nullptr);

		std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, cisLayoutBinding };


		vk::DescriptorSetLayoutCreateInfo layoutInfo({}, static_cast<uint32_t>(bindings.size()), bindings.data());

		if (mDevice->createDescriptorSetLayout(&layoutInfo, nullptr, &mDescriptorSetLayout) != vk::Result::eSuccess)
			throw std::runtime_error("failed to create descriptor set layout!");

	}

	void CADRender::createDescriptorPool() {

		std::array<vk::DescriptorPoolSize, 2> poolSizes = {};
		poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(mMainCanvas.mImages.size());
		poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(mMainCanvas.mImages.size());

		vk::DescriptorPoolCreateInfo poolInfo({}, static_cast<uint32_t>(mMainCanvas.mImages.size()),
			static_cast<uint32_t>(poolSizes.size()), poolSizes.data());

		mDescriptorPool = mDevice->createDescriptorPool(poolInfo, nullptr);

	}

	void CADRender::createDescriptorSets() {

		
		std::vector<vk::DescriptorSetLayout> layouts(mMainCanvas.mImages.size(), mDescriptorSetLayout);

		vk::DescriptorSetAllocateInfo allocInfo(mDescriptorPool, static_cast<uint32_t>(mMainCanvas.mImages.size()), layouts.data());

		mDescriptorSets.resize(mMainCanvas.mImages.size());


		mDescriptorSets = mDevice->allocateDescriptorSets(allocInfo);


		// Uniform Buffer

		for (size_t i = 0; i < mMainCanvas.mImages.size(); i++) {


			vk::DescriptorBufferInfo bufferInfo(mUniformBuffers[i], 0, sizeof(u));

			vk::DescriptorImageInfo imageInfo(mTextureSampler, mTextureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);

			std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {};


			descriptorWrites[0].dstSet = mDescriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].dstSet = mDescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			mDevice->updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		}



	}

	void CADRender::createCommandBuffers() {

		mDevice->resetCommandPool(mCommandPool, vk::CommandPoolResetFlags());

		for (size_t i = 0; i < mCommandBuffers.size(); i++) {

			vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

			mCommandBuffers[i].begin(beginInfo);

			vk::Rect2D renderArea({ 0,0 }, { mMainCanvas.mExtent.width, mMainCanvas.mExtent.height });

			std::array<float, 4> color = { bgColor.x, bgColor.y, bgColor.z, bgColor.w };

			std::array<vk::ClearValue, 2> clearValues{};
			clearValues[0].setColor(color);
			clearValues[1].depthStencil.depth = 1.0f;
			clearValues[1].depthStencil.stencil = 0;

			vk::RenderPassBeginInfo renderPassInfo(mRenderPass, mFramebuffers[i], renderArea,
				static_cast<uint32_t>(clearValues.size()), clearValues.data());

			mCommandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

			vk::DeviceSize offsets[] = { 0 };

			mCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout, 0, 1, &mDescriptorSets[i], 0, nullptr);


			// Selection Points
			if (!mBuffers[BUF_SELECTION_POINTS].isEmpty) {
				mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, Pipelines.SketchPoint);
				mCommandBuffers[i].bindVertexBuffers(0, 1, &mBuffers[BUF_SELECTION_POINTS].mBuffer, offsets);
				mCommandBuffers[i].draw(mBuffers[BUF_SELECTION_POINTS].mPointSize, 1, 0, 0);
			}

			// Sketch Points
			if (!mBuffers[BUF_SKETCH_POINTS].isEmpty) {
				mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, Pipelines.SketchPoint);
				mCommandBuffers[i].bindVertexBuffers(0, 1, &mBuffers[BUF_SKETCH_POINTS].mBuffer, offsets);
				mCommandBuffers[i].draw(mBuffers[BUF_SKETCH_POINTS].mPointSize, 1, 0, 0);
			}

			// Grid
			mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, Pipelines.SketchGrid);
			mCommandBuffers[i].bindVertexBuffers(0, 1, &mBuffers[1].mBuffer, offsets);
			mCommandBuffers[i].bindVertexBuffers(1, 1, &mBuffers[2].mBuffer, offsets);
			mCommandBuffers[i].draw(2, 4002, 0, 0);

			// Text
			if (!mBuffers[BUF_TEXT_VERTICES].isEmpty) {
				mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, mTextPipeline);
				mCommandBuffers[i].bindVertexBuffers(0, 1, &mBuffers[BUF_TEXT_VERTICES].mBuffer, offsets);
				mCommandBuffers[i].bindIndexBuffer(mBuffers[BUF_TEXT_INDICES].mBuffer, 0, vk::IndexType::eUint32);
				mCommandBuffers[i].drawIndexed(mBuffers[BUF_TEXT_INDICES].mPointSize, 1, 0, 0, 0);
			}




			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), mCommandBuffers[i]);

			mCommandBuffers[i].endRenderPass();

			mCommandBuffers[i].end();


		}


	}

	void CADRender::updateUniformBuffer(uint32_t currentImage) {

		u.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		u.view = glm::lookAt(Cam.pos, Cam.focus, glm::vec3(0.0f, 1.0f, 0.0f));
		
		
		if (Cam.flags.test(cam::ortho)) {

			float screenRatio = (float)mMainCanvas.mExtent.height / (float)mMainCanvas.mExtent.width;
			
			u.proj = glm::ortho(Cam.left, -Cam.left, screenRatio * Cam.left, -screenRatio * Cam.left, 0.0f, 500.0f);
		}
		else {
			u.proj = glm::perspective(glm::radians(45.0f), mMainCanvas.mExtent.width / (float)mMainCanvas.mExtent.height, 
				                      0.001f, 100.0f);
		}

		u.proj[1][1] *= -1;


		vk::MemoryMapFlags memMapFlags;

		void* data;
		data = mDevice->mapMemory(mUniformBufferMemories[currentImage], 0, sizeof(u), memMapFlags);
		memcpy(data, &u, sizeof(u));
		mDevice->unmapMemory(mUniformBufferMemories[currentImage]);

	}

	void CADRender::destroy() {

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		mDevice->destroyDescriptorPool(mGuiDescriptorPool);
	

		destroyPipelines();

		cleanup();

	}

	void CADRender::runCamera() {
		
		
		double x, y;
		glfwGetCursorPos(mMainCanvas.window, &x, &y);
		Cam.updateMouseRay(static_cast<float>(x), static_cast<float>(y), u.model, u.view, u.proj,
						   mMainCanvas.mExtent.width, mMainCanvas.mExtent.height
		);

		if (Cam.flags.test(cam::pan)) {
			glfwGetCursorPos(mMainCanvas.window, &Cam.xpos, &Cam.ypos);
			Cam.pan({ 0.0f, 0.0f, 0.0f }, glm::normalize(Cam.pos - Cam.focus));
		}

		Cam.update();
	}

	void CADRender::render(Model &M) {

		
		if (M.getType() == cad_sketch) {
			renderSketchPoints(M);
			renderSketchNotes(M);
		}

		flags.reset(render_update_sketch);
	}

	void CADRender::renderSketchNotes(Model& S) {
		
		TxtRend.clearTexts();

		for (const auto& N : S.Notes) {
			TxtRend.addText(N.second);
		}

		// Relation Symbols
		std::vector<pcs::txt::Text> RelationTexts;
		RelationTexts = S.getRelationTexts();

		for (const auto& T : RelationTexts) {
			TxtRend.addText(T);
		}

		std::vector<pcs::txt::Vertex> txtVertices = TxtRend.generateQuads();
		std::vector<uint32_t> txtIndices = TxtRend.generateIndices();

		if (!txtVertices.empty()) {
			updateBuffer(BUF_TEXT_VERTICES, txtVertices, vk::BufferUsageFlagBits::eVertexBuffer);
			updateBuffer(BUF_TEXT_INDICES, txtIndices, vk::BufferUsageFlagBits::eIndexBuffer);
		}
		else if (!mBuffers[BUF_TEXT_VERTICES].isEmpty) {
			deleteBuffer(BUF_TEXT_VERTICES);
			deleteBuffer(BUF_TEXT_INDICES);
		}

	}

	void CADRender::renderSketchPoints(Model &S) {


		std::vector<glm::vec3> pointColors;
		std::vector<glm::vec3> pointVertices = S.getVertices(pointColors);
		std::vector<glm::vec3> selPointVertices = Sel.getVertices();
		
		

		std::vector<Vertex> Vertices;

		if (!selPointVertices.empty()) {
			for (const auto& v : selPointVertices) {

				Vertices.push_back({ v, {0.0f, 1.0f, 0.0f} });

			}
		}

		if (!pointVertices.empty() && !pointColors.empty()) {
			for (size_t i = 0; i < pointVertices.size(); i++) {

				Vertices.push_back({ pointVertices[i], pointColors[i] });

			}
		}

	
		if (!Vertices.empty()) {
			updateBuffer(BUF_SKETCH_POINTS, Vertices, vk::BufferUsageFlagBits::eVertexBuffer);
		}
		else if (!mBuffers[BUF_SKETCH_POINTS].isEmpty){
			deleteBuffer(BUF_SKETCH_POINTS);
		}

	}

	
}