#include "cadrender.hpp"
#include "pch.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace CADERA_APP_NAMESPACE {

CADRender::CADRender() {}

void CADRender::setup() {

  // GLFW
  createWindow();

  // mInstance
  createInstance();
  createSurface();
  
  // Physical mDevice
  pickPhysicalDevice();
  
  // Logical mDevice
  createLogicalDevice();

  // Debug
  setupDebugUtils();
  
  // Swapchain
  createSwapChain();
  createImageViews();

  // Graphics Pipelines
  createRenderPass();
  createDescriptorSetLayout();
  createPipelineLayout();
  loadFonts();
  preparePipelines();

  createCommandPool();
  createDepthResources();
  createTextureImage();
  createTextureImageView();
  createTextureSampler();
  createFramebuffers();
  createUniformBuffer();
  createDescriptorPool();
  createDescriptorSets();
  allocCommandBuffers();
  createSyncObjects();
}

void CADRender::setBGColor(glm::vec4 color) { mRenderColors.bgColor = color; }

void CADRender::createWindow() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  mMainWindow = glfwCreateWindow(300, 300, "Cadera", nullptr, nullptr);

  glfwMaximizeWindow(mMainWindow);
  glfwSetWindowUserPointer(mMainWindow, this);

  glfwSetInputMode(mMainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

vk::Result CADRender::createInstance() {

  vkGetInstanceProcAddr =
      dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

  
  VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
  // VULKAN_HPP_DEFAULT_DISPATCHER.init();

  vk::InstanceCreateInfo createInfo;

  createInfo.enabledLayerCount = 0;

  auto glfwExtensionCount = 0u;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  const char *layers = "VK_LAYER_KHRONOS_validation";

  createInfo.enabledLayerCount = 1; // static_cast<uint32_t>(layers.size());
  createInfo.ppEnabledLayerNames = &layers;

  createInfo.enabledExtensionCount =
      glfwExtensionCount; // static_cast<uint32_t>(glfwExtensionsVector.size());
  createInfo.ppEnabledExtensionNames =
      glfwExtensions; // glfwExtensionsVector.data();
  createInfo.pApplicationInfo = &mAppInfo;

  vk::Result result = vk::createInstance(&createInfo, nullptr, &mInstance);

  VULKAN_HPP_DEFAULT_DISPATCHER.init(mInstance);

  return result;

}

void CADRender::createSurface() {

  if (glfwCreateWindowSurface(mInstance, mMainWindow, nullptr,
                              reinterpret_cast<VkSurfaceKHR *>(&mSurface)) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

void CADRender::setupDebugUtils() {

  // Check if the debug utils extension is present (which is the case if run from a graphics debugger)
  bool extensionPresent = false;
  uint32_t extensionCount;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount) ;//= vk::enumerateInstanceExtensionProperties();
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
  for (auto& extension : extensions) {
    if (strcmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0) {
      extensionPresent = true;
      break;
    }
  }

  if (extensionPresent) {
    // As with an other extension, function pointers need to be manually loaded
    vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
    vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
    vkCmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(vkGetInstanceProcAddr(mInstance, "vkCmdBeginDebugUtilsLabelEXT"));
    vkCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(vkGetInstanceProcAddr(mInstance, "vkCmdInsertDebugUtilsLabelEXT"));
    vkCmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(vkGetInstanceProcAddr(mInstance, "vkCmdEndDebugUtilsLabelEXT"));
    vkQueueBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueBeginDebugUtilsLabelEXT>(vkGetInstanceProcAddr(mInstance, "vkQueueBeginDebugUtilsLabelEXT"));
    vkQueueInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueInsertDebugUtilsLabelEXT>(vkGetInstanceProcAddr(mInstance, "vkQueueInsertDebugUtilsLabelEXT"));
    vkQueueEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueEndDebugUtilsLabelEXT>(vkGetInstanceProcAddr(mInstance, "vkQueueEndDebugUtilsLabelEXT"));
    vkSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(mInstance, "vkSetDebugUtilsObjectNameEXT"));
    pfnDebugMarkerSetObjectTag = (PFN_vkDebugMarkerSetObjectTagEXT)vkGetDeviceProcAddr(mDevice, "vkDebugMarkerSetObjectTagEXT");

    // Set flag if at least one function pointer is present
    debugUtilsSupported = (vkCreateDebugUtilsMessengerEXT != VK_NULL_HANDLE);
  }
  else {
    std::cout << "Warning: " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << " not present, debug utils are disabled.";
    std::cout << "Try running the sample from inside a Vulkan graphics debugger (e.g. RenderDoc)" << std::endl;
  }
}

void CADRender::cmd_begin_label(vk::CommandBuffer cmdBuffer, const char* name, glm::vec4 color ) {

  if (debugUtilsSupported && vkCmdBeginDebugUtilsLabelEXT) {
    
    float col[4] = {color.x, color.y, color.z, color.w};
    VkDebugUtilsLabelEXT labelInfo {
      VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
      nullptr,
      name
    };

    std::copy(std::begin(col), std::end(col), std::begin(labelInfo.color));
    // memcpy(labelInfo.color, &color[0], sizeof(float)*4);
    vkCmdBeginDebugUtilsLabelEXT(cmdBuffer, &labelInfo);

  }
}

void CADRender::cmd_end_label(vk::CommandBuffer cmdBuffer) {

  if (debugUtilsSupported && vkCmdEndDebugUtilsLabelEXT)
    vkCmdEndDebugUtilsLabelEXT(cmdBuffer);
}

void CADRender::setObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char *name) {
  // Check for valid function pointer (may not be present if not running in a debugging application)
  if (debugUtilsSupported && pfnDebugMarkerSetObjectTag) {
      VkDebugMarkerObjectTagInfoEXT tagInfo = {};
      tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
      tagInfo.objectType = objectType;
      tagInfo.object = object;
      tagInfo.tagName = 0;
      tagInfo.pTag = name;
      tagInfo.tagSize = strlen(name);

      //tagInfo.pObjectName = name;
      pfnDebugMarkerSetObjectTag(device, &tagInfo);
  }
}

