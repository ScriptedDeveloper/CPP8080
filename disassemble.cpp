#include "disassemble.hpp"
#include "cpu.hpp"
#include <arpa/inet.h>
#include <bit>
#include <codecvt>
#include <cstdint>

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
	// clang-format off
	opmap = std::make_unique<std::unordered_map<
		uint8_t, disassembler_globals::
		 AnyTuple>>(std::
		unordered_map<uint8_t, disassembler_globals::AnyTuple>{
		{0x00, {"NOP", [](uint8_t val = 0) { nop(); }, 1, 0.0}},
		{0x40, {"B,B", [](uint8_t val = 0) { mov(memory::B, memory::B); }, 1, 0.0}},
		{0x41, {"B,C", [](uint8_t val = 0) { mov(memory::B, memory::C); }, 1, 0.0}},
		{0x42, {"B,D", [](uint8_t val = 0) { mov(memory::B, memory::D); }, 1, 0.0}},
		{0x43, {"B,E", [](uint8_t val = 0) { mov(memory::B, memory::E); }, 1, 0.0}},
		{0x44, {"B,H", [](uint8_t val = 0) { mov(memory::B, memory::H); }, 1, 0.0}},
		{0x45, {"B,L", [](uint8_t val = 0) { mov(memory::B, memory::L); }, 1, 0.0}},
		{0x47, {"B,A", [](uint8_t val = 0) { mov(memory::B, memory::A); }, 1, 0.0}},
		{0x48, {"C,B", [](uint8_t val = 0) { mov(memory::C, memory::B); }, 1, 0.0}},
		{0x49, {"C,C", [](uint8_t val = 0) { mov(memory::C, memory::C); }, 1, 0.0}},
		{0x4A, {"C,D", [](uint8_t val = 0) { mov(memory::C, memory::D); }, 1, 0.0}},
		{0x4B, {"C,E", [](uint8_t val = 0) { mov(memory::C, memory::E); }, 1, 0.0}},
		{0x4C, {"C,H", [](uint8_t val = 0) { mov(memory::C, memory::H); }, 1, 0.0}},
		{0x4D, {"C,L", [](uint8_t val = 0) { mov(memory::C, memory::L); }, 1, 0.0}},
		{0x4F, {"C,A", [](uint8_t val = 0) { mov(memory::C, memory::A); }, 1, 0.0}},
		{0x50, {"D,B", [](uint8_t val = 0) { mov(memory::D, memory::B); }, 1, 0.0}},
		{0x51, {"D,C", [](uint8_t val = 0) { mov(memory::D, memory::C); }, 1, 0.0}},
		{0x52, {"D,D", [](uint8_t val = 0) { mov(memory::D, memory::D); }, 1, 0.0}},
		{0x53, {"D,E", [](uint8_t val = 0) { mov(memory::D, memory::E); }, 1, 0.0}},
		{0x54, {"D,H", [](uint8_t val = 0) { mov(memory::D, memory::H); }, 1, 0.0}},
		{0x55, {"D,L", [](uint8_t val = 0) { mov(memory::D, memory::L); }, 1, 0.0}},
		{0x57, {"D,A", [](uint8_t val = 0) { mov(memory::D, memory::A); }, 1, 0.0}},
		{0x58, {"E,B", [](uint8_t val = 0) { mov(memory::E, memory::B); }, 1, 0.0}},
		{0x59, {"E,C", [](uint8_t val = 0) { mov(memory::E, memory::C); }, 1, 0.0}},
		{0x5A, {"E,D", [](uint8_t val = 0) { mov(memory::E, memory::D); }, 1, 0.0}},
		{0x5B, {"E,E", [](uint8_t val = 0) { mov(memory::E, memory::E); }, 1, 0.0}},
		{0x5C, {"E,H", [](uint8_t val = 0) { mov(memory::E, memory::H); }, 1, 0.0}},
		{0x5D, {"E,L", [](uint8_t val = 0) { mov(memory::E, memory::L); }, 1, 0.0}},
		{0x5F, {"E,A", [](uint8_t val = 0) { mov(memory::E, memory::A); }, 1, 0.0}},
		{0x60, {"H,B", [](uint8_t val = 0) { mov(memory::H, memory::B); }, 1, 0.0}},
		{0x61, {"H,C", [](uint8_t val = 0) { mov(memory::H, memory::C); }, 1, 0.0}},
		{0x62, {"H,D", [](uint8_t val = 0) { mov(memory::H, memory::D); }, 1, 0.0}},
		{0x63, {"H,E", [](uint8_t val = 0) { mov(memory::H, memory::E); }, 1, 0.0}},
		{0x64, {"H,H", [](uint8_t val = 0) { mov(memory::H, memory::H); }, 1, 0.0}},
		{0x65, {"H,L", [](uint8_t val = 0) { mov(memory::H, memory::L); }, 1, 0.0}},
		{0x67, {"H,A", [](uint8_t val = 0) { mov(memory::H, memory::A); }, 1, 0.0}},
		{0x68, {"L,B", [](uint8_t val = 0) { mov(memory::L, memory::B); }, 1, 0.0}},
		{0x69, {"L,C", [](uint8_t val = 0) { mov(memory::L, memory::C); }, 1, 0.0}},
		{0x6A, {"L,D", [](uint8_t val = 0) { mov(memory::L, memory::D); }, 1, 0.0}},
		{0x6B, {"L,E", [](uint8_t val = 0) { mov(memory::L, memory::E); }, 1, 0.0}},
		{0x6C, {"L,H", [](uint8_t val = 0) { mov(memory::L, memory::H); }, 1, 0.0}},
		{0x6D, {"L,L", [](uint8_t val = 0) { mov(memory::L, memory::L); }, 1, 0.0}},
		{0x6F, {"L,A", [](uint8_t val = 0) { mov(memory::L, memory::A); }, 1, 0.0}},
		{0x78, {"A,B", [](uint8_t val = 0) { mov(memory::A, memory::B); }, 1, 0.0}},
		{0x79, {"A,C", [](uint8_t val = 0) { mov(memory::A, memory::C); }, 1, 0.0}},
		{0x7A, {"A,D", [](uint8_t val = 0) { mov(memory::A, memory::D); }, 1, 0.0}},
		{0x7B, {"A,E", [](uint8_t val = 0) { mov(memory::A, memory::E); }, 1, 0.0}},
		{0x7C, {"A,H", [&](uint8_t val = 0) { mov(memory::A, memory::H); }, 1, 0.0}},
		{0x7D, {"A,L", [](uint8_t val = 0) { mov(memory::A, memory::L); }, 1, 0.0}},
		{0x7F, {"A,A", [](uint8_t val = 0) { mov(memory::A, memory::A); }, 1, 0.0}},
		{0x3E, {"A", [](uint8_t val) { mvi(val, memory::A); }, 0, 1.0}},
		{0x06, {"B", [](uint8_t val) { mvi(val, memory::B); }, 0, 1.0}},
		{0x0E, {"C", [](uint8_t val) { mvi(val, memory::C); }, 0, 1.0}},
		{0x16, {"D", [](uint8_t val) { mvi(val, memory::D); }, 0, 1.0}},
		{0x1E, {"E", [](uint8_t val) { mvi(val, memory::E); }, 0, 1.0}},
		{0x26, {"H", [](uint8_t val) { mvi(val, memory::H); }, 0, 1.0}},
		{0x2E, {"L", [](uint8_t val) { mvi(val, memory::L); }, 0, 1.0}},
		{0xC5, {"B,B", [](uint8_t val = 0) { // double B, marked as no need for params (pushed onto stack)
			cpu_instructions::push(memory::B);
			},
		1, 0.0}},
		{0xD5, {"D,D", [](uint8_t val = 0) {
			cpu_instructions::push(memory::D);
			},
		1, 0.0}},	
		{0xE5, {"H,H", [](uint8_t val = 0) {
			cpu_instructions::push(memory::H);
			},
		1, 0.0}},
		{0xC1, {"B,B", [](uint8_t val = 0) {
			cpu_instructions::pop(memory::B);
			},
		1, 0.0}},	
		{0xD1, {"D,D", [](uint8_t val = 0) {
			cpu_instructions::pop(memory::D);
			},
		1, 0.0}},	
		{0xD2, {"H,H", [](uint8_t val = 0) {
			cpu_instructions::pop(memory::H);
			},
		1, 0.0}},
		{0xC3, {"X,X", [](uint8_t val) {
			cpu_instructions::jmp(val);
			},
		0, 2.0}},
		{0xCD, {"X,X", [](uint8_t val) {
			cpu_instructions::call(val);
			},
		0, 2.0}},	
		{0xC9, {"X", [](uint8_t val = 0) {
			cpu_instructions::ret();
			},
		0, 0.0}},

	});
	// clang-format on
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
								  uint8_t &current_opcode, int param) {
	auto tuple_inst = find_instruction(current_opcode);
	if (std::get<1>(tuple_inst) == nullptr || (std::get<0>(tuple_inst).size() == 1 && param == UINT16_MAX + 1))
		return false;
	else {
		/*
		 * It worked so lets push the tuple onto the vector
		 */
		std::get<2>(tuple_inst) = (param == UINT16_MAX + 1) ? 0 : param;
		int test = std::get<2>(tuple_inst);
		test--;
		tuple_instructions.push_back(tuple_inst);
		current_opcode = 0;
	}
	return true; // also can indicate by return val if it has more than 1 byte instruction and pass the value like that
				 // inside a tuple
}

