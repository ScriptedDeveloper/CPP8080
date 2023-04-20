#pragma once
#include "disassemble.hpp"
#include "error.hpp"
#include <stack>
#include <iostream>

namespace cpu_instructions {
/*
 * Instruction handlers
 */
extern bool nop();
extern void mvi(uint8_t val, uint8_t &reg);	
extern void push(uint8_t &reg); // self explanatory
extern void pop();

template <typename T, typename T2> void mov(T &register_one, T2 &register_two) {
	if (std::is_same<T, uint8_t>() && std::is_same<T2, uint16_t>())
		return; // throwing invalid assembly instruction l8ter
	//register_one = register_two;
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
	extern std::unordered_map<std::string_view, uint8_t> register_params; // extra params for the registers, could also use the tuple for this
									 // Will change this from string to char later!
};

class cpu_handler {
  public:
	cpu_handler(std::vector<disassembler_globals::AnyTuple> &tuple)
		: tuple_instructions(tuple){

		  };
	virtual ~cpu_handler(){

	};
	bool handle_instructions();


  private:
	std::vector<disassembler_globals::AnyTuple> tuple_instructions{};
};
