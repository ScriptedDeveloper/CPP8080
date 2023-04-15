#pragma once
#include <array>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <string_view>
#include <sstream>
#include <variant>
#include <tuple>
#include <vector>
#include <optional>
#include <functional>

namespace disassembler_globals {
	using AnyTuple = std::tuple<uint8_t, std::string_view, std::function<bool()>>;

};

#include "cpu.hpp"

class disassembler {
	public:
		disassembler(const std::string &filename) {
			if(!std::filesystem::exists(filename)) {
				throw std::invalid_argument("No file named : " + filename);
			}
			ifsfile = std::ifstream(filename);
			init_array();
		};
		virtual ~disassembler() {

		};
		std::vector<disassembler_globals::AnyTuple> disassemble();
		
	private:
		using AnyVar = std::variant<int, bool, char, uint8_t, void>;
		std::ifstream ifsfile;
		disassembler_globals::AnyTuple find_opcode(const uint8_t &opcode);
		void init_array();
		template <typename ... T>
		static int add_digits(T ... digits);
	//	std::string get_file_contents();
		static std::array<disassembler_globals::AnyTuple, 2> opmap; // 117


};

template <typename ... T>
int disassembler::add_digits(T ... digits) {
	std::stringstream ss;
	((ss << digits), ...);
	return std::stoi(ss.str());
}

