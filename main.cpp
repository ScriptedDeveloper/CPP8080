#include "cpu/cpu.hpp"
#include "cpu/isr.hpp"
#include "disassemble/disassemble.hpp"
#include "error/error.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Error! No file specified." << std::endl << argv[0] << " <file>" << std::endl;
		std::cout << "OR : Perform a Hello print test! : " << argv[0] << " --test" << std::endl;
		std::exit(1);
	}
	std::string param{argv[1]};
	if (param == "--test") {
		param = std::filesystem::current_path().generic_string() + "/Tests/Hello.com";
	}
	disassembler d(param);
	auto disassembled_vec = d.disassemble();
	cpu_handler c(disassembled_vec);
	if (!c.handle_instructions()) {
		int temp{};
		exception::invalid_asm(temp);
	}
	return 0;
}
