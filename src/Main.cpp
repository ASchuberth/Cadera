#include "pch.hpp"
#include "Cadera.hpp"
#include <gsl/gsl>



#define CADERA_APP_NAMESPACE cad

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