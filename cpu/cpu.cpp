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
			exception::invalid_asm();
		}
	}
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

void cpu_instructions::ei() { cpu_handler::interrupts_enabled = true; }

void cpu_instructions::di() { cpu_handler::interrupts_enabled = false; }

void cpu_instructions::hlt() { cpu_handler::is_hlt = true; }
