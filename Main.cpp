#include "pch.hpp"
#include "Cadera.hpp"

int main() {
	Cadera App;

	try {		
		App.run();
	}
	catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
	}



	return 0;
}