#pragma once
#include <fstream>
#include <filesystem>
#include <sstream>

class disassembler {
	public:
		disassembler(const std::string &filename) {
			if(!std::filesystem::exists(filename)) {
				throw std::invalid_argument("No file named : " + filename);
			}
			ifsfile = std::ifstream(filename);
		};
		virtual ~disassembler() {

		};
		std::string disassemble();
		
	private:
		std::ifstream ifsfile;
		std::string get_file_contents();

};
