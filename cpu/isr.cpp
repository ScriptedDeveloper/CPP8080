#include "isr.hpp"

bool isr::RST(uint8_t num) {
	if (num > 7)
		exception::invalid_asm();
	if (!cpu_handler::interrupts_enabled)
		return false;
	cpu_handler::interrupts_enabled = false;			 // intel 8080 doesnt support stacked interrupts
	uint16_t addr{static_cast<uint16_t>((num * 8) - 1)}; // according to docs n * 8
														 // - 1 so it corresponds to the map val
	cpu_instructions::call(addr);
	return true;
}
