#pragma once



namespace CADERA_APP_NAMESPACE {

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

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};


	class Canvas {

	protected:

		

	public:

		GLFWwindow* window;
		bool frameBufferResized;

		// Window Size
		int mWidth = 750;
		int mHeight = 750;
		
		vk::UniqueSurfaceKHR mSurface;
		vk::SwapchainKHR mSwapchain;
	
		std::vector<vk::Framebuffer> mFramebuffers;

		void createWindow(std::string name);

		std::vector<vk::Image> mImages;
		std::vector<vk::ImageView> mImageViews;
		vk::Format mFormat;
		vk::Extent2D mExtent;

		vk::Image depthImage;
		vk::DeviceMemory depthImageMemory;
		vk::ImageView depthImageView;

		void destroy(const vk::Device& Device);

		bool checkFormat(vk::Format Format);

		SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device);
		
		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes);

		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

		void createSwapChain(const vk::Device& Device, const vk::PhysicalDevice& PhysicalDevice,
			                 QueueFamilyIndices& Indices);

		uint32_t findMemoryType(vk::PhysicalDevice const& PhysicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);

		void createImage(vk::PhysicalDevice const& PhysicalDevice, vk::Device const& Device, uint32_t width, uint32_t height, 
			             vk::Format format, 
			             vk::ImageTiling tiling,
			             vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);

		void createImageViews(const vk::Device& Device);

		vk::ImageView createImageView(const vk::Device& Device, vk::Image image, vk::Format format, 
			                          vk::ImageAspectFlags aspectFlags);

		vk::Format findSupportedFormat(vk::PhysicalDevice const& PhysicalDevice, const std::vector<vk::Format>& candidates,
			                           vk::ImageTiling tiling, vk::FormatFeatureFlags features);

		vk::Format findDepthFormat(vk::PhysicalDevice const& PhysicalDevice);

		bool hasStencilComponent(vk::Format format);

		void createDepthResources(vk::PhysicalDevice const& PhysicalDevice, vk::Device const& Device);

	};

}