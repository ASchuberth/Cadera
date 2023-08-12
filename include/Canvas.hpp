#pragma once



namespace CADERA_APP_NAMESPACE {


	class Canvas {

	protected:

		

	public:

		bool frameBufferResized;

		
		
		
	
		std::vector<vk::Framebuffer> mFramebuffers;

		

		void destroy(const vk::Device& Device);

		bool checkFormat(vk::Format Format);

	
		bool hasStencilComponent(vk::Format format);


	};

}