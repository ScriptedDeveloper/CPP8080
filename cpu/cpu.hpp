#pragma once
#include "../disassemble/disassemble.hpp"
#include "../error/error.hpp"
#include <iostream>
#include <stack>

class cpu_handler {
  public:
	cpu_handler(std::map<uint16_t, disassembler_globals::AnyTuple> &tuple)
		: tuple_instructions(tuple){

		  };
	virtual ~cpu_handler(){

	};
	bool handle_instructions();
	template <typename T> static void set_carry_flag(T &val) {
		CF = true;
		auto mask = ~(1 << (sizeof(T) * 8 - 1)); // removing most significant bit, mask
		val = mask & val;
	}
	static bool CF; // Carry Flag
	static bool ZF; // Zero Flag
	static bool S;	// Sign Flag
	static bool CY; // Carry Flag
	static bool AC; // Auxiliary Carry Flag
	static bool I;	// Interrupt Flag
	static bool T;	// Trap Flag

  private:
	std::map<uint16_t, disassembler_globals::AnyTuple> tuple_instructions{};
};

namespace cpu_instructions {
/*
 * Instruction handlers
 */
bool nop();
template <typename T1, typename T2> void mvi(T1 val, T2 &reg) {
	if (val > sizeof(T2)) { // register cant hold this value... setting carry flag
		cpu_handler::set_carry_flag(val);
	}
	reg = val;
}

void push(uint8_t &reg); // self explanatory
void pop(uint8_t &reg);
void jmp(uint16_t &addr);
void jc(uint16_t addr);	 // jump if carry flag set
void jnc(uint16_t addr); // jump if not carry flag set
void jz(uint16_t addr);	 // jump if zero flag is set
void call(uint16_t &addr);
void jnz(uint16_t addr); // jump if zero flag is not set
void ret();

template <typename T, typename T2> void mov(T &register_one, T2 &register_two) {
	if (std::is_same<T, uint8_t>() && std::is_same<T2, uint16_t>())
		return; // throwing invalid assembly instruction l8ter
	// register_one = register_two;
	register_one = register_two;
	register_two = 0; // emptying register
}

}; // namespace cpu_instructions

namespace memory {
/*
 * Registers
 */
extern uint8_t A;
extern uint8_t B;
extern uint8_t C;
extern uint8_t D;
extern uint8_t E;
extern uint8_t H;
extern uint8_t L;
extern uint16_t PC; // Program counter
extern uint16_t SP; // Stack Pointer

extern std::stack<uint8_t> stack; // Stack of CPU
extern std::unordered_map<std::string_view, uint16_t>
	register_params; // extra params for the registers, could also use the tuple for this
					 // Will change this from string to char later!
};					 // namespace memory
