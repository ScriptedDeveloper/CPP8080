#include "error.hpp"

void exception::invalid_asm() {
	std::cerr << "Error! Invalid Assembly instruction."
			  << std::endl; // will provide more error logs l8ter, rn focusing on main functionality
	std::exit(1);
}
