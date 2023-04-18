#include "cpu.hpp"
	

uint8_t memory::A{};
uint8_t memory::B{};
uint8_t memory::C{};
uint8_t memory::D{};
uint8_t memory::E{};
uint8_t memory::H{};
uint8_t memory::L{};
uint16_t memory::PC{};
uint16_t memory::SP{}; 

std::stack<uint8_t> memory::stack{}; // Stack of CPU

// TODO: Change register params once >1 byte instruction, add other opcodes than MVI
std::unordered_map<std::string_view, uint8_t> register_params{
	{"A", 0x00},
	{"B", 0x00},
	{"C", 0x00},
	{"D", 0x00},
	{"E", 0x00},
	{"H", 0x00},
	{"L", 0x00},
	{"PC", 0x00},
	{"SP", 0x00}
};

bool cpu_handler::handle_instructions() {
	for (auto instruction : tuple_instructions) {
		try {
			auto ptr = std::get<1>(instruction);
			auto params = register_params.find(std::get<0>(instruction))->second;
			ptr(params);
		} catch (...) {
			exception::invalid_asm();
		}
	}
	return true;
}

bool cpu_instructions::nop() { return true; }

void cpu_instructions::mvi(uint8_t val, uint8_t &reg) {
	reg = val;
}