uint8_t disassembler::finish_instruction(int &current_opcode, short &addr_count, std::vector<uint8_t> &last_param,
										 int &param, int &address, disassembler_globals::AnyTuple &tuple) {
	current_opcode = 0;
	param = 0;
	addr_count = 0;
	address += (std::get<3>(tuple) + 1);
	last_param.clear();
	return address;
}

void disassembler::add_last_param(double &i_instruction_find, std::vector<uint8_t> &last_param, short &zero_count,
								  int current_opcode, int param) {
	// i_instruction_find += 0.5;
	last_param.clear();
	zero_count = 0;
	last_param.push_back(current_opcode);
	last_param.push_back(param);
}

bool disassembler::validate_opcode(int &current_opcode, double &i_instruction_max, double &i_instruction_find,
								   std::vector<disassembler_globals::AnyTuple> &tuple_instructions_temp, int &param,
								   bool &failed, std::vector<uint8_t> &last_param, short &zero_count) {
	if ((current_opcode > 0xA && current_opcode <= 0xAF) || (current_opcode > 0xAF) ||
		zero_count >= 1) { // first looking for a 1 byte instruction
		auto temp_int = static_cast<uint8_t>(current_opcode);
		// if instruction failed, we can just keep looping to find the instruction.
		tuple_instructions_temp.clear();
		if (i_instruction_find >= 1.5 && i_instruction_max >= 2.0) {
			param = ntohs(param); // Will not always work though
			(i_instruction_find == 1) ? i_instruction_find += 1 : int();
		}
		failed = !correct_opcode(tuple_instructions_temp, temp_int, param);
		if (failed && param != 0) {
			current_opcode = add_digits(current_opcode, param);
			i_instruction_find = 0;
			temp_int = current_opcode;
			param = 0;
			failed = !correct_opcode(tuple_instructions_temp, temp_int, param);
		}
		if (!failed) {
			auto i = tuple_instructions_temp.back();
			i_instruction_max = std::get<3>(i);
			add_last_param(i_instruction_find, last_param, zero_count, current_opcode, param);
			return true;
		}
	}
	return false;
}