void CADRender::pickPhysicalDevice() {

  std::vector<vk::PhysicalDevice> devices =
      mInstance.enumeratePhysicalDevices();

  if (devices.empty()) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  for (const auto &device : devices) {
    if (isDeviceSuitable(device)) {
      mPhysicalDevice = device;
      break;
    }
  }

  if (&mPhysicalDevice == VK_NULL_HANDLE) {
    std::cout << "Failed!" << std::endl;
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

SwapChainSupportDetails
CADRender::querySwapChainSupport(const vk::PhysicalDevice &device) {

  SwapChainSupportDetails details;

  details.capabilities = device.getSurfaceCapabilitiesKHR(mSurface);
  details.formats = device.getSurfaceFormatsKHR(mSurface);
  details.presentModes = device.getSurfacePresentModesKHR(mSurface);

  return details;
}

QueueFamilyIndices CADRender::findQueueFamilies(vk::PhysicalDevice device,
                                                VkSurfaceKHR surface) {
  QueueFamilyIndices indices;

  std::vector<vk::QueueFamilyProperties> queueFamilies =
      device.getQueueFamilyProperties();

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueCount > 0 &&
        queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    presentSupport = device.getSurfaceSupportKHR(i, surface);

    if (queueFamily.queueCount > 0 && presentSupport) {
      indices.presentFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }

    i++;
  }

  return indices;
}

bool CADRender::isDeviceSuitable(vk::PhysicalDevice device) {

  mIndices = findQueueFamilies(device, mSurface);

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  return mIndices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool CADRender::checkDeviceExtensionSupport(vk::PhysicalDevice device) {
  std::vector<vk::ExtensionProperties> availableExtensions =
      device.enumerateDeviceExtensionProperties(nullptr);

  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());

  for (const auto &extension : availableExtensions) {

    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

void CADRender::createLogicalDevice() {

  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
  std::set<int> uniqueQueueFamilies = {mIndices.graphicsFamily,
                                       mIndices.presentFamily};

  float queuePriority = 1.0f;
  for (int queueFamily : uniqueQueueFamilies) {

    vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1,
                                              &queuePriority);
    queueCreateInfos.push_back(queueCreateInfo);
  }

  vk::PhysicalDeviceFeatures deviceFeatures;

  deviceFeatures.fillModeNonSolid = true;
  deviceFeatures.depthBounds = true;
  deviceFeatures.wideLines = true;
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  vk::DeviceCreateInfo createInfo(
      {}, static_cast<uint32_t>(queueCreateInfos.size()),
      queueCreateInfos.data(), {}, {},
      static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
      &deviceFeatures);

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  mDevice = mPhysicalDevice.createDevice(createInfo, nullptr);

  mGraphicsQueue = mDevice.getQueue(mIndices.graphicsFamily, 0);
  mPresentQueue = mDevice.getQueue(mIndices.presentFamily, 0);

  VULKAN_HPP_DEFAULT_DISPATCHER.init(mDevice);
  vkGetDeviceProcAddr =
      dl.getProcAddress<PFN_vkGetDeviceProcAddr>("vkGetDeviceProcAddr");
}

vk::SurfaceFormatKHR CADRender::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &availableFormats) {

  if (availableFormats.size() == 1 &&
      availableFormats[0].format == vk::Format::eUndefined) {
    return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
  }

  for (const auto &availableFormat : availableFormats) {

    if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

      return availableFormat;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR CADRender::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> availablePresentModes) {
  vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
      return availablePresentMode;
    } else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
      bestMode = availablePresentMode;
    }
  }

  return bestMode;
}

vk::Extent2D
CADRender::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(mMainWindow, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actualExtent.width));

    actualExtent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

