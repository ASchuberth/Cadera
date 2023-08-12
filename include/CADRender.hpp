#pragma once

//#include "RenderUtil.hpp"
//#include "Camera.hpp"
//#include "SketchSolver.hpp"
//#include "Canvas.hpp"
//#include "TextRender.hpp"
//#include "RenderUtil.hpp"




namespace CADERA_APP_NAMESPACE {


		struct SwapChainSupportDetails {
			vk::SurfaceCapabilitiesKHR capabilities;
			std::vector<vk::SurfaceFormatKHR> formats;
			std::vector<vk::PresentModeKHR> presentModes;
		};

		struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

		bool isDifferent() {
			return graphicsFamily != presentFamily;
		}
	};



	struct ubo {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};


	struct Buffer {


		vk::Buffer mBuffer;
		vk::DeviceMemory mMemory;
		vk::DeviceSize mDeviceSize;
		uint32_t mPointSize;
		bool isEmpty = true;

	};


	enum RenderFlags {
		render_update_sketch,
		render_num_flags
	};

	class CADRender  {
		
	private:
		std::vector<const char*> validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		vk::DescriptorPool mGuiDescriptorPool;
		VkAllocationCallbacks* mGuiAllocator; 

		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
		PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = 0;
		PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTag = 0;
		PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectName = 0;
		PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBegin = 0;
		PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEnd = 0;
		PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsert = 0;

		struct {

			vk::Pipeline SketchPoint;
			vk::Pipeline SketchLine;
			vk::Pipeline SketchGrid;

		} Pipelines; 

	

	public:

		

		// GLFW
		GLFWwindow* mMainWindow;



		// Main Vulkan Objects
		vk::Instance mInstance;
		vk::PhysicalDevice mPhysicalDevice;
		vk::Device mDevice;
		vk::RenderPass mRenderPass;

		// Secondary Vulkan Objects

		// mInstance
		vk::DynamicLoader dl;
		vk::DispatchLoaderDynamic dldy;
		VkDebugUtilsMessengerEXT mDebug;
		vk::ApplicationInfo mAppInfo;
		bool enableValidationLayers;
		vk::DebugUtilsMessengerEXT mCallback;
		vk::SurfaceKHR mSurface;


		// Physical mDevice
		QueueFamilyIndices mIndices;

		// Logical mDevice
		vk::Queue mGraphicsQueue;
		vk::Queue mPresentQueue;



		// Pipeline
		vk::PipelineCache mPipelineCache;

		vk::PipelineLayout mPipelineLayout;

		vk::Pipeline mTextPipeline;

		// Buffers
		vk::CommandPool mCommandPool;
		std::vector<vk::CommandBuffer> mCommandBuffers;
		std::vector<vk::Framebuffer> mFramebuffers;

		std::vector<vk::Buffer> mUniformBuffers;
		std::vector<vk::DeviceMemory> mUniformBufferMemories;

		vk::DescriptorPool mDescriptorPool;
		std::vector<vk::DescriptorSet> mDescriptorSets;

		std::map<uint32_t, Buffer> mBuffers;

		// Textures
		vk::Image mTextureImage;
		vk::DeviceMemory mTextureMemory;
		vk::ImageView mTextureImageView;
		vk::Sampler mTextureSampler;


		// Sync Objects
		std::vector<vk::Semaphore> mImageAvailableSemaphores;
		std::vector<vk::Semaphore> mRenderFinishedSemaphores;
		std::vector<vk::Fence> mInFlightFences;
		size_t mCurrentFrame = 0;

		// Options
		glm::vec4 bgColor;



		vk::DescriptorSetLayout mDescriptorSetLayout;


		std::bitset<render_num_flags> flags;

		ubo u;

		//cam::Camera Cam;

		//sel::Selector Sel;

		//sketch::SketchSolver SktSolver;

		//txt::TextRender TxtRend;

		// Canvas
		//Canvas mMainCanvas;

		void setup();
		
		
		//-------------------------------------------


		// Functions

		void setBGColor(glm::vec4 color);

		// GLFW

		void createWindow();

		// Instance

		void createInstance();

		void createSurface();

		// Physical Device

		void pickPhysicalDevice();

        SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &device);

        QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface);

        bool isDeviceSuitable(vk::PhysicalDevice device);

        bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

		// Logical mDevice
		void createLogicalDevice();
