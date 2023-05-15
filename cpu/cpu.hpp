#pragma once
#include "../disassemble/disassemble.hpp"
#include "../error/error.hpp"
#include <iostream>
#include <limits>
#include <map>
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
		val = std::numeric_limits<T>::max(); // setting register value to max
	}
	static bool CF;					// Carry Flag
	static bool ZF;					// Zero Flag
	static bool S;					// Sign Flag
	static bool CY;					// Carry Flag
	static bool AC;					// Auxiliary Carry Flag
	static bool I;					// Interrupt Flag
	static bool T;					// Trap Flag
	static bool interrupts_enabled; // Interrupt flag
	static bool is_hlt;				// is the CPU on halt?

  private:
	std::map<uint16_t, disassembler_globals::AnyTuple> tuple_instructions{};
	void print_registers();
};

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
extern uint16_t M;
extern uint16_t PC; // Program counter
extern uint16_t SP; // Stack Pointer

extern std::stack<uint8_t> stack; // Stack of CPU
extern std::unordered_map<std::string_view, uint16_t>
	register_params; // extra params for the registers, could also use the tuple for this
					 // Will change this from string to char later!
} // namespace memory

namespace cpu_instructions {
/*
 * Instruction handlers
 */
template <typename... T2>
concept is_digits = (std::is_arithmetic_v<T2> && ...);
bool nop();
template <typename T1, typename T2>
	requires is_digits<T1, T2>
void mvi(T1 val, T2 &reg) {
	if (val > std::numeric_limits<T2>::max()) { // register cant hold this value... setting carry flag
		cpu_handler::set_carry_flag(val);
	}
	reg = val;
	cpu_handler::CF = (reg == 0) ? 1 : 0; // the mvi operation is 0, so set zero flag
}

template <typename T1>
	requires is_digits<T1>
void add(T1 &reg) {
	if (reg + memory::A > std::numeric_limits<decltype(memory::A)>::max())
		cpu_handler::set_carry_flag(reg);
	cpu_handler::CF = (reg == 0) ? 1 : 0; // the add operation is 0, so set zero flag
	memory::A += reg;
}
template <typename T1>
	requires is_digits<T1>
void sub(T1 &reg) {
	if (memory::A - reg < std::numeric_limits<decltype(memory::A)>::min())
		cpu_handler::set_carry_flag(reg);
	cpu_handler::CF = (reg == 0) ? 1 : 0; // the add operation is 0, so set zero flag
	memory::A += reg;
}

template <typename T1>
	requires is_digits<T1>
void cmp(T1 reg) { // same as sub instruction, but doesnt change accumulator
	sub(reg);
}

void out(uint8_t &device_list); // currently 0 : console output
void in(uint8_t &device_list);	// currently 0 : console input
void push(uint8_t &reg);		// self explanatory
void pop(uint8_t &reg);
void jmp(uint16_t &addr);
void jc(uint16_t addr);	 // jump if carry flag set
void jnc(uint16_t addr); // jump if not carry flag set
void jz(uint16_t addr);	 // jump if zero flag is set
void call(uint16_t &addr);
void jnz(uint16_t addr); // jump if zero flag is not set
void ei();				 // enables interrupt flag
void di();				 // disables interrupt flag
void hlt();
void ret();
void cmc(); // complement carry
template <typename T>
	requires is_digits<T>
void xra(T &reg) {
	/*
	 * Performs logical XOR with register A + reg
	 * Saves result in accumulator (register A)
	 */
	memory::A = memory::A ^ static_cast<decltype(memory::A)>(reg);
}
template <typename T>
	requires is_digits<T>
void ana(T &reg) {
	/*
	 * Same as above, only ANA
	 */
	memory::A = memory::A & static_cast<decltype(memory::A)>(reg);
}
template <typename T>
	requires is_digits<T>
void ora(T &reg) {
	/*
	 * Same as above, only OR
	 */
	memory::A = memory::A | static_cast<uint8_t>(reg);
}

template <typename T>
	requires is_digits<T>
void inr(T &reg) {
	if (std::numeric_limits<T>::digits + 1 < reg + 1) // value is greater than register size, so set carry flag
		cpu_handler::set_carry_flag(reg);
	reg++;
}

template <typename T, typename T2> void mov(T &register_one, T2 &register_two) {
	if (std::is_same<T, uint8_t>() && std::is_same<T2, uint16_t>())
		return; // throwing invalid assembly instruction l8ter
	// register_one = register_two;
	register_one = register_two;
	cpu_handler::CF = (register_one == 0) ? 1 : 0;
	register_two = 0; // emptying register
}

}; // namespace cpu_instructions