void CADRender::createSwapChain() {
  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(mPhysicalDevice);

  vk::SurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
  vk::PresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
  vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {

    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR createInfo(
      {}, mSurface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace,
      extent, 1, vk::ImageUsageFlagBits::eColorAttachment);

  uint32_t queueFamilyIndices[] = {
      static_cast<uint32_t>(mIndices.graphicsFamily),
      static_cast<uint32_t>(mIndices.presentFamily)};

  if (mIndices.isDifferent()) {

    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  mSwapchain = mDevice.createSwapchainKHR(createInfo, nullptr);

  mImages = mDevice.getSwapchainImagesKHR(mSwapchain);

  mFormat = surfaceFormat.format;
  mExtent = extent;
}

void CADRender::recreateSwapchain() {

  int width = 0, height = 0;
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(mMainWindow, &width, &height);
    glfwWaitEvents();
  }

  mDevice.waitIdle();

  cleanupSwapchain();

  createSwapChain();
  createImageViews();

  createRenderPass();
  createDescriptorSetLayout();
  createPipelineLayout();
  createDepthResources();
  createFramebuffers();

  preparePipelines();
}

void CADRender::createImageViews() {
  mImageViews.resize(mImages.size());

  for (decltype(mImages.size()) i = 0; i < mImages.size(); i++) {
    mImageViews[i] =
        createImageView(mImages[i], mFormat, vk::ImageAspectFlagBits::eColor);
  }
}

vk::ImageView CADRender::createImageView(vk::Image image, vk::Format format,
                                         vk::ImageAspectFlags aspectFlags) {
  vk::ImageViewCreateInfo createInfo = {};
  createInfo.image = image;
  createInfo.viewType = vk::ImageViewType::e2D; // VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = format;
  createInfo.components.r =
      vk::ComponentSwizzle::eIdentity; // VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.g = vk::ComponentSwizzle::eIdentity;
  createInfo.components.b = vk::ComponentSwizzle::eIdentity;
  createInfo.components.a = vk::ComponentSwizzle::eIdentity;
  createInfo.subresourceRange.aspectMask = aspectFlags;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  vk::ImageView imageView;

  vk::Result result{};
                                  
  result = mDevice.createImageView(&createInfo, nullptr, &imageView);

  if (result != vk::Result::eSuccess) 
    throw("Failed to create ImageView");
  

  return imageView;
}

vk::Format
CADRender::findSupportedFormat(vk::PhysicalDevice const &PhysicalDevice,
                               const std::vector<vk::Format> &candidates,
                               vk::ImageTiling tiling,
                               vk::FormatFeatureFlags features) {

  for (vk::Format format : candidates) {
    vk::FormatProperties props;

    props = PhysicalDevice.getFormatProperties(format);

    if (tiling == vk::ImageTiling::eLinear &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == vk::ImageTiling::eOptimal &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  throw std::runtime_error("Failed to find supported format!");
}

vk::Format
CADRender::findDepthFormat(vk::PhysicalDevice const &PhysicalDevice) {

  return findSupportedFormat(
      PhysicalDevice,
      {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint,
       vk::Format::eD24UnormS8Uint},
      vk::ImageTiling::eOptimal,
      vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

void CADRender::createRenderPass() {

  // Color Attachment
  vk::AttachmentDescription colorAttachment = {};
  colorAttachment.format = mFormat;
  colorAttachment.samples = vk::SampleCountFlagBits::e1;
  colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
  colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
  colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

  vk::AttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

  // Depth Buffer Attachment
  vk::AttachmentDescription depthAttachment = {};
  depthAttachment.format = findDepthFormat(mPhysicalDevice);
  depthAttachment.samples = vk::SampleCountFlagBits::e1;
  depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
  depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
  depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
  depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

  vk::AttachmentReference depthAttachmentRef = {};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

  vk::SubpassDescription subpass = {};
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  vk::SubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
  dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead |
                             vk::AccessFlagBits::eColorAttachmentWrite;

  std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment,
                                                          depthAttachment};
  vk::RenderPassCreateInfo renderPassInfo = {};

  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (mDevice.createRenderPass(&renderPassInfo, nullptr, &mRenderPass) !=
      vk::Result::eSuccess)
    throw std::runtime_error("failed to create render pass!");

}

void CADRender::createDescriptorSetLayout() {

  vk::DescriptorSetLayoutBinding uboLayoutBinding(
      0, vk::DescriptorType::eUniformBuffer, 1,
      vk::ShaderStageFlagBits::eVertex, nullptr);

  vk::DescriptorSetLayoutBinding cisLayoutBinding(
      1, vk::DescriptorType::eCombinedImageSampler, 1,
      vk::ShaderStageFlagBits::eFragment, nullptr);

  std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding,
                                                            cisLayoutBinding};

  vk::DescriptorSetLayoutCreateInfo layoutInfo(
      {}, static_cast<uint32_t>(bindings.size()), bindings.data());

  if (mDevice.createDescriptorSetLayout(&layoutInfo, nullptr,
                                        &this->mDescriptorSetLayout) !=
      vk::Result::eSuccess)
    throw std::runtime_error("failed to create descriptor set layout!");
}

void CADRender::createPipelineLayout() {

  vk::PipelineLayoutCreateInfo pipelineLayoutInfo({}, 1, &mDescriptorSetLayout,
                                                  0, nullptr);

  if (mDevice.createPipelineLayout(&pipelineLayoutInfo, nullptr,
                                   &mPipelineLayout) != vk::Result::eSuccess)
    throw std::runtime_error("failed to create pipeline layout");
}

std::vector<char> CADRender::readFile(const std::string filename) {

  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

vk::ShaderModule CADRender::createShaderModule(const std::vector<char> &code) {
  vk::ShaderModuleCreateInfo createInfo = {};

  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  vk::ShaderModule shaderModule;

  if (mDevice.createShaderModule(&createInfo, nullptr, &shaderModule) !=
      vk::Result::eSuccess) {
    throw std::runtime_error("failed to create shader module!");
  }

  return shaderModule;
}

void CADRender::loadFonts() {

  TxtRend.setFontSize(100.0f);
  TxtRend.loadFont(config::arial_font_coordinates_file_path);
}


void CADRender::createCommandPool() {

  vk::CommandPoolCreateInfo commandPoolInfo(
      vk::CommandPoolCreateFlagBits::eTransient, mIndices.graphicsFamily);


  vk::Result result{};

  result = mDevice.createCommandPool(&commandPoolInfo, nullptr, &mCommandPool);

  if (result != vk::Result::eSuccess) {
    throw("Command Pool was not created!\n");
  }
  
  
}

uint32_t CADRender::findMemoryType(uint32_t typeFilter,
                                   vk::MemoryPropertyFlags properties) {

  vk::PhysicalDeviceMemoryProperties memProperties;
  memProperties = mPhysicalDevice.getMemoryProperties();

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}

void CADRender::createImage(vk::PhysicalDevice const &PhysicalDevice,
                            vk::Device const &Device, uint32_t width,
                            uint32_t height, vk::Format format,
                            vk::ImageTiling tiling, vk::ImageUsageFlags usage,
                            vk::MemoryPropertyFlags properties,
                            vk::Image &image, vk::DeviceMemory &imageMemory) {

  vk::ImageCreateInfo imageInfo(
      {}, vk::ImageType::e2D, format, {width, height, 1}, 1, 1,
      vk::SampleCountFlagBits::e1, tiling, usage, vk::SharingMode::eExclusive);

  image = Device.createImage(imageInfo, nullptr);

  vk::MemoryRequirements memRequirements;
  memRequirements = Device.getImageMemoryRequirements(image);

  vk::MemoryAllocateInfo allocInfo(
      memRequirements.size,
      findMemoryType(memRequirements.memoryTypeBits, properties));
  imageMemory = Device.allocateMemory(allocInfo, nullptr);

  Device.bindImageMemory(image, imageMemory, 0);
}

void CADRender::createDepthResources() {
  vk::Format depthFormat = findDepthFormat(mPhysicalDevice);

  createImage(mPhysicalDevice, mDevice, mExtent.width, mExtent.height,
              depthFormat, vk::ImageTiling::eOptimal,
              vk::ImageUsageFlagBits::eDepthStencilAttachment,
              vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage,
              depthImageMemory);

  depthImageView =
      createImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
}

void CADRender::createBuffer(vk::DeviceSize &size,
                             const vk::BufferUsageFlags &usage,
                             const vk::MemoryPropertyFlags &properties,
                             vk::Buffer &buffer,
                             vk::DeviceMemory &bufferMemory) {

  vk::BufferCreateInfo bufferInfo({}, size, usage, vk::SharingMode::eExclusive);

  buffer = mDevice.createBuffer(bufferInfo, nullptr);

  vk::MemoryRequirements memRequirements;
  mDevice.getBufferMemoryRequirements(buffer, &memRequirements);

  vk::MemoryAllocateInfo allocInfo(
      memRequirements.size,
      findMemoryType(memRequirements.memoryTypeBits, properties));

  bufferMemory = mDevice.allocateMemory(allocInfo);
  mDevice.bindBufferMemory(buffer, bufferMemory, 0);
}

vk::CommandBuffer CADRender::beginSingleTimeCommands(
    const vk::CommandBufferLevel &level,
    const vk::CommandBufferInheritanceInfo &inheritance) {

  // 3rd parameter is for Comm. Buffer Count
  vk::CommandBufferAllocateInfo allocInfo(mCommandPool, level, 1);

  std::vector<vk::CommandBuffer> commandBuffers =
      mDevice.allocateCommandBuffers(allocInfo);

  vk::CommandBufferBeginInfo beginInfo(
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  if (level == vk::CommandBufferLevel::eSecondary) {
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;
    beginInfo.pInheritanceInfo = &inheritance;
  }

  if (!commandBuffers.empty()) {
    commandBuffers[0].begin(beginInfo);
  } else {
    throw std::runtime_error("No Command Buffers in beginSingleTimeCommands()");
  }

  return commandBuffers[0];
}

void CADRender::endSingleTimeCommands(vk::CommandBuffer &commandBuffer) {

  commandBuffer.end();

  vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr);

  mGraphicsQueue.submit(submitInfo, nullptr);
  mGraphicsQueue.waitIdle();
  mDevice.freeCommandBuffers(mCommandPool, commandBuffer);
}

void CADRender::transitionImageLayout(vk::Image &image, vk::Format format,
                                      vk::ImageLayout oldLayout,
                                      vk::ImageLayout newLayout) {

  vk::CommandBuffer commandBuffer = beginSingleTimeCommands(
      vk::CommandBufferLevel::ePrimary, vk::CommandBufferInheritanceInfo());

  vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

  vk::ImageMemoryBarrier barrier({}, {}, oldLayout, newLayout,
                                 VK_QUEUE_FAMILY_IGNORED,
                                 VK_QUEUE_FAMILY_IGNORED, image, range);

  vk::PipelineStageFlags sourceStage;
  vk::PipelineStageFlags destinationStage;

  if (oldLayout == vk::ImageLayout::eUndefined &&
      newLayout == vk::ImageLayout::eTransferDstOptimal) {
    barrier.srcAccessMask = {};
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

    sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
    destinationStage = vk::PipelineStageFlagBits::eTransfer;
  } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
             newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

    sourceStage = vk::PipelineStageFlagBits::eTransfer;
    destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }

  commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, 0, nullptr,
                                0, nullptr, 1, &barrier);

  endSingleTimeCommands(commandBuffer);
}

void CADRender::copyBufferToImage(vk::Buffer &buffer, vk::Image &image,
                                  uint32_t width, uint32_t height) {

  vk::CommandBuffer commandBuffer = beginSingleTimeCommands(
      vk::CommandBufferLevel::ePrimary, vk::CommandBufferInheritanceInfo());

  vk::ImageSubresourceLayers layers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);

  vk::BufferImageCopy region(0, 0, 0, layers, {0, 0, 0}, {width, height, 1});

  commandBuffer.copyBufferToImage(
      buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

  endSingleTimeCommands(commandBuffer);
}

void CADRender::createTextureImage() {

  int texWidth, texHeight, texChannels;

  stbi_uc *pixels = stbi_load(config::arial_font_texture_file_path, &texWidth, &texHeight,
                              &texChannels, STBI_rgb_alpha);
  vk::DeviceSize imageSize = (uint64_t)texWidth * (uint64_t)texHeight * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  vk::Buffer stagingBuffer;
  vk::DeviceMemory stagingBufferMemory;
  createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
               vk::MemoryPropertyFlagBits::eHostVisible |
                   vk::MemoryPropertyFlagBits::eHostCoherent,
               stagingBuffer, stagingBufferMemory);

  void *data;
  data = mDevice.mapMemory(stagingBufferMemory, 0, imageSize);
  memcpy(data, pixels, (size_t)imageSize);
  mDevice.unmapMemory(stagingBufferMemory);

  stbi_image_free(pixels);

  createImage(
      mPhysicalDevice, mDevice, texWidth, texHeight, vk::Format::eR8G8B8A8Srgb,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
      vk::MemoryPropertyFlagBits::eDeviceLocal, mTextureImage, mTextureMemory);

  transitionImageLayout(mTextureImage, vk::Format::eR8G8B8A8Srgb,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::eTransferDstOptimal);

  copyBufferToImage(stagingBuffer, mTextureImage,
                    static_cast<uint32_t>(texWidth),
                    static_cast<uint32_t>(texHeight));

  transitionImageLayout(mTextureImage, vk::Format::eR8G8B8A8Srgb,
                        vk::ImageLayout::eTransferDstOptimal,
                        vk::ImageLayout::eShaderReadOnlyOptimal);

  mDevice.destroyBuffer(stagingBuffer);
  mDevice.freeMemory(stagingBufferMemory);
}

void CADRender::createTextureImageView() {

  mTextureImageView = createImageView(mTextureImage, vk::Format::eR8G8B8A8Srgb,
                                      vk::ImageAspectFlagBits::eColor);
}

void CADRender::createTextureSampler() {

  vk::SamplerCreateInfo samplerInfo(
      {}, vk::Filter::eLinear, vk::Filter::eLinear,
      vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eRepeat,
      vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0.0f,
      VK_TRUE, 16, VK_FALSE, vk::CompareOp::eNever, 0.0f, 0.0f,
      vk::BorderColor::eIntOpaqueBlack, VK_FALSE);

  mTextureSampler = mDevice.createSampler(samplerInfo, nullptr);
}

void CADRender::createFramebuffers() {

  mFramebuffers.resize(mImageViews.size());

  for (decltype(mImageViews.size()) i = 0; i < mImageViews.size(); i++) {

    std::array<vk::ImageView, 2> attachments = {mImageViews[i], depthImageView};

    vk::FramebufferCreateInfo FramebufferInfo(
        {}, mRenderPass, static_cast<uint32_t>(attachments.size()),
        attachments.data(), mExtent.width, mExtent.height, 1);


    vk::Result result{};

    result = mDevice.createFramebuffer(&FramebufferInfo, nullptr, &mFramebuffers[i]);

    if (result != vk::Result::eSuccess) 
      throw("Framebuffer was not created!\n");
    
  }
}

void CADRender::createUniformBuffer() {

  vk::DeviceSize bufferSize = sizeof(ubo);

  mUniformBuffers.resize(mImages.size());
  mUniformBufferMemories.resize(mImages.size());

  for (size_t i = 0; i < mImages.size(); i++) {
    createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
                 vk::MemoryPropertyFlagBits::eHostVisible |
                     vk::MemoryPropertyFlagBits::eHostCoherent,
                 mUniformBuffers[i], mUniformBufferMemories[i]);
  }
}

