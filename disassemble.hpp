#pragma once
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <optional>
#include <sstream>
#include <string_view>
#include <tuple>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace disassembler_globals {
using AnyTuple = std::tuple<std::string_view, const std::function<void(uint8_t val)>, uint8_t>; 

};

#include "cpu.hpp"

class disassembler {
  public:
	disassembler(const std::string &filename) {
		if (!std::filesystem::exists(filename)) {
			throw std::invalid_argument("No file named : " + filename);
		}
		/*
		 * Gets machine code, puts into string
		 */
		std::ifstream ifsfile(filename);
		std::stringstream ss_temp;
		ss_temp << ifsfile.rdbuf();
		machine_code = ss_temp.str();
		init_array();
	};
	virtual ~disassembler(){

	};
	// this function tries to separate one opcode from another and returns the vector without the opcode
	std::vector<disassembler_globals::AnyTuple> disassemble();

  private:
	std::string machine_code;
	using AnyVar = std::variant<int, bool, char, uint8_t, void>;
	std::ifstream ifsfile;
	disassembler_globals::AnyTuple find_instruction(const uint8_t &opcode);
	bool correct_opcode(std::vector<disassembler_globals::AnyTuple> &tuple_instructions, uint8_t &current_opcode, int param = UINT8_MAX + 1);
	void init_array();
	template <typename... T> static auto add_digits(T... digits);
	template <typename T> auto char_to_hex(T ch);
	//	std::string get_file_contents();
	static std::unique_ptr<std::unordered_map<uint8_t, disassembler_globals::AnyTuple>>
		opmap; // unordered_map find operation has O(1) while std::find in array has O(n)
};

template <typename... T> auto disassembler::add_digits(T... digits) {
	std::stringstream ss;
	((ss << std::hex << digits), ...);
	return std::stoi(ss.str(), nullptr, 16);
}

template <typename T> auto disassembler::char_to_hex(T ch) {
	if (std::isdigit(ch))
		return static_cast<T>(ch - '0');
	else if (std::isxdigit(ch))
		return static_cast<T>(std::toupper(ch) - 'A' + 10);
	throw std::invalid_argument("Invalid input!");
}
