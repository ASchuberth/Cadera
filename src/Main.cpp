#include "pch.hpp"
#include "Cadera.hpp"
#include <gsl/gsl>
#include <CADRender.hpp>


VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE


/// @brief  main function
/// @return 0
int main() {

	const char* a = "VK_LAYER_LUNARG_standard_validation";
	//std::vector<const char*> b = {a};
	
	cad::Cadera App;
	
	try {
		App.run();

	}
	catch (const std::exception & err) {
		std::cerr << err.what() << std::endl;
	}

	std::cout << "Hello\n";


	return 0;
}