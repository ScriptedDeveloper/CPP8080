#include "disassemble.hpp"
#include "cpu.hpp"
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
	opmap = std::make_unique<std::unordered_map<uint8_t, disassembler_globals::AnyTuple>>(
		std::unordered_map<uint8_t, disassembler_globals::AnyTuple>{
			{0x00, {"NOP", [](uint8_t val = 0){
			nop();}, 1}},
			{0x40, {" B,B", [](uint8_t val = 0) {
			mov(memory::B, memory::B); }, 1}},
			{0x41, {"B,C", [](uint8_t val = 0) {
			mov(memory::B, memory::C); }, 1}},
			{0x42, {"B,D", [](uint8_t val = 0) {
			mov(memory::B, memory::D); }, 1}},
			{0x43, {"B,E", [](uint8_t val = 0) {
			mov(memory::B, memory::E); }, 1}},
			{0x44, {"B,H", [](uint8_t val = 0) {
			mov(memory::B, memory::H); }, 1}},
			{0x45, {"B,L", [](uint8_t val = 0) {
			mov(memory::B, memory::L); }, 1}},
			{0x47, {"B,A", [](uint8_t val = 0) {
			mov(memory::B, memory::A); }, 1}},
			{0x48, {"C,B", [](uint8_t val = 0) {
			mov(memory::C, memory::B); }, 1}},
			{0x49, {"C,C", [](uint8_t val = 0) {
			mov(memory::C, memory::C); }, 1}},
			{0x4A, {"C,D", [](uint8_t val = 0) {
			mov(memory::C, memory::D); }, 1}},
			{0x4B, {"C,E", [](uint8_t val = 0) {
			mov(memory::C, memory::E); }, 1}},
			{0x4C, {"C,H", [](uint8_t val = 0) {
			mov(memory::C, memory::H); }, 1}},
			{0x4D, {"C,L", [](uint8_t val = 0) {
			mov(memory::C, memory::L); }, 1}},
			{0x4F, {"C,A", [](uint8_t val = 0) {
			mov(memory::C, memory::A); }, 1}},
			{0x50, {"D,B", [](uint8_t val = 0) {
			mov(memory::D, memory::B); }, 1}},
			{0x51, {"D,C", [](uint8_t val = 0) {
			mov(memory::D, memory::C); }, 1}},
			{0x52, {"D,D", [](uint8_t val = 0) {
			mov(memory::D, memory::D); }, 1}},
			{0x53, {"D,E", [](uint8_t val = 0) {
			mov(memory::D, memory::E); }, 1}},
			{0x54, {"D,H", [](uint8_t val = 0) {
			mov(memory::D, memory::H); }, 1}},
			{0x55, {"D,L", [](uint8_t val = 0) {
			mov(memory::D, memory::L); }, 1}},
			{0x57, {"D,A", [](uint8_t val = 0) {
			mov(memory::D, memory::A); }, 1}},
			{0x58, {"E,B", [](uint8_t val = 0) {
			mov(memory::E, memory::B); }, 1}},
			{0x59, {"E,C", [](uint8_t val = 0) {
			mov(memory::E, memory::C); }, 1}},
			{0x5A, {"E,D", [](uint8_t val = 0) {
			mov(memory::E, memory::D); }, 1}},
			{0x5B, {"E,E", [](uint8_t val = 0) {
			mov(memory::E, memory::E); }, 1}},
			{0x5C, {"E,H", [](uint8_t val = 0) {
			mov(memory::E, memory::H); }, 1}},
			{0x5D, {"E,L", [](uint8_t val = 0) {
			mov(memory::E, memory::L); }, 1}},
			{0x5F, {"E,A", [](uint8_t val = 0) {
			mov(memory::E, memory::A); }, 1}},
			{0x60, {"H,B", [](uint8_t val = 0) {
			mov(memory::H, memory::B); }, 1}},
			{0x61, {"H,C", [](uint8_t val = 0) {
			mov(memory::H, memory::C); }, 1}},
			{0x62, {"H,D", [](uint8_t val = 0) {
			mov(memory::H, memory::D); }, 1}},
			{0x63, {"H,E", [](uint8_t val = 0) {
			mov(memory::H, memory::E); }, 1}},
			{0x64, {"H,H", [](uint8_t val = 0) {
			mov(memory::H, memory::H); }, 1}},
			{0x65, {"H,L", [](uint8_t val = 0) {
			mov(memory::H, memory::L); }, 1}},
			{0x67, {"H,A", [](uint8_t val = 0) {
			mov(memory::H, memory::A); }, 1}},
			{0x68, {"L,B", [](uint8_t val = 0) {
			mov(memory::L, memory::B); }, 1}},
			{0x69, {"L,C", [](uint8_t val = 0) {
			mov(memory::L, memory::C); }, 1}},
			{0x6A, {"L,D", [](uint8_t val = 0) {
			mov(memory::L, memory::D); }, 1}},
			{0x6B, {"L,E", [](uint8_t val = 0) {
			mov(memory::L, memory::E); }, 1}},
			{0x6C, {"L,H", [](uint8_t val = 0) {
			mov(memory::L, memory::H); }, 1}},
			{0x6D, {"L,L", [](uint8_t val = 0) {
			mov(memory::L, memory::L); }, 1}},
			{0x6F, {"L,A", [](uint8_t val = 0) {
			mov(memory::L, memory::A); }, 1}},
			{0x78, {"A,B", [](uint8_t val = 0) {
			mov(memory::A, memory::B); }, 1}},
			{0x79, {"A,C", [](uint8_t val = 0) {
			mov(memory::A, memory::C); }, 1}},
			{0x7A, {"A,D", [](uint8_t val = 0) {
			mov(memory::A, memory::D); }, 1}},
			{0x7B, {"A,E", [](uint8_t val = 0) {
			mov(memory::A, memory::E); }, 1}},
			{0x7C, {"A,H", [&](uint8_t val = 0) {
			mov(memory::A, memory::H); }, 1}},
			{0x7D, {"A,L", [](uint8_t val = 0) {
			mov(memory::A, memory::L); }, 1}},
			{0x7F, {"A,A", [](uint8_t val = 0) {
			mov(memory::A, memory::A); }, 1}},
			{0x3E, {"A", [](uint8_t val) {
				mvi(val, memory::A);
			}, 0}}
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
								  uint8_t &current_opcode, int param) {
		auto tuple_inst = find_instruction(current_opcode);
		if (std::get<1>(tuple_inst) == nullptr || (std::get<0>(tuple_inst).size() == 1 && param == UINT8_MAX + 1))
			return false;
		else {
			/*
			 * It worked so lets push the tuple onto the vector
			 */
			std::get<2>(tuple_inst) = (param == UINT8_MAX + 1) ? 0 : param;
			tuple_instructions.push_back(tuple_inst);
			current_opcode = 0;
		}
		return true; // also can indicate by return val if it has more than 1 byte instruction and pass the value like that inside a tuple
}

void finish_instruction(int &current_opcode, short &addr_count, int &address, std::vector<uint8_t> &last_param, int &param) {
		current_opcode = 0;
		addr_count = 0;
		address++;
		param = 0;
		last_param.clear();
}

std::vector<disassembler_globals::AnyTuple> disassembler::disassemble() {
		int address{};
		int current_opcode{};			  // using uint8_t is gave me a real headache
		short addr_count{}, zero_count{}; // zero count for nop
		std::stringstream ss;			  // for hex
		std::vector<disassembler_globals::AnyTuple> tuple_instructions{};
		std::vector<disassembler_globals::AnyTuple> tuple_instructions_temp{};
		int param{}; // MVI, etc.
		bool failed; // for checking if instruction x byte find operation failed
		std::vector<uint8_t> last_param; // in case we forgot a param
		double i_instruction_find = 0; // for finding all digits in a >=2 byte instruction
		for (char ch_int : machine_code) {
			if (ch_int == '\n' || ch_int == '\0')
				continue; // skipping
			if (std::isalpha(ch_int) && !std::isxdigit(ch_int))
				exception::invalid_asm(); // aint no letters in machine code
			ss << std::hex << ch_int;
			int digit = char_to_hex(ch_int);
			if(digit == 0 && !last_param.empty()) { // instruction successfully found, no need to keep looking
				i_instruction_find += 0.5;
				if(i_instruction_find == 2.0) {
					finish_instruction(current_opcode, addr_count, address, last_param, param);
					auto last_elm = tuple_instructions_temp.end() - 1;
					tuple_instructions.push_back(*last_elm);
				}
			}
			if (addr_count < 4) {
				addr_count++;
				continue;
			}
			bool is_nop = current_opcode == 0 &&
						  digit == 0; // nop is very special since it has two zeros, so this is how I represent it
			if (is_nop)
				zero_count++;
			if(failed || !last_param.empty())
				param = add_digits(param, digit);
			else
				current_opcode = add_digits(current_opcode, digit);
			if ((current_opcode > 0xA && current_opcode <= 0xAF) || (current_opcode > 0xAF) ||
				zero_count == 2) { // first looking for a 1 byte instruction
				auto temp_int = static_cast<uint8_t>(current_opcode);
							// if instruction failed, we can just keep looping to find the instruction.
				tuple_instructions_temp.clear();
				failed = !correct_opcode(tuple_instructions_temp, temp_int, param);
				if (!failed) {
					i_instruction_find += 0.5;
					last_param.clear();
					last_param.push_back(current_opcode);
					last_param.push_back(param);
				}
			}
		}
		if(!last_param.empty())
			tuple_instructions.push_back(*tuple_instructions_temp.begin()++);
		return tuple_instructions;
}
