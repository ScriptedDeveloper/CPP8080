#include "cpu.hpp"

/*
 * static variable declaration
 */
uint8_t cpu_handler::A = 0;
uint8_t cpu_handler::B = 0;
uint8_t cpu_handler::C = 0;
uint8_t cpu_handler::D = 0;
uint8_t cpu_handler::E = 0;
uint8_t cpu_handler::H = 0;
uint8_t cpu_handler::L = 0;
uint16_t cpu_handler::PC = 0;
uint16_t cpu_handler::SP = 0; 

bool cpu_handler::handle_instructions() {
	for(auto instruction : tuple_instructions) {
		try {
			std::invoke(std::get<2>(instruction));
		} catch(...) {
			exception::invalid_asm();
		}
	}
	return true;
}

bool cpu_instructions::nop() {
	return true;
}
