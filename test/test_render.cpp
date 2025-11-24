#include "config.hpp"
#include "pch.hpp"
#include "render/cadrender.hpp"
#include <gtest/gtest.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

TEST(render, createInstance) {

  CADERA_APP_NAMESPACE::CADRender render;

  vk::Result result = render.createInstance();

  EXPECT_EQ(result, vk::Result::eSuccess);

  //vkDestroyInstance(render.mInstance, nullptr);
}

TEST(render, createWindow) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();

  EXPECT_NE(render.mMainWindow, nullptr);

  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createSurface) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();

  render.createSurface();

  EXPECT_NE(render.mSurface, nullptr);

  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, pickPhysicalDevice) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();

  render.pickPhysicalDevice();

  EXPECT_NE(render.mPhysicalDevice, nullptr);

  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createLogicalDevice) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();

  render.createLogicalDevice();

  EXPECT_NE(render.mDevice, nullptr);

  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createSwapChain) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();

  render.createSwapChain();

  EXPECT_NE(render.mSwapchain, nullptr);

  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createImageView) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();

  render.createSwapChain();

  vk::Image image = render.mImages[0];
  vk::Format format = vk::Format::eR8G8B8A8Srgb;
  vk::ImageAspectFlags aspectFlags = vk::ImageAspectFlagBits::eColor;
  vk::ImageView imageView = render.createImageView(image, format, aspectFlags);

  EXPECT_NE(imageView, nullptr);

  // vkDestroyImageView(render.mDevice, imageView, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createImageViews) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createImageViews();

  EXPECT_NE(render.mImageViews.size(), 0);

  // for (auto imageView : render.mImageViews) {
  //   vkDestroyImageView(render.mDevice, imageView, nullptr);
  // }

  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createRenderPass) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createRenderPass();

  EXPECT_NE(render.mRenderPass, nullptr);

  // vkDestroyRenderPass(render.mDevice, render.mRenderPass, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createDescriptorSetLayout) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createRenderPass();

  render.createDescriptorSetLayout();

  EXPECT_NE(render.mDescriptorSetLayout, nullptr);

  // vkDestroyDescriptorSetLayout(render.mDevice, render.mDescriptorSetLayout,
  //                              nullptr);
  // vkDestroyRenderPass(render.mDevice, render.mRenderPass, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createPipelineLayout) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createRenderPass();
  render.createDescriptorSetLayout();

  render.createPipelineLayout();

  EXPECT_NE(render.mPipelineLayout, nullptr);

  // vkDestroyPipelineLayout(render.mDevice, render.mPipelineLayout, nullptr);
  // vkDestroyDescriptorSetLayout(render.mDevice, render.mDescriptorSetLayout,
  //                              nullptr);
  // vkDestroyRenderPass(render.mDevice, render.mRenderPass, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, preparePipelines) {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createRenderPass();
  render.createDescriptorSetLayout();
  render.createPipelineLayout();

  render.preparePipelines();

  // vk::Pipeline pipeline = render.getSketchPointPipeline();
  // EXPECT_NE(pipeline, nullptr);

  // // vkDestroyPipeline(render.mDevice, render.Pipelines.SketchPoint, nullptr);
  // vkDestroyPipelineLayout(render.mDevice, render.mPipelineLayout, nullptr);
  // vkDestroyDescriptorSetLayout(render.mDevice, render.mDescriptorSetLayout,
  //                              nullptr);
  // vkDestroyRenderPass(render.mDevice, render.mRenderPass, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createCommandPool) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();

  render.createCommandPool();

  EXPECT_NE(render.mCommandPool, nullptr);

  // vkDestroyCommandPool(render.mDevice, render.mCommandPool, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createDepthResources) {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createDepthResources();

  EXPECT_NE(render.depthImage, nullptr);
  EXPECT_NE(render.depthImageMemory, nullptr);
  EXPECT_NE(render.depthImageView, nullptr);

  // vkDestroyImageView(render.mDevice, render.depthImageView, nullptr);
  // vkDestroyImage(render.mDevice, render.depthImage, nullptr);
  // vkFreeMemory(render.mDevice, render.depthImageMemory, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createFramebuffers) {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createImageViews();

  render.createRenderPass();

  render.createDepthResources();

  render.createFramebuffers();

  EXPECT_NE(render.mFramebuffers.size(), 0);

  // for (auto framebuffer : render.mFramebuffers) {
  //   vkDestroyFramebuffer(render.mDevice, framebuffer, nullptr);
  // }

  // vkDestroyImageView(render.mDevice, render.depthImageView, nullptr);
  // vkDestroyImage(render.mDevice, render.depthImage, nullptr);
  // vkFreeMemory(render.mDevice, render.depthImageMemory, nullptr);

  // vkDestroyRenderPass(render.mDevice, render.mRenderPass, nullptr);

  // for (auto imageView : render.mImageViews) {
  //   vkDestroyImageView(render.mDevice, imageView, nullptr);
  // }

  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createUniformBuffer) {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createUniformBuffer();

  EXPECT_NE(render.mUniformBuffers.size(), 0);
  EXPECT_NE(render.mUniformBufferMemories.size(), 0);

  // for (auto buffer : render.mUniformBuffers) {
  //   vkDestroyBuffer(render.mDevice, buffer, nullptr);
  // }

  // for (auto memory : render.mUniformBufferMemories) {
  //   vkFreeMemory(render.mDevice, memory, nullptr);
  // }

  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createDescriptorPool) {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createDescriptorSetLayout();

  render.createDescriptorPool();


  EXPECT_NE(render.mDescriptorPool, nullptr);

  // vkDestroyDescriptorPool(render.mDevice, render.mDescriptorPool, nullptr);

  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, createDescriptorSets) {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createDescriptorSetLayout();
  
  render.createCommandPool();
  
  render.createUniformBuffer();

  render.createTextureImage();
  render.createTextureImageView();
  render.createTextureSampler();

  render.createDescriptorPool();

  render.createDescriptorSets();

  EXPECT_NE(render.mDescriptorSets.size(), 0);

  // vkDestroyDescriptorPool(render.mDevice, render.mDescriptorPool, nullptr);

  // for (auto buffer : render.mUniformBuffers) {
  //   vkDestroyBuffer(render.mDevice, buffer, nullptr);
  // }

  // for (auto memory : render.mUniformBufferMemories) {
  //   vkFreeMemory(render.mDevice, memory, nullptr);
  // }

  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();
}

