#include "pch.hpp"
#include "Canvas.hpp"

namespace CADERA_APP_NAMESPACE {


	void Canvas::destroy(const vk::Device& Device) {

		

	}

	bool Canvas::checkFormat(vk::Format Format)
	{
		return mFormat == Format;
	}

	







	bool Canvas::hasStencilComponent(vk::Format format) {

		return format == vk::Format::eD32Sfloat || format == vk::Format::eD24UnormS8Uint;
	}

	



}
