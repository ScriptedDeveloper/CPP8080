#pragma once
#include "../error/error.hpp"
#include "cpu.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <vector>

class isr {
  public:
	isr(){};
	virtual ~isr(){

	};
	bool RST(uint8_t num);

  private:
}; // Interrupt Service Routine
