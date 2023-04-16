#include "disassemble.hpp"
#include "cpu.hpp"

std::unique_ptr<std::unordered_map<uint8_t, disassembler_globals::AnyTuple>> disassembler::opmap;

/*
std::string disassembler::get_file_contents() {
	std::stringstream ss_file;
	ss_file << ifsfile.rdbuf();
	return ss_file.str();
}
*/

void disassembler::init_array() {
	using namespace cpu_instructions;
	opmap = std::make_unique<std::unordered_map<uint8_t, disassembler_globals::AnyTuple>>(
		std::unordered_map<uint8_t, disassembler_globals::AnyTuple>{
			{0x40, {"MOV B,B", []() { mov(cpu_handler::B, cpu_handler::B); }}},
			{0x41, {"MOV B,C", []() { mov(cpu_handler::B, cpu_handler::C); }}},
			{0x42, {"MOV B,D", []() { mov(cpu_handler::B, cpu_handler::D); }}},
			{0x43, {"MOV B,E", []() { mov(cpu_handler::B, cpu_handler::E); }}},
			{0x44, {"MOV B,H", []() { mov(cpu_handler::B, cpu_handler::H); }}},
			{0x45, {"MOV B,L", []() { mov(cpu_handler::B, cpu_handler::L); }}},
			{0x47, {"MOV B,A", []() { mov(cpu_handler::B, cpu_handler::A); }}},
			{0x48, {"MOV C,B", []() { mov(cpu_handler::C, cpu_handler::B); }}},
			{0x49, {"MOV C,C", []() { mov(cpu_handler::C, cpu_handler::C); }}},
			{0x4A, {"MOV C,D", []() { mov(cpu_handler::C, cpu_handler::D); }}},
			{0x4B, {"MOV C,E", []() { mov(cpu_handler::C, cpu_handler::E); }}},
			{0x4C, {"MOV C,H", []() { mov(cpu_handler::C, cpu_handler::H); }}},
			{0x4D, {"MOV C,L", []() { mov(cpu_handler::C, cpu_handler::L); }}},
			{0x4F, {"MOV C,A", []() { mov(cpu_handler::C, cpu_handler::A); }}},
			{0x50, {"MOV D,B", []() { mov(cpu_handler::D, cpu_handler::B); }}},
			{0x51, {"MOV D,C", []() { mov(cpu_handler::D, cpu_handler::C); }}},
			{0x52, {"MOV D,D", []() { mov(cpu_handler::D, cpu_handler::D); }}},
			{0x53, {"MOV D,E", []() { mov(cpu_handler::D, cpu_handler::E); }}},
			{0x54, {"MOV D,H", []() { mov(cpu_handler::D, cpu_handler::H); }}},
			{0x55, {"MOV D,L", []() { mov(cpu_handler::D, cpu_handler::L); }}},
			{0x57, {"MOV D,A", []() { mov(cpu_handler::D, cpu_handler::A); }}},
			{0x58, {"MOV E,B", []() { mov(cpu_handler::E, cpu_handler::B); }}},
			{0x59, {"MOV E,C", []() { mov(cpu_handler::E, cpu_handler::C); }}},
			{0x5A, {"MOV E,D", []() { mov(cpu_handler::E, cpu_handler::D); }}},
			{0x5B, {"MOV E,E", []() { mov(cpu_handler::E, cpu_handler::E); }}},
			{0x5C, {"MOV E,H", []() { mov(cpu_handler::E, cpu_handler::H); }}},
			{0x5D, {"MOV E,L", []() { mov(cpu_handler::E, cpu_handler::L); }}},
			{0x5F, {"MOV E,A", []() { mov(cpu_handler::E, cpu_handler::A); }}},
			{0x60, {"MOV H,B", []() { mov(cpu_handler::H, cpu_handler::B); }}},
			{0x61, {"MOV H,C", []() { mov(cpu_handler::H, cpu_handler::C); }}},
			{0x62, {"MOV H,D", []() { mov(cpu_handler::H, cpu_handler::D); }}},
			{0x63, {"MOV H,E", []() { mov(cpu_handler::H, cpu_handler::E); }}},
			{0x64, {"MOV H,H", []() { mov(cpu_handler::H, cpu_handler::H); }}},
			{0x65, {"MOV H,L", []() { mov(cpu_handler::H, cpu_handler::L); }}},
			{0x67, {"MOV H,A", []() { mov(cpu_handler::H, cpu_handler::A); }}},
			{0x68, {"MOV L,B", []() { mov(cpu_handler::L, cpu_handler::B); }}},
			{0x69, {"MOV L,C", []() { mov(cpu_handler::L, cpu_handler::C); }}},
			{0x6A, {"MOV L,D", []() { mov(cpu_handler::L, cpu_handler::D); }}},
			{0x6B, {"MOV L,E", []() { mov(cpu_handler::L, cpu_handler::E); }}},
			{0x6C, {"MOV L,H", []() { mov(cpu_handler::L, cpu_handler::H); }}},
			{0x6D, {"MOV L,L", []() { mov(cpu_handler::L, cpu_handler::L); }}},
			{0x6F, {"MOV L,A", []() { mov(cpu_handler::L, cpu_handler::A); }}},
			{0x78, {"MOV A,B", []() { mov(cpu_handler::A, cpu_handler::B); }}},
			{0x79, {"MOV A,C", []() { mov(cpu_handler::A, cpu_handler::C); }}},
			{0x7A, {"MOV A,D", []() { mov(cpu_handler::A, cpu_handler::D); }}},
			{0x7B, {"MOV A,E", []() { mov(cpu_handler::A, cpu_handler::E); }}},
			{0x7C, {"MOV A,H", []() { mov(cpu_handler::A, cpu_handler::H); }}},
			{0x7D, {"MOV A,L", []() { mov(cpu_handler::A, cpu_handler::L); }}},
			{0x7F, {"MOV A,A", []() { mov(cpu_handler::A, cpu_handler::A); }}},
		});
}
disassembler_globals::AnyTuple disassembler::find_instruction(const uint8_t &opcode) {
	auto val = opmap->find(opcode);
	if (val == opmap->end())
		return {};
	return val->second;
}

int get_digits(std::string &opcode) {
	try {
		return std::stoi(opcode);
	} catch (std::invalid_argument &) {
		exception::invalid_asm();
	}
	return -1; // making the compiler stop whining
}

bool disassembler::correct_opcode(std::vector<disassembler_globals::AnyTuple> &tuple_instructions,
								  uint8_t &current_opcode) {
	auto tuple_inst = find_instruction(current_opcode);
	if (std::tuple_size<decltype(tuple_inst)>::value == 0)
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
	for (char ch_int : machine_code) {
		if (ch_int == '\n' || ch_int == '\0')
			continue; // skipping
		if (std::isalpha(ch_int) && !std::isxdigit(ch_int))
			exception::invalid_asm(); // aint no letters in machine code
		ss << std::hex << ch_int;
		int digit = char_to_hex(ch_int);
		if (addr_count < 4) {
			addr_count++;
			continue;
		}
		bool is_nop = current_opcode == 0 &&
					  digit == 0; // nop is very special since it has two zeros, so this is how I represent it
		current_opcode = add_digits(current_opcode, digit);
		if ((current_opcode > 0xA && current_opcode <= 0xAF) || is_nop) { // first looking for a 1 byte instruction
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
