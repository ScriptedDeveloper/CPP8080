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

std::unordered_map<uint8_t, uint8_t> register_params{};

bool cpu_handler::handle_instructions() {
	for (auto instruction : tuple_instructions) {
		try {
			std::invoke(std::get<1>(instruction));
		} catch (...) {
			exception::invalid_asm();
		}
	}
	return true;
}

bool cpu_instructions::nop() { return true; }