TEST(render, allocCommandBuffers) {

  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  render.createSwapChain();

  render.createImageViews();

  render.createRenderPass();
  render.createCommandPool();
  render.createDepthResources();

  render.createFramebuffers();


  render.allocCommandBuffers();

  EXPECT_EQ(render.mCommandBuffers.size(), render.mFramebuffers.size());

  // for (auto framebuffer : render.mFramebuffers) {
  //   vkDestroyFramebuffer(render.mDevice, framebuffer, nullptr);
  // }

  // vkDestroyImageView(render.mDevice, render.depthImageView, nullptr);
  // vkDestroyImage(render.mDevice, render.depthImage, nullptr);
  // vkFreeMemory(render.mDevice, render.depthImageMemory, nullptr);

  // vkDestroyRenderPass(render.mDevice, render.mRenderPass, nullptr);

  // for (auto imageView : render.mImageViews) {
  //   vkDestroyImageView(render.mDevice, imageView, nullptr);
  // }

  // vkDestroyCommandPool(render.mDevice, render.mCommandPool, nullptr);
  // vkDestroySwapchainKHR(render.mDevice, render.mSwapchain, nullptr);
  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();

}

TEST(render, createSyncObjects) {


  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  CADERA_APP_NAMESPACE::CADRender render;

  render.createWindow();
  render.createInstance();
  render.createSurface();
  render.pickPhysicalDevice();
  render.createLogicalDevice();
  
  render.createSyncObjects();

  for (int i = 0; i < 2; i++) {
    render.mDevice.destroySemaphore(render.mImageAvailableSemaphores[i]);
    render.mDevice.destroySemaphore(render.mRenderFinishedSemaphores[i]);
    render.mDevice.destroyFence(render.mInFlightFences[i]);
  }

  // vkDestroyDevice(render.mDevice, nullptr);
  // vkDestroySurfaceKHR(render.mInstance, render.mSurface, nullptr);
  // vkDestroyInstance(render.mInstance, nullptr);
  glfwDestroyWindow(render.mMainWindow);
  glfwTerminate();

}