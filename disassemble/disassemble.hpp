#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

namespace disassembler_globals {
using AnyTuple = std::tuple<std::string_view, std::function<void(uint8_t val)>,
							int,  // Param
							float // Amount of bytes to be read (1 hex digit = 0.5 bytes)
							>;
constexpr auto EMPTY_TUPLE = std::make_tuple("", nullptr, UINT16_MAX + 1, 0.0);
}; // namespace disassembler_globals

#include "../cpu/cpu.hpp"

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
	std::map<uint16_t, disassembler_globals::AnyTuple> disassemble();

  private:
	std::string machine_code;
	double i_instruction_find;	// for finding all digits in a >=2 byte instruction
	double i_instruction_max{}; // every instruction has its own max length
	std::map<uint16_t, disassembler_globals::AnyTuple> tuple_instructions{};
	std::vector<disassembler_globals::AnyTuple> tuple_instructions_temp{};
	using AnyVar = std::variant<int, bool, char, uint8_t, void>;
	std::ifstream ifsfile;
	disassembler_globals::AnyTuple find_instruction(const uint8_t &opcode);
	bool add_instruction(int &current_opcode, short &addr_count, std::vector<uint8_t> &last_param, int &param,
						 int &address, disassembler_globals::AnyTuple tuple = disassembler_globals::EMPTY_TUPLE);
	bool validate_opcode(int &current_opcode, double &i_instruction_max, double &i_instruction_find,
						 std::vector<disassembler_globals::AnyTuple> &tuple_instructions_temp, int &param, bool &failed,
						 std::vector<uint8_t> &last_param, short &zero_count);
	bool correct_opcode(std::vector<disassembler_globals::AnyTuple> &tuple_instructions, uint8_t &current_opcode,
						int param = UINT16_MAX + 1);
	void add_last_param(double &i_instruction_find, std::vector<uint8_t> &last_param, short &zero_count,
						int current_opcode, int param);
	uint8_t finish_instruction(int &current_opcode, short &addr_count, std::vector<uint8_t> &last_param, int &param,
							   int &address, disassembler_globals::AnyTuple &tuple);
	bool is_empty_instruction(uint8_t current_opcode, int i_string, short zero_count);
	void init_array();
	bool add_digit(char ch_int, std::stringstream &ss);
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
