#include "disassemble.hpp"

std::string disassembler::get_file_contents() {
	std::stringstream ss_file;
	ss_file << ifsfile.rdbuf();
	return ss_file.str();
}

std::string disassembler::disassemble() {
	auto str_contents = get_file_contents();
	return str_contents;
}
