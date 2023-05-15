#include "disassemble.hpp"
#include <bit>
#include <codecvt>
#include <cstdint>

class disassembler;

std::unique_ptr<std::unordered_map<uint16_t, disassembler_globals::AnyTuple>> disassembler::opmap;

/*
std::string disassembler::get_file_contents() {
	std::stringstream ss_file;
	ss_file << ifsfile.rdbuf();
	return ss_file.str();
}
*/

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
		exception::invalid_asm(opcode);
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

uint8_t disassembler::finish_instruction(uint8_t &current_opcode, std::vector<uint16_t> &last_param, uint16_t &param,
										 uint16_t &address, disassembler_globals::AnyTuple &tuple) {
	current_opcode = 0;
	param = 0;
	address += ((std::get<3>(tuple) * 2) + 1);
	last_param.clear();
	return address;
}

void disassembler::add_last_param(double &i_instruction_find, std::vector<uint16_t> &last_param, short &zero_count,
								  int current_opcode, int param) {
	// i_instruction_find += 0.5;
	last_param.clear();
	zero_count = 0;
	last_param.push_back(current_opcode);
	last_param.push_back(param);
}

bool disassembler::validate_opcode(uint8_t &current_opcode, double &i_instruction_max, double &i_instruction_find,
								   std::vector<disassembler_globals::AnyTuple> &tuple_instructions_temp,
								   uint16_t &param, bool &failed, std::vector<uint16_t> &last_param, short &zero_count,
								   uint16_t &address) {
	auto temp_int = static_cast<uint8_t>(current_opcode);
	// if instruction failed, we can just keep looping to find the instruction.
	tuple_instructions_temp.clear();
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
		if (i_instruction_max == 0) {
			add_instruction(current_opcode, last_param, param, address, i_instruction_find, i_instruction_max, i);
		} else {
			add_last_param(i_instruction_find, last_param, zero_count, current_opcode, param);
		}
		return true;
	}
	return false;
}

