#include "cpu.hpp"
#include "disassemble.hpp"
#include "error.h"
#include "error.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Error! No file specified." << std::endl;
		std::exit(1);
	}
	disassembler d(argv[1]);
	auto disassembled_vec = d.disassemble();
	std::vector<uint8_t> opcodes{};
	cpu_handler c(disassembled_vec, opcodes);
	if (!c.handle_instructions())
		exception::invalid_asm();
	;
	return 0;
}