void CADRender::createDescriptorPool() {

  std::array<vk::DescriptorPoolSize, 2> poolSizes = {};
  poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
  poolSizes[0].descriptorCount = static_cast<uint32_t>(mImages.size());
  poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
  poolSizes[1].descriptorCount = static_cast<uint32_t>(mImages.size());

  vk::DescriptorPoolCreateInfo poolInfo(
      {}, static_cast<uint32_t>(mImages.size()),
      static_cast<uint32_t>(poolSizes.size()), poolSizes.data());

  mDescriptorPool = mDevice.createDescriptorPool(poolInfo, nullptr);
}

void CADRender::createDescriptorSets() {

  std::vector<vk::DescriptorSetLayout> layouts(mImages.size(),
                                               mDescriptorSetLayout);

  vk::DescriptorSetAllocateInfo allocInfo(
      mDescriptorPool, static_cast<uint32_t>(mImages.size()), layouts.data());

  mDescriptorSets.resize(mImages.size());

  mDescriptorSets = mDevice.allocateDescriptorSets(allocInfo);

  // Uniform Buffer

  for (size_t i = 0; i < mImages.size(); i++) {

    vk::DescriptorBufferInfo bufferInfo(mUniformBuffers[i], 0, sizeof(u));

    vk::DescriptorImageInfo imageInfo(mTextureSampler, mTextureImageView,
                                      vk::ImageLayout::eShaderReadOnlyOptimal);

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
    descriptorWrites[1].descriptorType =
        vk::DescriptorType::eCombinedImageSampler;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    mDevice.updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()),
                                 descriptorWrites.data(), 0, nullptr);
  }
}

void CADRender::allocCommandBuffers() {

  mCommandBuffers.resize(mFramebuffers.size());

  vk::CommandBufferAllocateInfo allocInfo(mCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          (uint32_t)mCommandBuffers.size());

  mCommandBuffers = mDevice.allocateCommandBuffers(allocInfo);
}

void CADRender::createSyncObjects() {

  mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  vk::SemaphoreCreateInfo semaphoreInfo;

  vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    mImageAvailableSemaphores[i] = mDevice.createSemaphore(semaphoreInfo);
    mRenderFinishedSemaphores[i] = mDevice.createSemaphore(semaphoreInfo);
    mInFlightFences[i] = mDevice.createFence(fenceInfo, nullptr);
  }
}

