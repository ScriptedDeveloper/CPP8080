#pragma once
#include <iostream>

namespace exception {
void invalid_format();
template <typename T> void invalid_asm(T &opcode) {
	std::cerr << "Error! Invalid format." << std::endl << "Opcode : " << opcode << std::endl;
	std::exit(1);
}
}; // namespace exception
