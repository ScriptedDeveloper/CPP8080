#include "disassemble.hpp"

/*
std::string disassembler::get_file_contents() {
	std::stringstream ss_file;
	ss_file << ifsfile.rdbuf();
	return ss_file.str();
}
*/

std::string disassembler::find_opcode(const uint8_t &opcode) {
	auto val = std::find_if(opmap.begin(), opmap.end(), [&](auto &pair) {
		return pair.first == opcode;
			});
	if(val == opmap.end())
		return "";
	return std::string(val->second);
}

int get_digits(std::string &opcode) {
	std::istringstream is(opcode);
	int result{};
	char ch{};
	while(is >> ch, std::isdigit(ch)) {
		int i = ch - '0';
		add_digits(result, i);
	}
	return result;
		
}

std::string disassembler::disassemble() {
	std::string line;
	int first_opcode{};
	while(std::getline(ifsfile, line)) {
		int opcode = get_digits(line);
		

	}
	return "";
}
