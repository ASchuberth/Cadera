#include "pch.hpp"
#include "Canvas.hpp"

namespace CADERA_APP_NAMESPACE {


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
