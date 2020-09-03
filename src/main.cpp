#include "HuffmanArchiver.h"
#include "CLI.h"
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {
	std::cout.exceptions(std::cout.failbit | std::cout.badbit);
	try{
		CLI::Params p = CLI::parseArgs(std::vector<std::string>(argv + 1, argv + argc));
		std::ifstream in;
		std::ofstream out;
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		out.exceptions(std::ifstream::failbit | std::ofstream::badbit);
		in.open(p.inputFile.value().c_str(), std::ios::binary);
		out.open(p.outputFile.value().c_str(), std::ios::binary);
		Archiver::HuffmanArchiver h(in, out);
		if (p.mode.value() == CLI::COMPRESS) {
			Archiver::Statistics sizes = h.compress();
			std::cout << sizes.sizeOfExtractData << '\n'
			<< sizes.sizeOfCompressData << '\n'
			<< sizes.sizeOfExtraData << std::endl;
		}
		else {
			Archiver::Statistics sizes = h.extract();
			std::cout << sizes.sizeOfCompressData << '\n'
			<< sizes.sizeOfExtractData << '\n'
			<< sizes.sizeOfExtraData << std::endl;
		}
		return 0;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
} 