void CADRender::initImgui() {

  // Create Descriptor Pool

  vk::DescriptorPoolSize pool_sizes[] = {

      {vk::DescriptorType::eCombinedImageSampler, 1000},

  };

  vk::DescriptorPoolCreateInfo poolInfo(
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
      1000 * IM_ARRAYSIZE(pool_sizes), (uint32_t)IM_ARRAYSIZE(pool_sizes),
      pool_sizes);

  mGuiDescriptorPool = mDevice.createDescriptorPool(poolInfo);

  // Create Window Surface

  VkResult err =
      glfwCreateWindowSurface(mInstance, mMainWindow, nullptr, &mImguiSurface);

  int w, h;
  glfwGetFramebufferSize(mMainWindow, &w, &h);
  ImGui_ImplVulkanH_Window *wd = &mImguiWindowData;

  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.WantCaptureMouse = true;

  // Setup GLFW binding
  ImGui_ImplGlfw_InitForVulkan(mMainWindow, true);

  // Setup Vulkan binding
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = mInstance;
  init_info.PhysicalDevice = mPhysicalDevice;
  init_info.Device = mDevice;
  init_info.QueueFamily = mIndices.graphicsFamily;
  init_info.Queue = mGraphicsQueue;
  init_info.PipelineCache = VK_NULL_HANDLE;
  init_info.DescriptorPool = mGuiDescriptorPool;
  init_info.Subpass = 0;
  init_info.MinImageCount = static_cast<uint32_t>(mImageViews.size());
  init_info.ImageCount = static_cast<uint32_t>(mImageViews.size());
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = VK_NULL_HANDLE;
  init_info.CheckVkResultFn = VK_NULL_HANDLE;

  ImGui_ImplVulkan_Init(&init_info, mRenderPass);

  // Setup style
  ImGui::StyleColorsDark();

  // Upload Fonts
  {
    // Use any command queue

    vk::CommandBuffer command_buffer =
        beginSingleTimeCommands(vk::CommandBufferLevel::ePrimary, {});

    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

    endSingleTimeCommands(command_buffer);
  }
}

