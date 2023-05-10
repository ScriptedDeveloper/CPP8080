#include "error.hpp"

void exception::invalid_format() {
	std::cerr << "Error! Invalid format." << std::endl;
	std::exit(1);
}
