#include "disassemble.hpp"
#include "error.hpp"

std::array<disassembler_globals::AnyTuple, 2> disassembler::opmap;

/*
std::string disassembler::get_file_contents() {
	std::stringstream ss_file;
	ss_file << ifsfile.rdbuf();
	return ss_file.str();
}
*/

void disassembler::init_array() {
	opmap = {{{0x00, "NOP", []() {
		cpu_instructions::nop();
		return true;
	}}}};
		/*
		 * All MOV instructions
		 */
	      /*		{0x78, "MOV A, B"}, {0x79, "MOV A, C"},
		{0x7A, "MOV A, D"},
		{0x7B, "MOV A, E"},
		{0x7C, "MOV A, H"},
		{0x7D, "MOV A, L"},
		{0x7E, "MOV A, M"},
		{0x7F, "MOV A, A"},
		{0x47, "MOV B, A"},
		{0x40, "MOV B, B"},
		{0x41, "MOV B, C"},
		{0x42, "MOV B, D"},
		{0x43, "MOV B, E"},
		{0x44, "MOV B, H"},
		{0x45, "MOV B, L"},
		{0x46, "MOV B, M"},
		{0x4F, "MOV C, A"},
		{0x48, "MOV C, B"},
		{0x49, "MOV C, C"},
		{0x4A, "MOV C, D"},
		{0x4B, "MOV C, E"},
		{0x4C, "MOV C, H"},
		{0x4D, "MOV C, L"},
		{0x4E, "MOV C, M"},
		{0x57, "MOV D, A"},
		{0x50, "MOV D, B"},
		{0x51, "MOV D, C"},
		{0x52, "MOV D, D"},
		{0x53, "MOV D, E"},
		{0x54, "MOV D, H"},
		{0x55, "MOV D, L"},
		{0x56, "MOV D, M"},
		{0x5F, "MOV E, A"},
		{0x58, "MOV E, B"},
		{0x59, "MOV E, C"},
		{0x5A, "MOV E, D"},
		{0x5B, "MOV E, E"},
		{0x5C, "MOV E, H"},
		{0x5D, "MOV E, L"},
		{0x5E, "MOV E, M"},
		{0x67, "MOV H, A"},
		{0x60, "MOV H, B"},
		{0x61, "MOV H, C"},
		{0x62, "MOV H, D"},
		{0x63, "MOV H, E"},
		{0x64, "MOV H, H"},
		{0x65, "MOV H, L"},
		{0x66, "MOV H, M"},
		{0x6F, "MOV L, A"},
		{0x68, "MOV L, B"},
		{0x69, "MOV L, C"},
		{0x6A, "MOV L, D"},
		{0x6B, "MOV L, E"},
		{0x6C, "MOV L, H"},
		{0x6D, "MOV L, L"},
		{0x6E, "MOV L, M"},
		{0x7A, "MOV A, D"},
		{0x7B, "MOV A, E"},
		{0x7C, "MOV A, H"},
		{0x7D, "MOV A, L"},
		{0x7E, "MOV A, M"},
		{0x7F, "MOV A, A"},
		{0x47, "MOV B, A"},
		{0x40, "MOV B, B"},
		{0x41, "MOV B, C"},
		{0x42, "MOV B, D"},
		{0x43, "MOV B, E"},
		{0x44, "MOV B, H"},
		{0x45, "MOV B, L"},
		{0x46, "MOV B, M"},
		{0x4F, "MOV C, A"},
		{0x48, "MOV C, B"},
		{0x49, "MOV C, C"},
		{0x4A, "MOV C, D"},
		{0x4B, "MOV C, E"},
		{0x4C, "MOV C, H"},
		{0x4D, "MOV C, L"},
		{0x4E, "MOV C, M"},
		{0x57, "MOV D, A"},
		{0x50, "MOV D, B"},
		{0x51, "MOV D, C"},
		{0x52, "MOV D, D"},
		{0x53, "MOV D, E"},
		{0x54, "MOV D, H"},
		{0x55, "MOV D, L"},
		{0x56, "MOV D, M"},
		{0x5F, "MOV E, A"},
		{0x58, "MOV E, B"},
		{0x59, "MOV E, C"},
		{0x5A, "MOV E, D"},
		{0x5B, "MOV E, E"},
		{0x5C, "MOV E, H"},
		{0x5D, "MOV E, L"},
		{0x5E, "MOV E, M"},
		{0x67, "MOV H, A"},
		{0x60, "MOV H, B"},
		{0x61, "MOV H, C"},
		{0x62, "MOV H, D"},
		{0x63, "MOV H, E"},
		{0x64, "MOV H, H"},
		{0x65, "MOV H, L"},
		{0x66, "MOV H, M"},
		{0x6F, "MOV L, A"},
		{0x68, "MOV L, B"},
		{0x69, "MOV L, C"},
		{0x6A, "MOV L, D"},
		{0x6B, "MOV L, E"},
		{0x6C, "MOV L, H"},
		{0x6D, "MOV L, L"},
		{0x6E, "MOV L, M"},
		{0x77, "MOV M, A"},
		{0x70, "MOV M, B"},
		{0x71, "MOV M, C"},
		{0x72, "MOV M, D"},
		{0x73, "MOV M, E"},
		{0x74, "MOV M, H"}};
	*/
}
disassembler_globals::AnyTuple disassembler::find_opcode(const uint8_t &opcode) {
	auto val = std::find_if(opmap.begin(), opmap.end(), [&](auto &pair) {
		return std::get<0>(pair) == opcode;
			});
	if(val == opmap.end())
		return {};
	return *val;
}

int get_digits(std::string &opcode) { 
	try {
		return std::stoi(opcode);
	} catch(std::invalid_argument&) {
		exception::invalid_asm();
	}
	return -1; // making the compiler stop whining
}

std::vector<disassembler_globals::AnyTuple> disassembler::disassemble() {
	std::string line;
	std::vector<disassembler_globals::AnyTuple> tuple_instructions{};
	while(std::getline(ifsfile, line)) {
		int opcode = get_digits(line);
		auto tuple_inst = find_opcode(opcode);
		tuple_instructions.push_back(tuple_inst);
	}
	return tuple_instructions;
}