/*		

		// Graphics Pipeline
		vk::Format findSupportedFormat(vk::PhysicalDevice& PhysicalDevice, const std::vector<vk::Format>& candidates,
		vk::ImageTiling tiling, vk::FormatFeatureFlagBits features);

		vk::Format findDepthFormat(vk::PhysicalDevice& PhysicalDevice);

		void createRenderPass();

		virtual void createDescriptorSetLayout();

		virtual void createPipelineLayout();

		std::vector<char> readFile(const std::string filename);

		void createTextPipeline();


		// Swapchain
		void cleanupSwapchain();

		void recreateSwapchain();

		// Shaders
		//static std::vector<char> readFile(const std::string filename);

		vk::ShaderModule createShaderModule(const std::vector<char>& code);

		// Buffers
		void createCommandPool();

		vk::CommandBuffer beginSingleTimeCommands(const vk::CommandBufferLevel& level,
			const vk::CommandBufferInheritanceInfo& inheritance);

		void endSingleTimeCommands(vk::CommandBuffer& commandBuffer);

		void createFramebuffers();

		void createUniformBuffer();

		void allocCommandBuffers();

		void transitionImageLayout(vk::Image& image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

		void copyBufferToImage(vk::Buffer& buffer, vk::Image& image, uint32_t width, uint32_t height);

		virtual void createTextureImage();

		void createTextureImageView();

		void createTextureSampler();

		void createSyncObjects();

		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

		void createBuffer(vk::DeviceSize& size, const vk::BufferUsageFlags& usage, const vk::MemoryPropertyFlags& properties,
			vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);

		template <class T>
		inline void createDeviceBuffer(uint32_t id, std::vector<T> const& points, vk::BufferUsageFlagBits const& flag) {

			vk::Buffer stagingBuffer;
			vk::DeviceMemory stagingBufferMemory;

			mBuffers[id].mPointSize = static_cast<uint32_t>(points.size());
			mBuffers[id].mDeviceSize = sizeof(points[0]) * points.size();

			createBuffer(mBuffers[id].mDeviceSize, vk::BufferUsageFlagBits::eTransferSrc,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
				stagingBuffer, stagingBufferMemory);

			vk::MemoryMapFlags memMapFlags;

			void* data;
			data = mDevice.mapMemory(stagingBufferMemory, 0, mBuffers[id].mDeviceSize, memMapFlags);
			memcpy(data, points.data(), (size_t)mBuffers[id].mDeviceSize);
			mDevice.unmapMemory(stagingBufferMemory);

			createBuffer(mBuffers[id].mDeviceSize, vk::BufferUsageFlagBits::eTransferDst | flag,
				vk::MemoryPropertyFlagBits::eDeviceLocal,
				mBuffers[id].mBuffer, mBuffers[id].mMemory);


			copyBuffer(stagingBuffer, mBuffers[id].mBuffer, mBuffers[id].mDeviceSize);

			mDevice.destroyBuffer(stagingBuffer);
			mDevice.freeMemory(stagingBufferMemory);

			mBuffers[id].isEmpty = false;

		};

		template <class T>
		inline void updateBuffer(uint32_t id, std::vector<T> const& points, vk::BufferUsageFlagBits const& flag) {
			if (mBuffers[id].isEmpty) {
				createDeviceBuffer(id, points, flag);
			}
			else {
				deleteBuffer(id);
				createDeviceBuffer(id, points, flag);
			}
		}

		void deleteBuffer(uint32_t id);

		void drawFrame(); */

		// Cleanup
		void cleanup();



		//-------------------------------

		// void initImgui();

		// void imguiRun();

		/* void createSketchPointPipeline();
		void createSketchLinePipeline();

		void createSketchGridPipeline();

		void preparePipelines();

		void destroyPipelines();

		void createDescriptorPool();

		void createDescriptorSets();

		void createCommandBuffers();

		void updateUniformBuffer(uint32_t currentImage);
 */
		void destroy();

		//void runCamera();

		//void render(Model &M);

		//void renderSketchNotes(Model& S);

		//void renderSketchPoints(Model &S);

	};
}

