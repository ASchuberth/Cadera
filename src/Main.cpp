#include "pch.hpp"
#include "Cadera.hpp"
#include <gsl/gsl>



VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE


/// @brief  main function
/// @return 0
int main() {

	
	cad::Cadera App;
	
	try {
		App.run();

	}
	catch (const std::exception & err) {
		std::cerr << err.what() << std::endl;
	}


	return 0;
}