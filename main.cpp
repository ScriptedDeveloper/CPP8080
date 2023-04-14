#include "disassemble.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
	if(argc < 2) {
		std::cerr << "Error! No file specified." << std::endl;
		std::exit(1);
	}
	disassembler d(argv[1]);
	std::string asm_code = d.disassemble();
	
	return 0;
}
