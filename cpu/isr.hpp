#pragma once
#include "cpu.hpp"
#include <cstdint>
#include <vector>
#include <functional>
#include <array>

class isr {
  public:
	isr(uint16_t addr) {
		cpu_handler::interrupts_enabled = false; // intel 8080 doesnt support stacked interrupts
		memory::stack.push(addr);
	};
	virtual ~isr(){

	};
	//static constexpr std::vector<void(*)(uint8_t)> ivt{
	//}; // interrupt vector table
  private:
	bool RST(uint8_t num);

}; // Interrupt Service Routine
