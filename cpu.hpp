#pragma once
#include <iostream>

class cpu {
	public:
		cpu() {
			A = 0;
			B = 0;
			C = 0;
			E = 0;
			H = 0;
			L = 0;
			PC = 0;
			SP = 0;
			
		};
		virtual ~cpu() {

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

		/*
		 * Instruction handlers
		 */
		static void nop() {
			return;
		}
		template<typename T, typename T2>
		static void mov(T &register_one, T2 &register_two) {
			if(std::is_same<T, uint8_t>() && std::is_same<T2, uint16_t>())
				return; // throwing invalid assembly instruction l8ter
			register_one = register_two;
		}

};
