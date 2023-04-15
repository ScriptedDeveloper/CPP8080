#include "disassemble.hpp"

std::array<disassembler_globals::AnyTuple, 3> disassembler::opmap;

/*
std::string disassembler::get_file_contents() {
	std::stringstream ss_file;
	ss_file << ifsfile.rdbuf();
	return ss_file.str();
}
*/

void disassembler::init_array() {
	using namespace cpu_instructions;
	opmap = {
		{
			{ 0x00, "nop", [](){ nop(); } },
			{ 0x78, "MOV, A, B", [](){ mov(cpu_handler::A, cpu_handler::B); } },
		}
	};
	      /*
	      {0x78, "MOV A, B", []() {
					      cpu_instructions::mov(cpu_handler::A, cpu_handler::B);
		}}, 
*/
		/*
		 * All MOV instructions
		 */
		      /*
	       {0x79, "MOV A, C"},
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
disassembler_globals::AnyTuple disassembler::find_instruction(const uint8_t &opcode) {
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

bool disassembler::correct_opcode(std::vector<disassembler_globals::AnyTuple> &tuple_instructions, uint8_t &current_opcode) {
	auto tuple_inst = find_instruction(current_opcode);
	if(std::tuple_size<decltype(tuple_inst)>::value == 0)
		return false;
	else {
		/*
		 * It worked so lets push the tuple onto the vector
		 */
		tuple_instructions.push_back(tuple_inst);
		current_opcode = 0;
	}
	return true;
}

std::vector<disassembler_globals::AnyTuple> disassembler::disassemble() { 
	int address{};
	int current_opcode{}; // using uint8_t is gave me a real headache 
	short addr_count{};
	std::stringstream ss; // for hex
	std::vector<disassembler_globals::AnyTuple> tuple_instructions{};
	bool failed = false; // for checking if instruction 1 byte find operation failed
	for(char ch_int : machine_code) {
		if(ch_int == '\n' || ch_int == '\0')
			continue; // skipping
		if(std::isalpha(ch_int))
			exception::invalid_asm(); // aint no letters in machine code
		ss << std::hex << ch_int;
		int digit = char_to_hex(ch_int);
		std::cout << digit << std::endl;
		if(addr_count < 4) {
			addr_count++;
			continue;
		}
		bool is_nop = current_opcode == 0 && digit == 0; // nop is very special since it has two zeros, so this is how I represent it
		current_opcode = add_digits(current_opcode, digit);
		if((current_opcode > 0xA && current_opcode <= 0xAF) || is_nop) { // first looking for a 1 byte instruction
			auto temp_int = static_cast<uint8_t>(current_opcode);
			failed = !correct_opcode(tuple_instructions, temp_int);
			addr_count = (failed) ? addr_count : 0;
			current_opcode = 0;
			address++;
		}
		failed = false;
	}
	return tuple_instructions;
}
