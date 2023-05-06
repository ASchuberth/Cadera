#include "pch.hpp"
#include "Cadera.hpp"
#include <gsl/gsl>


#define CADERA_APP_NAMESPACE cad

/// @brief  main function
/// @return 0
int main() {



	//cad::Cadera App;
	std::vector<int> x = {1};
	glm::vec3 v = {0.0f, 1.0f, 10.0f};
	try {
		//App.run();

	}
	catch (const std::exception & err) {
		std::cerr << err.what() << std::endl;
	}



	return 0;
}