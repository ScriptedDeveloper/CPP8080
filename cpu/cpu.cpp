#include "cpu.hpp"

uint8_t memory::A{};
uint8_t memory::B{};
uint8_t memory::C{};
uint8_t memory::D{};
uint8_t memory::E{};
uint8_t memory::H{};
uint8_t memory::L{};
uint16_t memory::PC{};
uint16_t memory::SP{}; // stack grows from down to upper (in most cases)

bool cpu_handler::CF{};
bool cpu_handler::ZF{};
bool cpu_handler::S{};
bool cpu_handler::CY{};
bool cpu_handler::AC{};
bool cpu_handler::I{};
bool cpu_handler::T{};
bool cpu_handler::interrupts_enabled{};
bool cpu_handler::is_hlt{};

std::stack<uint8_t> memory::stack{}; // Stack of CPU

void cpu_handler::print_registers() {
	using namespace memory;
	/*
	We have to use static_cast here, otherwise the console output will be interpreted
	as a char(ascii) value due to the uint8_t type.
	*/
	std::cout << "Execution complete ! " << std::endl
			  << "Register values (hex)" << std::endl
			  << "A : " << std::hex << static_cast<int>(A) << std::endl
			  << "B : " << std::hex << static_cast<int>(B) << std::endl
			  << "C : " << std::hex << static_cast<int>(C) << std::endl
			  << "D : " << std::hex << static_cast<int>(D) << std::endl
			  << "E : " << std::hex << static_cast<int>(E) << std::endl
			  << "H : " << std::hex << static_cast<int>(H) << std::endl
			  << "L : " << std::hex << static_cast<int>(L) << std::endl
			  << "PC : " << std::hex << static_cast<int>(PC) << std::endl
			  << "SP : " << std::hex << static_cast<int>(SP) << std::endl;

#ifdef __WIN32
	do {
		std::cout << "Press any key to continue." << std::endl;
	} while (std::cin.get() != '\n');
#endif
}

bool cpu_handler::handle_instructions() {
	if (tuple_instructions.empty())
		exception::invalid_format(); // why would you run an empty executeable?
	for (; memory::PC <= tuple_instructions.rbegin()->first;) {
		try {
			if (cpu_handler::is_hlt)
				continue; // cpu is halted
			uint16_t previous_pc = memory::PC;
			auto instruction = tuple_instructions[memory::PC];
			auto ptr = std::get<1>(instruction);
			auto param = std::get<2>(instruction);
			ptr(param);
			if (memory::PC == previous_pc)
				memory::PC += ((std::get<3>(instruction) * 2) + 1);

		} catch (...) {
			int temp{};
			exception::invalid_asm(temp);
		}
	}
	print_registers(); // at the end of execution, we should print the register values to the console

	return true;
}

bool cpu_instructions::nop() { return true; }

// void cpu_instructions::mvi(uint8_t val, uint8_t &reg) { reg = val; }

void cpu_instructions::push(uint8_t &reg) {
	memory::stack.push(reg);
	memory::SP++;
	reg = 0;
}

void cpu_instructions::pop(uint8_t &reg) {
	reg = memory::stack.top();
	memory::stack.pop();
	memory::SP--;
}

void cpu_instructions::jmp(uint16_t &addr) { memory::PC = addr; }

void cpu_instructions::call(uint16_t &addr) {
	memory::stack.push(memory::PC + 3); // we dont wanna have a call loop
	memory::PC = addr;
}

void cpu_instructions::ret() {
	if (memory::stack.empty())
		return; // Issue here : function gets invoked twice, so it will result in a segfault
	memory::PC = memory::stack.top();
	memory::stack.pop();
}

void cpu_instructions::cmc() {
	/*
	 * Sets carry flag to opposite value.
	 */
	cpu_handler::CF = !cpu_handler::CF;
}

void cpu_instructions::jc(uint16_t addr) {
	if (!cpu_handler::CF)
		return; // carry flag not set
	jmp(addr);
}

void cpu_instructions::jnc(uint16_t addr) {
	if (cpu_handler::CF)
		return;
	jmp(addr);
}

void cpu_instructions::jz(uint16_t addr) {
	if (!cpu_handler::CF)
		return;
	jmp(addr);
}

void cpu_instructions::jnz(uint16_t addr) {
	if (cpu_handler::CF)
		return;
	jmp(addr);
}

void cpu_instructions::out(uint8_t &device_list) {
	switch (device_list) { // an unordered_map rn is a little too overkill
	case 1: {
		std::cout << std::hex << static_cast<int>(memory::A) << std::endl;
		break;
	}
	default: {
		std::string_view opcode("0xD3");
		exception::invalid_asm(opcode);
	}
	};
}

void cpu_instructions::in(uint8_t &device_list) {
	switch (device_list) { // same reason as above
	case 1: {
		std::cin >> memory::A;
		memory::A -= '0';
		break;
	}
	default: {
		std::string_view opcode("0xDB");
		exception::invalid_asm(opcode);
	}
	}
}

void cpu_instructions::xra(uint8_t &reg) {
	/*
	 * Performs logical XOR with register A + reg
	 * Saves result in accumulator (register A)
	 */
	memory::A = memory::A ^ reg;
}

void cpu_instructions::ana(uint8_t &reg) {
	/*
	 * Same as above, only ANA
	 */
	memory::A = memory::A & reg;
}

void cpu_instructions::ora(uint8_t &reg) {
	/*
	 * Same as above, only OR
	 */
	memory::A = memory::A | reg;
}

void cpu_instructions::ei() { cpu_handler::interrupts_enabled = true; }

void cpu_instructions::di() { cpu_handler::interrupts_enabled = false; }

void cpu_instructions::hlt() { cpu_handler::is_hlt = true; }
