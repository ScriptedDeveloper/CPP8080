#pragma once
#include <algorithm>
#include <arpa/inet.h>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
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
							uint16_t, // Param
							float	  // Amount of bytes to be read (1 hex digit = 0.5 bytes)
							>;
constexpr auto EMPTY_TUPLE = std::make_tuple("", nullptr, UINT16_MAX + 1, 0.0);
}; // namespace disassembler_globals

#include "../cpu/cpu.hpp"
#include "../cpu/isr.hpp"

class disassembler {
  public:
	disassembler(std::string_view filename) {
		if (!std::filesystem::exists(filename)) {
			throw std::invalid_argument(std::string("No file named : ") + filename.data());
		}
		/*
		 * Gets machine code, puts into string
		 */
		std::ifstream ifsfile(filename.data(), std::iostream::binary);
		if (ifsfile.fail())
			throw std::invalid_argument("Could not access file ");
		// std::basic_stringstream<uint8_t> ss_temp;
		// ss_temp << ifsfile.rdbuf();
		auto size = ifsfile.gcount();
		bytes_vec.reserve(size);
		bytes_vec.insert(bytes_vec.begin(), std::istream_iterator<uint8_t>(ifsfile), std::istream_iterator<uint8_t>());
		init_array();
	};
	virtual ~disassembler(){

	};
	// this function tries to separate one opcode from another and returns the vector without the opcode
	std::map<uint16_t, disassembler_globals::AnyTuple> disassemble();

  private:
	std::vector<uint8_t> bytes_vec;
	double i_instruction_find{}; // for finding all digits in a >=2 byte instruction
	double i_instruction_max{};	 // every instruction has its own max length
	std::map<uint16_t, disassembler_globals::AnyTuple> tuple_instructions{};
	std::vector<disassembler_globals::AnyTuple> tuple_instructions_temp{};
	using AnyVar = std::variant<int, bool, char, uint8_t, void>;
	std::ifstream ifsfile;
	disassembler_globals::AnyTuple find_instruction(const uint8_t &opcode);
	template <typename T>
		requires std::is_arithmetic_v<T> bool
	big_to_little_endian(int i_instruction_find, int i_instruction_max, T &param);
	template <typename T>
	bool add_instruction(uint8_t &current_opcode, std::vector<uint16_t> &last_param, uint16_t &param, T &address,
						 double &i_instruction_find, double &i_instruction_max,
						 disassembler_globals::AnyTuple tuple = disassembler_globals::EMPTY_TUPLE,
						 bool skip_endian = false);
	bool validate_opcode(uint8_t &current_opcode, double &i_instruction_max, double &i_instruction_find,
						 std::vector<disassembler_globals::AnyTuple> &tuple_instructions_temp, uint16_t &param,
						 bool &failed, std::vector<uint16_t> &last_param, short &zero_count, uint16_t &address);
	bool correct_opcode(std::vector<disassembler_globals::AnyTuple> &tuple_instructions, uint8_t &current_opcode,
						int param = UINT16_MAX + 1);
	void add_last_param(double &i_instruction_find, std::vector<uint16_t> &last_param, short &zero_count,
						int current_opcode, int param);
	uint8_t finish_instruction(uint8_t &current_opcode, std::vector<uint16_t> &last_param, uint16_t &param,
							   uint16_t &address, disassembler_globals::AnyTuple &tuple);
	bool is_empty_instruction(uint8_t current_opcode, int i_string, short zero_count);
	void init_array();
	bool add_digit(char ch_int, std::stringstream &ss);
	template <typename T, typename T2> auto add_digits(T byte1, T2 byte2);
	template <typename T> auto char_to_hex(T ch);
	//	std::string get_file_contents();
	static std::unique_ptr<std::unordered_map<uint16_t, disassembler_globals::AnyTuple>>
		opmap; // unordered_map find operation has O(1) while std::find in array has O(n)
};

template <typename T, typename T2> auto disassembler::add_digits(T byte1, T2 byte2) { return (byte1 << 4) | byte2; }

template <typename T> auto disassembler::char_to_hex(T ch) {
	if (std::isdigit(ch))
		return static_cast<T>(ch - '0');
	else if (std::isxdigit(ch))
		return static_cast<T>(std::toupper(ch) - 'A' + 10);
	throw std::invalid_argument("Invalid input!");
}

template <typename T>
bool disassembler::add_instruction(uint8_t &current_opcode, std::vector<uint16_t> &last_param, uint16_t &param,
								   T &address, double &i_instruction_find, double &i_instruction_max,
								   disassembler_globals::AnyTuple tuple, bool skip_endian) {
	if (tuple != disassembler_globals::EMPTY_TUPLE) {
		auto tuple = tuple_instructions_temp.back();
		if (!skip_endian)
			big_to_little_endian(i_instruction_find, i_instruction_max, param);
		std::get<2>(tuple) = param;
		tuple_instructions[address] = tuple;
	}
	finish_instruction(current_opcode, last_param, param, address, tuple);
	i_instruction_max = 0;
	i_instruction_find = 0;

	return true;
}

template <typename T>
	requires std::is_arithmetic_v<T> bool
disassembler::big_to_little_endian(int i_instruction_find, int i_instruction_max, T &param) {
	if (i_instruction_find >= 1 && i_instruction_max >= 1) {
		param = ntohs(param); // Will not always work though
		(i_instruction_find == 1) ? i_instruction_find += 1 : int();
		return true;
	}
	return false;
}
