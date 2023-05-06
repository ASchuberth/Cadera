#include "pch.hpp"
#include "Canvas.hpp"

namespace CADERA_APP_NAMESPACE {

	

	void Canvas::createWindow(std::string name) {
		
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		
		window = glfwCreateWindow(mWidth, mHeight, name.c_str(), nullptr, nullptr);
		
		glfwMaximizeWindow(window);
		glfwSetWindowUserPointer(window, this);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
	}

	void Canvas::destroy(const vk::Device& Device) {

		Device.destroyImage(depthImage);
		Device.freeMemory(depthImageMemory);
		Device.destroyImageView(depthImageView);

		for (auto& view : mImageViews)
			Device.destroyImageView(view, nullptr);


		Device.destroySwapchainKHR(mSwapchain, nullptr);

	}

	bool Canvas::checkFormat(vk::Format Format)
	{
		return mFormat == Format;
	}

	SwapChainSupportDetails Canvas::querySwapChainSupport(const vk::PhysicalDevice& device)
	{
		
		SwapChainSupportDetails details;

		details.capabilities = device.getSurfaceCapabilitiesKHR(*mSurface);
		details.formats = device.getSurfaceFormatsKHR(*mSurface);
		details.presentModes = device.getSurfacePresentModesKHR(*mSurface);

		return details;
		
	}

	vk::SurfaceFormatKHR Canvas::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
	{

		if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
			return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };

		}

		for (const auto& availableFormat : availableFormats) {

			if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
				availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	vk::PresentModeKHR Canvas::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes)
	{
		vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;


		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
				return availablePresentMode;
			}
			else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	vk::Extent2D Canvas::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};


			actualExtent.width = std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, actualExtent.width));

			actualExtent.height = std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	void Canvas::createSwapChain(const vk::Device& Device, const vk::PhysicalDevice& PhysicalDevice, 
						         QueueFamilyIndices& Indices)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(PhysicalDevice);

		vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 
			&& imageCount > swapChainSupport.capabilities.maxImageCount) {

				imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR createInfo({}, *mSurface, imageCount, surfaceFormat.format,
			                                  surfaceFormat.colorSpace, extent, 1, 
			                                  vk::ImageUsageFlagBits::eColorAttachment);


		uint32_t queueFamilyIndices[] = { Indices.graphicsFamily, Indices.presentFamily };

		if (Indices.isDifferent()) {

			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

	
		mSwapchain = Device.createSwapchainKHR(createInfo, nullptr);
			


		mImages = Device.getSwapchainImagesKHR(mSwapchain);

		mFormat = surfaceFormat.format;
		mExtent = extent;

	}

	uint32_t Canvas::findMemoryType(vk::PhysicalDevice const& PhysicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
	
		vk::PhysicalDeviceMemoryProperties memProperties;
		memProperties = PhysicalDevice.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");

	}


	void Canvas::createImage(vk::PhysicalDevice const& PhysicalDevice, vk::Device const& Device, uint32_t width, uint32_t height,
							vk::Format format,
							vk::ImageTiling tiling,
							vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, 
		                    vk::Image& image, vk::DeviceMemory& imageMemory) {
	
		

							vk::ImageCreateInfo imageInfo({}, vk::ImageType::e2D, format, { width, height,  1}, 1, 1, vk::SampleCountFlagBits::e1, 
														  tiling, usage, vk::SharingMode::eExclusive);

							image = Device.createImage(imageInfo, nullptr);
	

							vk::MemoryRequirements memRequirements;
							memRequirements = Device.getImageMemoryRequirements(image);


							vk::MemoryAllocateInfo allocInfo(memRequirements.size, findMemoryType(PhysicalDevice, memRequirements.memoryTypeBits, properties));
							imageMemory = Device.allocateMemory(allocInfo, nullptr);


							Device.bindImageMemory(image, imageMemory, 0);

						}

	

	void Canvas::createImageViews(const vk::Device& Device)
	{
		mImageViews.resize(mImages.size());


		for (decltype(mImages.size()) i = 0; i < mImages.size(); i++) {
			mImageViews[i] = createImageView(Device, mImages[i], mFormat, vk::ImageAspectFlagBits::eColor);
		}
	}

	vk::ImageView Canvas::createImageView(const vk::Device& Device, vk::Image image, vk::Format format, 
		                                  vk::ImageAspectFlags aspectFlags)
	{
		vk::ImageViewCreateInfo createInfo = {};
		createInfo.image = image;
		createInfo.viewType = vk::ImageViewType::e2D;//VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.components.r = vk::ComponentSwizzle::eIdentity;//VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = vk::ComponentSwizzle::eIdentity;
		createInfo.components.b = vk::ComponentSwizzle::eIdentity;
		createInfo.components.a = vk::ComponentSwizzle::eIdentity;
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		vk::ImageView imageView;


		Device.createImageView(&createInfo, nullptr, &imageView);
	

		return imageView;
	}

	

	vk::Format Canvas::findSupportedFormat(vk::PhysicalDevice const& PhysicalDevice, const std::vector<vk::Format>& candidates, 
		                                   vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
		
		for (vk::Format format : candidates) {
			vk::FormatProperties props;
			
			props = PhysicalDevice.getFormatProperties(format);
		

			if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		throw std::runtime_error("Failed to find supported format!");
	}

	vk::Format Canvas::findDepthFormat(vk::PhysicalDevice const& PhysicalDevice) {

		return findSupportedFormat(PhysicalDevice,
			{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
			vk::ImageTiling::eOptimal,
			vk::FormatFeatureFlagBits::eDepthStencilAttachment);
	}

	bool Canvas::hasStencilComponent(vk::Format format) {

		return format == vk::Format::eD32Sfloat || format == vk::Format::eD24UnormS8Uint;
	}

	

	void Canvas::createDepthResources(vk::PhysicalDevice const& PhysicalDevice, vk::Device const& Device) {
		vk::Format depthFormat = findDepthFormat(PhysicalDevice);

		createImage(PhysicalDevice, Device, mExtent.width, mExtent.height, depthFormat, vk::ImageTiling::eOptimal, 
			        vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);
		
		depthImageView = createImageView(Device, depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
	}

}
