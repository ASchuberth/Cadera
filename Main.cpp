#include "pch.hpp"
#include "Cadera.hpp"


#define CADERA_APP_NAMESPACE cad

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