void CADRender::createSketchPointPipeline() {

  vk::VertexInputBindingDescription BindingDescription(
      0, sizeof(Vertex), vk::VertexInputRate::eVertex);

  std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
      BindingDescription};

  std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(2);

  AttributeDescriptions[0].binding = 0;
  AttributeDescriptions[0].location = 0;
  AttributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[0].offset = offsetof(Vertex, pos);

  AttributeDescriptions[1].binding = 0;
  AttributeDescriptions[1].location = 1;
  AttributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[1].offset = offsetof(Vertex, col);

  vk::PipelineVertexInputStateCreateInfo VertexInputInfo(
      {}, static_cast<uint32_t>(BindingDescriptions.size()),
      BindingDescriptions.data(),
      static_cast<uint32_t>(AttributeDescriptions.size()),
      AttributeDescriptions.data());

  auto vertShaderCode = readFile(config::vertex_shader_file_path);
  auto fragShaderCode = readFile(config::fragment_shader_file_path);

  vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  vk::PipelineShaderStageCreateInfo VertShaderInfo(
      {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
  vk::PipelineShaderStageCreateInfo FragShaderInfo(
      {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

  vk::PipelineShaderStageCreateInfo ShaderStages[] = {VertShaderInfo,
                                                      FragShaderInfo};

  vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo(
      {}, vk::PrimitiveTopology::ePointList, VK_FALSE);

  vk::PipelineRasterizationStateCreateInfo RasterizerInfo(
      {}, VK_FALSE, VK_FALSE, vk::PolygonMode::ePoint,
      vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise, VK_FALSE);

  vk::Viewport ViewPort(0.0f, 0.0f, static_cast<float>(mExtent.width),
                        static_cast<float>(mExtent.height), 0.0f, 1.0f);

  vk::Rect2D Scissor({0, 0}, mExtent);

  vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1,
                                                   &Scissor);

  vk::PipelineMultisampleStateCreateInfo MultisampleInfo(
      {}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE,
      VK_FALSE);

  vk::PipelineColorBlendAttachmentState ColorBlendAttachment(
      VK_TRUE, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha,
      vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo(
      {}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
      {0.0f, 0.0f, 0.0f, 0.0f});

  vk::PipelineDepthStencilStateCreateInfo depthStencilInfo(
      {}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_TRUE, VK_FALSE, {}, {},
      0.0f, 1.0f);

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

  pipelines.SketchPoint =
      mDevice
          .createGraphicsPipeline(mPipelineCache, PipelineCreateInfo, nullptr)
          .value;

  mDevice.destroyShaderModule(vertShaderModule, nullptr);
  mDevice.destroyShaderModule(fragShaderModule, nullptr);
}

void CADRender::createSketchLinePipeline() {
  vk::VertexInputBindingDescription BindingDescription(
      0, sizeof(Vertex), vk::VertexInputRate::eVertex);

  std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
      BindingDescription};

  std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(2);

  AttributeDescriptions[0].binding = 0;
  AttributeDescriptions[0].location = 0;
  AttributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[0].offset = offsetof(Vertex, pos);

  AttributeDescriptions[1].binding = 0;
  AttributeDescriptions[1].location = 1;
  AttributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[1].offset = offsetof(Vertex, col);

  vk::PipelineVertexInputStateCreateInfo VertexInputInfo(
      {}, static_cast<uint32_t>(BindingDescriptions.size()),
      BindingDescriptions.data(),
      static_cast<uint32_t>(AttributeDescriptions.size()),
      AttributeDescriptions.data());

  auto vertShaderCode = readFile(config::vertex_shader_file_path);
  auto fragShaderCode = readFile(config::fragment_shader_file_path);

  vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  vk::PipelineShaderStageCreateInfo VertShaderInfo(
      {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
  vk::PipelineShaderStageCreateInfo FragShaderInfo(
      {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

  vk::PipelineShaderStageCreateInfo ShaderStages[] = {VertShaderInfo,
                                                      FragShaderInfo};

  vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo(
      {}, vk::PrimitiveTopology::eLineList, VK_FALSE);

  vk::PipelineRasterizationStateCreateInfo RasterizerInfo(
      {}, VK_FALSE, VK_FALSE, vk::PolygonMode::eLine,
      vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise, VK_FALSE,
      {}, {}, {}, 1.0f);

  vk::Viewport ViewPort(0.0f, 0.0f, static_cast<float>(mExtent.width),
                        static_cast<float>(mExtent.height), 0.0f, 1.0f);

  vk::Rect2D Scissor({0, 0}, mExtent);

  vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1,
                                                   &Scissor);

  vk::PipelineMultisampleStateCreateInfo MultisampleInfo(
      {}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE,
      VK_FALSE);

  vk::PipelineColorBlendAttachmentState ColorBlendAttachment(
      VK_TRUE, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha,
      vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo(
      {}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
      {0.0f, 0.0f, 0.0f, 0.0f});

  vk::PipelineDepthStencilStateCreateInfo depthStencilInfo(
      {}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_TRUE, VK_FALSE, {}, {},
      0.0f, 1.0f);

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

  pipelines.SketchLine =
      mDevice
          .createGraphicsPipeline(mPipelineCache, PipelineCreateInfo, nullptr)
          .value;

  mDevice.destroyShaderModule(vertShaderModule, nullptr);
  mDevice.destroyShaderModule(fragShaderModule, nullptr);

  // setObjectName(mDevice, VK_OBJECT_TYPE_PIPELINE, (uint64_t)0, "Sketch Line pipeline");
  
}

void CADRender::createSketchGridPipeline() {

  vk::VertexInputBindingDescription BindingDescription(
      0, sizeof(Vertex), vk::VertexInputRate::eVertex);
  vk::VertexInputBindingDescription BindingDescriptionInstance(
      1, sizeof(GridRotationAxis), vk::VertexInputRate::eInstance);

  std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
      BindingDescription, BindingDescriptionInstance};

  std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(6);

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
  AttributeDescriptions[4].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[4].offset = offsetof(GridRotationAxis, color);

  AttributeDescriptions[5].binding = 1;
  AttributeDescriptions[5].location = 5;
  AttributeDescriptions[5].format = vk::Format::eR32Sfloat;
  AttributeDescriptions[5].offset = offsetof(GridRotationAxis, angle);

  vk::PipelineVertexInputStateCreateInfo VertexInputInfo(
      {}, static_cast<uint32_t>(BindingDescriptions.size()),
      BindingDescriptions.data(),
      static_cast<uint32_t>(AttributeDescriptions.size()),
      AttributeDescriptions.data());

  auto vertShaderCode = readFile(config::grid_vertex_shader_file_path);
  auto fragShaderCode = readFile(config::fragment_shader_file_path);

  vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  vk::PipelineShaderStageCreateInfo VertShaderInfo(
      {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
  vk::PipelineShaderStageCreateInfo FragShaderInfo(
      {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

  vk::PipelineShaderStageCreateInfo ShaderStages[] = {VertShaderInfo,
                                                      FragShaderInfo};

  vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo(
      {}, vk::PrimitiveTopology::eLineList, VK_FALSE);

  vk::PipelineRasterizationStateCreateInfo RasterizerInfo(
      {}, VK_FALSE, VK_FALSE, vk::PolygonMode::eLine,
      vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise, VK_FALSE,
      {}, {}, {}, 1.0f);

  vk::Viewport ViewPort(0.0f, 0.0f, static_cast<float>(mExtent.width),
                        static_cast<float>(mExtent.height), 0.0f, 1.0f);

  vk::Rect2D Scissor({0, 0}, mExtent);

  vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1,
                                                   &Scissor);

  vk::PipelineMultisampleStateCreateInfo MultisampleInfo(
      {}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE,
      VK_FALSE);

  vk::PipelineColorBlendAttachmentState ColorBlendAttachment(
      VK_TRUE, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha,
      vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo(
      {}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
      {0.0f, 0.0f, 0.0f, 0.0f});

  vk::PipelineDepthStencilStateCreateInfo depthStencilInfo(
      {}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_TRUE, VK_FALSE, {}, {},
      0.0f, 1.0f);

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

  pipelines.SketchGrid =
      mDevice.createGraphicsPipeline({}, PipelineCreateInfo, nullptr).value;

  mDevice.destroyShaderModule(vertShaderModule, nullptr);
  mDevice.destroyShaderModule(fragShaderModule, nullptr);
}

void CADRender::createTextPipeline() {

  vk::VertexInputBindingDescription BindingDescription(
      0, sizeof(txt::Vertex), vk::VertexInputRate::eVertex);

  std::vector<vk::VertexInputBindingDescription> BindingDescriptions = {
      BindingDescription};

  std::vector<vk::VertexInputAttributeDescription> AttributeDescriptions(4);

  AttributeDescriptions[0].binding = 0;
  AttributeDescriptions[0].location = 0;
  AttributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[0].offset = offsetof(txt::Vertex, pos);

  AttributeDescriptions[1].binding = 0;
  AttributeDescriptions[1].location = 1;
  AttributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[1].offset = offsetof(txt::Vertex, textColor);

  AttributeDescriptions[2].binding = 0;
  AttributeDescriptions[2].location = 2;
  AttributeDescriptions[2].format = vk::Format::eR32G32B32Sfloat;
  AttributeDescriptions[2].offset = offsetof(txt::Vertex, backgroundColor);

  AttributeDescriptions[3].binding = 0;
  AttributeDescriptions[3].location = 3;
  AttributeDescriptions[3].format = vk::Format::eR32G32Sfloat;
  AttributeDescriptions[3].offset = offsetof(txt::Vertex, texCoord);

  vk::PipelineVertexInputStateCreateInfo VertexInputInfo(
      {}, static_cast<uint32_t>(BindingDescriptions.size()),
      BindingDescriptions.data(),
      static_cast<uint32_t>(AttributeDescriptions.size()),
      AttributeDescriptions.data());

  auto vertShaderCode = readFile(config::text_vertex_shader_file_path);
  auto fragShaderCode = readFile(config::text_fragment_shader_file_path);

  vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  vk::PipelineShaderStageCreateInfo VertShaderInfo(
      {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main");
  vk::PipelineShaderStageCreateInfo FragShaderInfo(
      {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main");

  vk::PipelineShaderStageCreateInfo ShaderStages[] = {VertShaderInfo,
                                                      FragShaderInfo};

  vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo(
      {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE);

  vk::PipelineRasterizationStateCreateInfo RasterizerInfo(
      {}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill,
      vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise, VK_FALSE);

  vk::Viewport ViewPort(0.0f, 0.0f, static_cast<float>(mExtent.width),
                        static_cast<float>(mExtent.height), 0.0f, 1.0f);

  vk::Rect2D Scissor({0, 0}, mExtent);

  vk::PipelineViewportStateCreateInfo ViewportInfo({}, 1, &ViewPort, 1,
                                                   &Scissor);

  vk::PipelineMultisampleStateCreateInfo MultisampleInfo(
      {}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE,
      VK_FALSE);

  vk::PipelineColorBlendAttachmentState ColorBlendAttachment(
      VK_TRUE, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha,
      vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo ColorBlendingInfo(
      {}, VK_FALSE, vk::LogicOp::eCopy, 1, &ColorBlendAttachment,
      {0.0f, 0.0f, 0.0f, 0.0f});

  vk::PipelineDepthStencilStateCreateInfo depthStencilInfo(
      {}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_TRUE, VK_FALSE, {}, {},
      0.0f, 1.0f);

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

  mTextPipeline = static_cast<vk::Pipeline>(
      mDevice
          .createGraphicsPipeline(mPipelineCache, PipelineCreateInfo, nullptr)
          .value);

  mDevice.destroyShaderModule(vertShaderModule, nullptr);
  mDevice.destroyShaderModule(fragShaderModule, nullptr);
}

void CADRender::preparePipelines() {

  createTextPipeline();
  createSketchPointPipeline();
  createSketchLinePipeline();
  createSketchGridPipeline();
}

void CADRender::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer,
                           vk::DeviceSize size) {

  vk::CommandBuffer commandBuffer = beginSingleTimeCommands(
      vk::CommandBufferLevel::ePrimary, vk::CommandBufferInheritanceInfo());

  vk::BufferCopy copyRegion;
  copyRegion.size = size;

  commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

  endSingleTimeCommands(commandBuffer);
}

void CADRender::deleteBuffer(int id) {

  mBuffers[id].isEmpty = true;
  mDevice.destroyBuffer(mBuffers[id].mBuffer);
  mDevice.freeMemory(mBuffers[id].mMemory);
}

void CADRender::createCommandBuffers() {

  mDevice.resetCommandPool(mCommandPool, vk::CommandPoolResetFlags());

  for (size_t i = 0; i < mCommandBuffers.size(); i++) {

    vk::CommandBufferBeginInfo beginInfo(
        vk::CommandBufferUsageFlagBits::eSimultaneousUse);

    mCommandBuffers[i].begin(beginInfo);

    vk::Rect2D renderArea({0, 0}, {mExtent.width, mExtent.height});

    std::array<float, 4> color = {mRenderColors.bgColor.x,
                                  mRenderColors.bgColor.y,
                                  mRenderColors.bgColor.z,
                                  mRenderColors.bgColor.w};
    

    std::array<vk::ClearValue, 2> clearValues{};
    clearValues[0].setColor(color);
    clearValues[1].depthStencil.depth = 1.0f;
    clearValues[1].depthStencil.stencil = 0;

    vk::RenderPassBeginInfo renderPassInfo(
        mRenderPass, mFramebuffers[i], renderArea,
        static_cast<uint32_t>(clearValues.size()), clearValues.data());

    cmd_begin_label(mCommandBuffers[i], "Model Render", {0.5f, 0.76f, 0.34f, 1.0f});

    mCommandBuffers[i].beginRenderPass(renderPassInfo,
                                       vk::SubpassContents::eInline);

    vk::DeviceSize offsets[] = {0};

    mCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                          mPipelineLayout, 0, 1,
                                          &mDescriptorSets[i], 0, nullptr);

    // Selection Points
    // if (!mBuffers[BufferName::selection_points].isEmpty) {
    //   mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics,
    //                                   pipelines.SketchPoint);
    //   mCommandBuffers[i].bindVertexBuffers(
    //       0, 1, &mBuffers[BufferName::selection_points].mBuffer, offsets);
    //   mCommandBuffers[i].draw(mBuffers[BufferName::selection_points].mPointSize,
    //                           1, 0, 0);
    // }

    cmd_begin_label(mCommandBuffers[i], "Draw Sketch Points", {1.0f, 1.0f, 1.0f, 1.0f});
    // Sketch Points
    if (!mBuffers[0].isEmpty) {
      mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics,
                                      pipelines.SketchPoint);
      mCommandBuffers[i].bindVertexBuffers(
          0, 1, &mBuffers[0].mBuffer, offsets);
      mCommandBuffers[i].draw(
          mBuffers[0].mPointSize + 2, 1, 0, 0);
    }
    cmd_end_label(mCommandBuffers[i]);
    // Sketch Tools
    // if (!mBuffers[BufferName::sketch_point_tool].isEmpty) {
    //   mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics,
    //                                   pipelines.SketchPoint);
    //   mCommandBuffers[i].bindVertexBuffers(
    //       0, 1, &mBuffers[BufferName::sketch_point_tool].mBuffer, offsets);
    //   mCommandBuffers[i].draw(
    //       mBuffers[BufferName::sketch_point_tool].mPointSize, 1, 0, 0);
    // }

    // Grid
    cmd_begin_label(mCommandBuffers[i], "Draw Sketch Grid", {0.0f, 0.0f, 0.0f, 1.0f});
    if (!mBuffers[1].isEmpty) {
      mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics,
                                      pipelines.SketchGrid);
      mCommandBuffers[i].bindVertexBuffers(
          0, 1, &mBuffers[1].mBuffer, offsets);
      mCommandBuffers[i].bindVertexBuffers(
          1, 1, &mBuffers[2].mBuffer, offsets);
      mCommandBuffers[i].draw(
          2, mBuffers[2].mPointSize, 0, 0);
    }
    cmd_end_label(mCommandBuffers[i]);

    // Text
    cmd_begin_label(mCommandBuffers[i], "Draw Sketch Text", {1.0f, 0.0f, 0.0f, 1.0f});
    if (!mBuffers[3].isEmpty) {
      mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics,
                                      mTextPipeline);
      mCommandBuffers[i].bindVertexBuffers(
          0, 1, &mBuffers[3].mBuffer, offsets);
      mCommandBuffers[i].bindIndexBuffer(
          mBuffers[4].mBuffer, 0,
          vk::IndexType::eUint32);
      mCommandBuffers[i].drawIndexed(
          mBuffers[4].mPointSize, 1, 0, 0, 0);
    }
    cmd_end_label(mCommandBuffers[i]);

    cmd_begin_label(mCommandBuffers[i], "Draw ImGui", {0.0f, 1.0f, 0.0f, 1.0f});
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), mCommandBuffers[i]);
    cmd_end_label(mCommandBuffers[i]);

    mCommandBuffers[i].endRenderPass();


    cmd_end_label(mCommandBuffers[i]);

    mCommandBuffers[i].end();
  }
}

void CADRender::updateUniformBuffer(uint32_t currentImage) {

  // u.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
  // glm::vec3(0.0f, 1.0f, 0.0f));
  //  u.view = glm::lookAt(Cam.pos, Cam.focus, glm::vec3(0.0f, 1.0f, 0.0f));
  u.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), Cam.up);
  u.view = glm::lookAt(Cam.pos, Cam.focus, Cam.up);

  if (Cam.flags.test(cam::ortho)) {

    float screenRatio = (float)mExtent.height / (float)mExtent.width;

    u.proj = glm::ortho(Cam.left, -Cam.left, screenRatio * Cam.left,
                        -screenRatio * Cam.left, 0.0f, 500.0f);
  } else {
    u.proj =
        glm::perspective(glm::radians(45.0f),
                         mExtent.width / (float)mExtent.height, 0.001f, 100.0f);
  }

  u.proj[1][1] *= -1;

  vk::MemoryMapFlags memMapFlags;

  void *data;
  data = mDevice.mapMemory(mUniformBufferMemories[currentImage], 0, sizeof(u),
                           memMapFlags);
  memcpy(data, &u, sizeof(u));
  mDevice.unmapMemory(mUniformBufferMemories[currentImage]);
}

void CADRender::drawFrame() {

  vk::Result result{};

  result = mDevice.waitForFences(mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

  if (result != vk::Result::eSuccess)
    throw("waitForFences failed! \n");
  

  uint32_t imageIndex = 0;

  vk::Fence F;

  result = mDevice.acquireNextImageKHR(
      mSwapchain, UINT64_MAX, mImageAvailableSemaphores[mCurrentFrame], F,
      &imageIndex);

  if (result == vk::Result::eErrorOutOfDateKHR) {
    recreateSwapchain();
    return;
  } else if (result != vk::Result::eSuccess &&
             result != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  updateUniformBuffer(imageIndex);

  vk::Semaphore waitSemaphore[] = {mImageAvailableSemaphores[mCurrentFrame]};
  vk::Semaphore signalSemaphore[] = {mRenderFinishedSemaphores[mCurrentFrame]};

  vk::PipelineStageFlags waitStages =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;

  vk::SubmitInfo submitInfo(1, waitSemaphore, &waitStages, 1,
                            &mCommandBuffers[imageIndex], 1, signalSemaphore);

  mDevice.resetFences(mInFlightFences[mCurrentFrame]);

  mGraphicsQueue.submit(submitInfo, mInFlightFences[mCurrentFrame]);

  vk::PresentInfoKHR presentInfo(1, signalSemaphore, 1, &mSwapchain,
                                 &imageIndex);

  result = mPresentQueue.presentKHR(presentInfo);

  if (result == vk::Result::eErrorOutOfDateKHR ||
      result == vk::Result::eSuboptimalKHR || frameBufferResized ||
      result == vk::Result::eErrorIncompatibleDisplayKHR) {
    frameBufferResized = false;
    recreateSwapchain();
  } else if (result != vk::Result::eSuccess) {
    // std::cout << "Present Queue Failed\n";
    throw std::runtime_error("failed to present swap chain image!");
  }

  mPresentQueue.waitIdle();

  mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void CADRender::destroyPipelines() {

  mDevice.destroyPipeline(pipelines.SketchPoint);
  mDevice.destroyPipeline(pipelines.SketchLine);
  mDevice.destroyPipeline(pipelines.SketchGrid);
  mDevice.destroyPipeline(mTextPipeline);
}

void CADRender::cleanupSwapchain() {

  for (auto &mFramebuffer : mFramebuffers)
    mDevice.destroyFramebuffer(mFramebuffer, nullptr);

  mDevice.destroyPipelineLayout(mPipelineLayout, nullptr);

  mDevice.destroyDescriptorSetLayout(mDescriptorSetLayout, nullptr);

  mDevice.destroyRenderPass(mRenderPass, nullptr);

  destroyPipelines();

  mDevice.destroyImage(depthImage);
  mDevice.freeMemory(depthImageMemory);
  mDevice.destroyImageView(depthImageView);

  for (auto &view : mImageViews)
    mDevice.destroyImageView(view, nullptr);

  mDevice.destroySwapchainKHR(mSwapchain, nullptr);
}

void CADRender::cleanup() {

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    mDevice.destroySemaphore(mImageAvailableSemaphores[i]);
    mDevice.destroySemaphore(mRenderFinishedSemaphores[i]);
    mDevice.destroyFence(mInFlightFences[i]);
  }

  destroyPipelines();

  for (auto &Buffer : mBuffers) {

    if (!Buffer.second.isEmpty) {
      mDevice.destroyBuffer(Buffer.second.mBuffer);
      mDevice.freeMemory(Buffer.second.mMemory);
    }
  }

  for (auto &framebuffer : mFramebuffers) {
    mDevice.destroyFramebuffer(framebuffer, nullptr);
  }

  for (auto &Buffer : mUniformBuffers) {
    mDevice.destroyBuffer(Buffer);
  }

  for (auto &memory : mUniformBufferMemories) {
    mDevice.freeMemory(memory);
  }

  mDevice.destroySampler(mTextureSampler);
  mDevice.destroyImageView(mTextureImageView);
  mDevice.destroyImage(mTextureImage);
  mDevice.freeMemory(mTextureMemory);

  mDevice.destroyImage(depthImage);
  mDevice.freeMemory(depthImageMemory);
  mDevice.destroyImageView(depthImageView);

  mDevice.destroyCommandPool(mCommandPool);
  mDevice.destroyDescriptorPool(mGuiDescriptorPool);
  mDevice.destroyDescriptorPool(mDescriptorPool);
  mDevice.destroyPipelineLayout(mPipelineLayout, nullptr);
  mDevice.destroyDescriptorSetLayout(mDescriptorSetLayout, nullptr);
  mDevice.destroy(mRenderPass, nullptr);

  for (auto &imageView : mImageViews) {
    mDevice.destroyImageView(imageView, nullptr);
  }

  mDevice.destroySwapchainKHR(mSwapchain, nullptr);

  mDevice.destroy();
  mInstance.destroySurfaceKHR(mImguiSurface);
  mInstance.destroySurfaceKHR(mSurface);
  mInstance.destroy();

  glfwTerminate();
}

void CADRender::destroy() {

  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  cleanup();
}

void CADRender::runCamera() {

  double x, y;
  glfwGetCursorPos(mMainWindow, &x, &y);
  Cam.updateMouseRay(static_cast<float>(x), static_cast<float>(y), u.model,
                     u.view, u.proj, mExtent.width, mExtent.height);

  if (Cam.flags.test(cam::pan)) {
    glfwGetCursorPos(mMainWindow, &Cam.xpos, &Cam.ypos);
    Cam.pan({0.0f, 0.0f, 0.0f}, glm::normalize(Cam.pos - Cam.focus));
  }

  Cam.update();
}

void CADRender::onNotify(int id, const RenderItems& renderables) {

  // Create Sketch Points buffers
  std::vector<Vertex> pointVertices;

  for (const auto& point : renderables.points) {
    pointVertices.push_back({point, {mRenderColors.sketchPointColor.x, 
                                mRenderColors.sketchPointColor.y, 
                                mRenderColors.sketchPointColor.z}});
  }

  
  if (!pointVertices.empty()) {
    updateBuffer(id, pointVertices,
                 vk::BufferUsageFlagBits::eVertexBuffer);
    setObjectName(mDevice, (uint64_t)(VkBuffer)mBuffers[id].mBuffer, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, "Sketch Points Buffer");
  } else if (!mBuffers[id].isEmpty) {
    deleteBuffer(id);
  }

  // Create Grid buffers
  std::vector<Vertex> gridVertices;
  
  std::vector<GridRotationAxis> axii;
  std::vector<glm::vec3> line;
  
  // Convert gridLine to vector of Vertex
  for (const auto& point : renderables.gridLine) {
    gridVertices.push_back({point, mRenderColors.sketchGridColor});
  }

  axii = renderables.gridAxii;

  if (!axii.empty() && !gridVertices.empty()) {
    updateBuffer(1, gridVertices,
                  vk::BufferUsageFlagBits::eVertexBuffer);
    updateBuffer(2, axii,
                  vk::BufferUsageFlagBits::eVertexBuffer);
  } else {
    deleteBuffer(1);
    deleteBuffer(2);
  }


  // Create Text buffers

  TxtRend.clearTexts();

  
  TxtRend.addText(renderables.texts);

  std::vector<txt::Vertex> txtVertices = TxtRend.generateQuads(mRenderColors.bgColor);
  std::vector<uint32_t> txtIndices = TxtRend.generateIndices();

  if (!txtVertices.empty()) {
    updateBuffer(3, txtVertices,
                 vk::BufferUsageFlagBits::eVertexBuffer);
    updateBuffer(4, txtIndices,
                 vk::BufferUsageFlagBits::eIndexBuffer);
  } else if (!mBuffers[3].isEmpty) {
    deleteBuffer(3);
    deleteBuffer(4);
  }

}

void CADRender::renderSketchNotes(Model &S) {

  TxtRend.clearTexts();

  for (const auto &N : S.Notes) {
    TxtRend.addText(N.second);
  }

  // Relation Symbols
  std::vector<txt::Text> RelationTexts;
  RelationTexts = S.getRelationTexts();

  for (const auto &T : RelationTexts) {
    TxtRend.addText(T);
  }

  std::vector<txt::Vertex> txtVertices = TxtRend.generateQuads(mRenderColors.bgColor);
  std::vector<uint32_t> txtIndices = TxtRend.generateIndices();

  // if (!txtVertices.empty()) {
  //   updateBuffer(BufferName::text_vertices, txtVertices,
  //                vk::BufferUsageFlagBits::eVertexBuffer);
  //   updateBuffer(BufferName::text_indices, txtIndices,
  //                vk::BufferUsageFlagBits::eIndexBuffer);
  // } else if (!mBuffers[BufferName::text_vertices].isEmpty) {
  //   deleteBuffer(BufferName::text_vertices);
  //   deleteBuffer(BufferName::text_indices);
  // }
}



} // namespace CADERA_APP_NAMESPACE
