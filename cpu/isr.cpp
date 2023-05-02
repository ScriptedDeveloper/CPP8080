#include "isr.hpp"
#include "../error/error.hpp"

//std::vector<std::function<bool()>> isr::ivt{}; // interrupt vector table


bool isr::RST(uint8_t num) {
	switch(num) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		default: exception::invalid_asm();

	} // i really didnt want to use switch here, but its better than unreadable constexpr C++ code
	return true;
}