bool disassembler::add_digit(char ch_int, std::stringstream &ss) {
	if (ch_int == '\n' || ch_int == '\0' || ch_int == ' ' || ch_int == '\t')
		return false; // skipping
	if (std::isalpha(ch_int) && !std::isxdigit(ch_int))
		exception::invalid_asm(); // aint no letters in machine code
	ss << std::hex << ch_int;
	return true;
}

/*
bool disassembler::is_empty_instruction(uint8_t current_opcode, int i_string, short zero_count) {
	int size = machine_code.size() - 1;
	if(zero_count < 4)
		return false;
	if(size == i_string)
		return false; // at end of program, preventing segfault
	if(machine_code[i_string + 1] == 0) // its an empty instruction
		return true;
	return false;
}
*/

bool disassembler::add_instruction(int &current_opcode, short &addr_count, std::vector<uint8_t> &last_param, int &param,
								   int &address, disassembler_globals::AnyTuple tuple) {
	if (tuple != disassembler_globals::EMPTY_TUPLE) {
		auto tuple = tuple_instructions_temp.back();
		tuple_instructions[address] = tuple;
	}
	finish_instruction(current_opcode, addr_count, last_param, param, address, tuple);
	i_instruction_max = 0;
	i_instruction_find = 0;

	return true;
}

std::map<uint16_t, disassembler_globals::AnyTuple> disassembler::disassemble() {
	int address{};
	int i_string = -1;				  // iteration for string
	int current_opcode{};			  // using uint8_t gave me a real headache
	short addr_count{}, zero_count{}; // zero count for nop
	std::stringstream ss;			  // for hex
	std::string digit_str;			  // in case function name, you can check for :
	int param{};					  // MVI, etc.
	bool failed{};					  // for checking if instruction x byte find operation failed
	std::vector<uint8_t> last_param;  // in case we forgot a param
	for (char ch_int : machine_code) {
		i_string++;
		if (!add_digit(ch_int, ss))
			continue;
		digit_str += ch_int;
		int digit = char_to_hex(ch_int);
		if (digit == 0 && !last_param.empty()) { // instruction successfully found, no need to keep looking
			i_instruction_find += 0.5;
			if (i_instruction_find >= i_instruction_max) {
				auto tuple = tuple_instructions_temp.back();
				add_instruction(current_opcode, addr_count, last_param, param, address, tuple);
			}
		}
		if (addr_count < 4) {
			addr_count++;
			continue;
		}
		if (failed || !last_param.empty())
			param = add_digits(param, digit);
		else
			current_opcode = add_digits(current_opcode, digit);
		if (validate_opcode(current_opcode, i_instruction_max, i_instruction_find, tuple_instructions_temp, param,
							failed, last_param, zero_count))
			continue;
		if (digit == 0)
			zero_count++;
	}
	if (!last_param.empty()) {
		tuple_instructions[address] = tuple_instructions_temp.back();
	}
	return tuple_instructions;
}
