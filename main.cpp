#include "cpu/cpu.hpp"
#include "disassemble/disassemble.hpp"
#include "error/error.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Error! No file specified." << std::endl;
		std::exit(1);
	}
	disassembler d(argv[1]);
	auto disassembled_vec = d.disassemble();
	cpu_handler c(disassembled_vec);
	if (!c.handle_instructions())
		exception::invalid_asm();
	;
	return 0;
}