bool disassembler::add_digit(char ch_int, std::stringstream &ss) {
	if (ch_int == '\n' || ch_int == '\0' || ch_int == ' ' || ch_int == '\t')
		return false; // skipping
	if (std::isalpha(ch_int) && !std::isxdigit(ch_int))
		exception::invalid_asm(ch_int); // aint no letters in machine code
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

std::map<uint16_t, disassembler_globals::AnyTuple> disassembler::disassemble() {
	uint16_t address{};
	int i_string = -1;				  // iteration for string
	uint8_t current_opcode{};		  // using uint8_t gave me a real headache
	short zero_count{};				  // zero count for nop
	std::stringstream ss;			  // for hex
	std::string digit_str;			  // in case function name, you can check for :
	uint16_t param{};				  // MVI, etc.
	bool failed{};					  // for checking if instruction x byte find operation failed
	std::vector<uint16_t> last_param; // in case we forgot a param
	for (uint8_t digit : bytes_vec) {
		i_string++;
		if (!last_param.empty())
			param = add_digits(param, digit);
		else
			current_opcode = add_digits(current_opcode, digit);
		if (!last_param.empty()) { // instruction successfully found, no need to keep looking
			i_instruction_find += 0.5;
			if (i_instruction_find >= i_instruction_max) {
				bool skip_endian{};
				auto tuple = tuple_instructions_temp.back();
				if (i_instruction_max == 1 && digit == 0) {
					param = param / 0x10; // useless extra 0
					skip_endian = true;
				}
				add_instruction(current_opcode, last_param, param, address, i_instruction_find, i_instruction_max,
								tuple, skip_endian);
				continue;
			}
		}
		if (validate_opcode(current_opcode, i_instruction_max, i_instruction_find, tuple_instructions_temp, param,
							failed, last_param, zero_count, address))
			continue;
		if (digit == 0)
			zero_count++;
		else
			exception::invalid_asm(current_opcode);
	}
	if (!last_param.empty()) {
		tuple_instructions[address] = tuple_instructions_temp.back();
	}

	return tuple_instructions;
}

void disassembler::init_array() {
	using namespace cpu_instructions;
	// clang-format off
	opmap = std::make_unique<std::unordered_map<
		uint16_t, disassembler_globals::
		 AnyTuple>>(std::
		unordered_map<uint16_t, disassembler_globals::AnyTuple>{
		{0x00, {"NOP", [](uint16_t val = 0) { nop(); }, 1, 0.0}},
		{0x40, {"B,B", [](uint16_t val = 0) { mov(memory::B, memory::B); }, 1, 0.0}},
		{0x41, {"B,C", [](uint16_t val = 0) { mov(memory::B, memory::C); }, 1, 0.0}},
		{0x42, {"B,D", [](uint16_t val = 0) { mov(memory::B, memory::D); }, 1, 0.0}},
		{0x43, {"B,E", [](uint16_t val = 0) { mov(memory::B, memory::E); }, 1, 0.0}},
		{0x44, {"B,H", [](uint16_t val = 0) { mov(memory::B, memory::H); }, 1, 0.0}},
		{0x45, {"B,L", [](uint16_t val = 0) { mov(memory::B, memory::L); }, 1, 0.0}},
		{0x47, {"B,A", [](uint16_t val = 0) { mov(memory::B, memory::A); }, 1, 0.0}},
		{0x48, {"C,B", [](uint16_t val = 0) { mov(memory::C, memory::B); }, 1, 0.0}},
		{0x49, {"C,C", [](uint16_t val = 0) { mov(memory::C, memory::C); }, 1, 0.0}},
		{0x4A, {"C,D", [](uint16_t val = 0) { mov(memory::C, memory::D); }, 1, 0.0}},
		{0x4B, {"C,E", [](uint16_t val = 0) { mov(memory::C, memory::E); }, 1, 0.0}},
		{0x4C, {"C,H", [](uint16_t val = 0) { mov(memory::C, memory::H); }, 1, 0.0}},
		{0x4D, {"C,L", [](uint16_t val = 0) { mov(memory::C, memory::L); }, 1, 0.0}},
		{0x4F, {"C,A", [](uint16_t val = 0) { mov(memory::C, memory::A); }, 1, 0.0}},
		{0x50, {"D,B", [](uint16_t val = 0) { mov(memory::D, memory::B); }, 1, 0.0}},
		{0x51, {"D,C", [](uint16_t val = 0) { mov(memory::D, memory::C); }, 1, 0.0}},
		{0x52, {"D,D", [](uint16_t val = 0) { mov(memory::D, memory::D); }, 1, 0.0}},
		{0x53, {"D,E", [](uint16_t val = 0) { mov(memory::D, memory::E); }, 1, 0.0}},
		{0x54, {"D,H", [](uint16_t val = 0) { mov(memory::D, memory::H); }, 1, 0.0}},
		{0x55, {"D,L", [](uint16_t val = 0) { mov(memory::D, memory::L); }, 1, 0.0}},
		{0x57, {"D,A", [](uint16_t val = 0) { mov(memory::D, memory::A); }, 1, 0.0}},
		{0x58, {"E,B", [](uint16_t val = 0) { mov(memory::E, memory::B); }, 1, 0.0}},
		{0x59, {"E,C", [](uint16_t val = 0) { mov(memory::E, memory::C); }, 1, 0.0}},
		{0x5A, {"E,D", [](uint16_t val = 0) { mov(memory::E, memory::D); }, 1, 0.0}},
		{0x5B, {"E,E", [](uint16_t val = 0) { mov(memory::E, memory::E); }, 1, 0.0}},
		{0x5C, {"E,H", [](uint16_t val = 0) { mov(memory::E, memory::H); }, 1, 0.0}},
		{0x5D, {"E,L", [](uint16_t val = 0) { mov(memory::E, memory::L); }, 1, 0.0}},
		{0x5F, {"E,A", [](uint16_t val = 0) { mov(memory::E, memory::A); }, 1, 0.0}},
		{0x60, {"H,B", [](uint16_t val = 0) { mov(memory::H, memory::B); }, 1, 0.0}},
		{0x61, {"H,C", [](uint16_t val = 0) { mov(memory::H, memory::C); }, 1, 0.0}},
		{0x62, {"H,D", [](uint16_t val = 0) { mov(memory::H, memory::D); }, 1, 0.0}},
		{0x63, {"H,E", [](uint16_t val = 0) { mov(memory::H, memory::E); }, 1, 0.0}},
		{0x64, {"H,H", [](uint16_t val = 0) { mov(memory::H, memory::H); }, 1, 0.0}},
		{0x65, {"H,L", [](uint16_t val = 0) { mov(memory::H, memory::L); }, 1, 0.0}},
		{0x67, {"H,A", [](uint16_t val = 0) { mov(memory::H, memory::A); }, 1, 0.0}},
		{0x68, {"L,B", [](uint16_t val = 0) { mov(memory::L, memory::B); }, 1, 0.0}},
		{0x69, {"L,C", [](uint16_t val = 0) { mov(memory::L, memory::C); }, 1, 0.0}},
		{0x6A, {"L,D", [](uint16_t val = 0) { mov(memory::L, memory::D); }, 1, 0.0}},
		{0x6B, {"L,E", [](uint16_t val = 0) { mov(memory::L, memory::E); }, 1, 0.0}},
		{0x6C, {"L,H", [](uint16_t val = 0) { mov(memory::L, memory::H); }, 1, 0.0}},
		{0x6D, {"L,L", [](uint16_t val = 0) { mov(memory::L, memory::L); }, 1, 0.0}},
		{0x6F, {"L,A", [](uint16_t val = 0) { mov(memory::L, memory::A); }, 1, 0.0}},
		{0x78, {"A,B", [](uint16_t val = 0) { mov(memory::A, memory::B); }, 1, 0.0}},
		{0x79, {"A,C", [](uint16_t val = 0) { mov(memory::A, memory::C); }, 1, 0.0}},
		{0x7A, {"A,D", [](uint16_t val = 0) { mov(memory::A, memory::D); }, 1, 0.0}},
		{0x7B, {"A,E", [](uint16_t val = 0) { mov(memory::A, memory::E); }, 1, 0.0}},
		{0x7C, {"A,H", [&](uint16_t val = 0) { mov(memory::A, memory::H); }, 1, 0.0}},
		{0x7D, {"A,L", [](uint16_t val = 0) { mov(memory::A, memory::L); }, 1, 0.0}},
		{0x7F, {"A,A", [](uint16_t val = 0) { mov(memory::A, memory::A); }, 1, 0.0}},
		{0x3E, {"A", [](uint16_t val) { mvi(val, memory::A); }, 0, 0.5}},
		{0x06, {"B", [](uint16_t val) { mvi(val, memory::B); }, 0, 0.5}},
		{0x0E, {"C", [](uint16_t val) { mvi(val, memory::C); }, 0, 0.5}},
		{0x16, {"D", [](uint16_t val) { mvi(val, memory::D); }, 0, 0.5}},
		{0x1E, {"E", [](uint16_t val) { mvi(val, memory::E); }, 0, 0.5}},
		{0x26, {"H", [](uint16_t val) { mvi(val, memory::H); }, 0, 0.5}},
		{0x2E, {"L", [](uint16_t val) { mvi(val, memory::L); }, 0, 0.5}},
		{0xC5, {"B,B", [](uint16_t val = 0) { // double B, marked as no need for params (pushed onto stack)
			cpu_instructions::push(memory::B);
			},
		1, 0.0}},
		{0xD5, {"D,D", [](uint16_t val = 0) {
			cpu_instructions::push(memory::D);
			},
		1, 0.0}},	
		{0xE5, {"H,H", [](uint16_t val = 0) {
			cpu_instructions::push(memory::H);
			},
		1, 0.0}},
		{0xC1, {"B,B", [](uint16_t val = 0) {
			cpu_instructions::pop(memory::B);
			},
		1, 0.0}},	
		{0xD1, {"D,D", [](uint16_t val = 0) {
			cpu_instructions::pop(memory::D);
			},
		1, 0.0}},	
		{0xD2, {"H,H", [](uint16_t val = 0) {
			cpu_instructions::pop(memory::H);
			},
		1, 0.0}},
		{0xC3, {"X,X", [](uint16_t val) {
			cpu_instructions::jmp(val);
			},
		0, 1.0}},
		{0xCD, {"X,X", [](uint16_t val) {
			cpu_instructions::call(val);
			},
		0, 1.0}},	
		{0xC9, {"X", [](uint16_t val = 0) {
			cpu_instructions::ret();
			},
		0, 0.0}},
		{0x80, {"B", [](uint16_t val = 0) {
			cpu_instructions::add(memory::B);
			},
		0, 0.0}},	
		{0x81, {"C", [](uint16_t val = 0) {
			cpu_instructions::add(memory::C);
			},
		0, 0.0}},	
		{0x82, {"D", [](uint16_t val = 0) {
			cpu_instructions::add(memory::D);
			},
		0, 0.0}},	
		{0x83, {"E", [](uint16_t val = 0) {
			cpu_instructions::add(memory::E);
			},
		0, 0.0}},	
		{0x84, {"H", [](uint16_t val = 0) {
			cpu_instructions::add(memory::H);
			},
		0, 0.0}},	
		{0x85, {"L", [](uint16_t val = 0) {
			cpu_instructions::add(memory::L);
			},
		0, 0.0}},	
		{0x87, {"A", [](uint16_t val = 0) {
			cpu_instructions::add(memory::A); // this is efficevly just doubling the register
			},
		0, 0.0}},	
		{0xFB, {"X", [](uint16_t val = 0) {
			cpu_instructions::ei(); // interrupt flag on
			},
		0, 0.0}},	
		{0xF3, {"X", [](uint16_t val = 0) {
			cpu_instructions::di(); // interrupt flag off
			},
		0, 0.0}},	
		{0x76, {"X", [](uint16_t val = 0) {
			cpu_instructions::hlt(); // halts CPU
			},
		0, 0.0}},	
		{0xC7, {"0", [&](uint16_t val = 0) {
			isr isr_obj;
			isr_obj.RST(0);
			},
		0, 0.0}},	
		{0xCF, {"1", [](uint16_t val = 0) {
			isr i;
			i.RST(1);
			},
		0, 0.0}},	
		{0xD7, {"2", [](uint16_t val = 0) {
			isr i;
			i.RST(2);
			},
		0, 0.0}},	
		{0xDF, {"3", [](uint16_t val = 0) {
			isr i;
			i.RST(3);
			},
		0, 0.0}},	
		{0xE7, {"4", [](uint16_t val = 0) {
			isr i;
			i.RST(4);
			},
		0, 0.0}},	
		{0xEF, {"5", [](uint16_t val = 0) {
			isr i;
			i.RST(5);
			},
		0, 0.0}},	
		{0xF7, {"6", [](uint16_t val = 0) {
			isr i;
			i.RST(6);
			},
		0, 0.0}},	
		{0xF7, {"7", [](uint16_t val = 0) {
			isr i;
			i.RST(7);
			},
		0, 0.0}},	
		{0xD3, {"X", [](uint16_t val) {
			uint8_t temp = val; // cant cast uint16_t because function requires reference
			cpu_instructions::out(temp);
			},
		0, 0.5}},	
		{0xDB, {"X", [](uint16_t val) {
			uint8_t temp = val;
			cpu_instructions::in(temp);
			},
		0, 0.5}},	
		{0x3C, {"A", [](uint16_t val = 0) {
			cpu_instructions::inr(memory::A);
			},
		0, 0.0}},	
		{0x04, {"B", [](uint16_t val = 0) {
			cpu_instructions::inr(memory::B);
			},
		0, 0.0}},	
		{0x0C, {"C", [](uint16_t val = 0) {
			cpu_instructions::inr(memory::C);
			},
		0, 0.0}},	
		{0x14, {"D", [](uint16_t val = 0) {
			cpu_instructions::inr(memory::D);
			},
		0, 0.0}},	
		{0x0C, {"E", [](uint16_t val = 0) {
			cpu_instructions::inr(memory::E);
			},
		0, 0.0}},	
		{0x2C, {"L", [](uint16_t val = 0) {
			cpu_instructions::inr(memory::L);
			},
		0, 0.0}},	
		{0x3F, {"X", [](uint16_t val = 0) {
			cpu_instructions::cmc();
			},
		0, 0.0}}, 
		{0x90, {"B", [](uint16_t val = 0) {
			cpu_instructions::sub(memory::B);
		},
		0, 0.0}},
		{0x91, {"C", [](uint16_t val = 0) {
			cpu_instructions::sub(memory::C);
			},
		0, 0.0}},
         	{0x92, {"D", [](uint16_t val = 0) {
             		cpu_instructions::sub(memory::D);
			},
		0, 0.0}},   
		{0x93, {"E", [](uint16_t val = 0) {
			cpu_instructions::sub(memory::E);
			},
		0, 0.0}},
         	{0x94, {"H", [](uint16_t val = 0) {
             		cpu_instructions::sub(memory::H);
			},
         	0, 0.0}},   
         	{0x95, {"L", [](uint16_t val = 0) {
             		cpu_instructions::sub(memory::L);
			},
         	0, 0.0}},
         	{0x97, {"A", [](uint16_t val = 0) {
             		cpu_instructions::sub(memory::A); // this is efficevly just subtracting the register
			},
		0, 0.0}},
		{0x8B, {"B", [](uint16_t val = 0) {
			cpu_instructions::cmp(memory::B);
		},
		0, 0.0}},
		{0x9B, {"C", [](uint16_t val = 0) {
			cpu_instructions::cmp(memory::C);
			},
		0, 0.0}},
         	{0xBA, {"D", [](uint16_t val = 0) {
             		cpu_instructions::cmp(memory::D);
			},
		0, 0.0}},   
		{0xBB, {"E", [](uint16_t val = 0) {
			cpu_instructions::cmp(memory::E);
			},
		0, 0.0}},
         	{0xBC, {"H", [](uint16_t val = 0) {
             		cpu_instructions::cmp(memory::H);
			},
         	0, 0.0}},   
         	{0xBD, {"L", [](uint16_t val = 0) {
             		cpu_instructions::cmp(memory::L);
			},
         	0, 0.0}},
         	{0xBF, {"A", [](uint16_t val = 0) {
             		cpu_instructions::cmp(memory::A); // this is efficevly just A==A
			},
		0, 0.0}},
		{0xA8, {"B", [](uint16_t val = 0) {
			cpu_instructions::xra(memory::B);
		},
		0, 0.0}},
		{0xA9, {"C", [](uint16_t val = 0) {
			cpu_instructions::xra(memory::C);
			},
		0, 0.0}},
         	{0xAA, {"D", [](uint16_t val = 0) {
             		cpu_instructions::xra(memory::D);
			},
		0, 0.0}},   
		{0xAB, {"E", [](uint16_t val = 0) {
			cpu_instructions::xra(memory::E);
			},
		0, 0.0}},
         	{0xAC, {"H", [](uint16_t val = 0) {
             		cpu_instructions::xra(memory::H);
			},
         	0, 0.0}},   
         	{0xAD, {"L", [](uint16_t val = 0) {
             		cpu_instructions::xra(memory::L);
			},
         	0, 0.0}},
         	{0xAF, {"A", [](uint16_t val = 0) {
             		cpu_instructions::xra(memory::A); 
			},
		0, 0.0}},
		{0xB0, {"B", [](uint16_t val = 0) {
			cpu_instructions::ora(memory::B);
		},
		0, 0.0}},
		{0xB1, {"C", [](uint16_t val = 0) {
			cpu_instructions::ora(memory::C);
			},
		0, 0.0}},
         	{0xB2, {"D", [](uint16_t val = 0) {
             		cpu_instructions::ora(memory::D);
			},
		0, 0.0}},   
		{0xB3, {"E", [](uint16_t val = 0) {
			cpu_instructions::ora(memory::E);
			},
		0, 0.0}},
         	{0xB4, {"H", [](uint16_t val = 0) {
             		cpu_instructions::ora(memory::H);
			},
         	0, 0.0}},   
         	{0xB5, {"L", [](uint16_t val = 0) {
             		cpu_instructions::ora(memory::L);
			},
         	0, 0.0}},
         	{0xB7, {"A", [](uint16_t val = 0) {
             		cpu_instructions::ora(memory::A); 
			},
		0, 0.0}},
		{0xA0, {"B", [](uint16_t val = 0) {
			cpu_instructions::ana(memory::B);
		},
		0, 0.0}},
		{0xA1, {"C", [](uint16_t val = 0) {
			cpu_instructions::ana(memory::C);
			},
		0, 0.0}},
         	{0xA2, {"D", [](uint16_t val = 0) {
             		cpu_instructions::ana(memory::D);
			},
		0, 0.0}},   
		{0xA3, {"E", [](uint16_t val = 0) {
			cpu_instructions::ana(memory::E);
			},
		0, 0.0}},
         	{0xA4, {"H", [](uint16_t val = 0) {
             		cpu_instructions::ana(memory::H);
			},
         	0, 0.0}},   
         	{0xA5, {"L", [](uint16_t val = 0) {
             		cpu_instructions::ana(memory::L);
			},
         	0, 0.0}},
         	{0xA7, {"A", [](uint16_t val = 0) {
             		cpu_instructions::ana(memory::A); 
			},
		0, 0.0}}

	});
	// clang-format on
}
