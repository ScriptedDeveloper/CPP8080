#pragma once
#include <iostream>
#include "disassemble.hpp"
#include "error.hpp"

namespace cpu_instructions {
	/*
	 * Instruction handlers
	 */
	bool nop();

	template<typename T, typename T2>
	void mov(T &register_one, T2 &register_two) {
		if(std::is_same<T, uint8_t>() && std::is_same<T2, uint16_t>())
			return; // throwing invalid assembly instruction l8ter
		register_one = register_two;
	}

};

class cpu_handler {
	public:
		cpu_handler(std::vector<disassembler_globals::AnyTuple> &tuple) : tuple_instructions(tuple) {
			
		};
		virtual ~cpu_handler() {

		};
		/*
		 * Registers
		 */
		static uint8_t A;
		static uint8_t B;
		static uint8_t C;
		static uint8_t D;
		static uint8_t E;
		static uint8_t H;
		static uint8_t L;
		static uint16_t PC; // Program counter
		static uint16_t SP; // Stack Pointer
		bool handle_instructions();
	private:
		std::vector<disassembler_globals::AnyTuple> tuple_instructions{};

};
