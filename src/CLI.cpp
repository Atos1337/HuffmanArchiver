#include "CLI.h"
#include <unordered_map>
#include <cassert>

namespace CLI {

bool isFlag(const std::string& arg) {
	assert(arg.length());
	return (arg.length() == 2 && arg.substr(0, 1) == "-") || (arg.length() >= 4 && arg.substr(0, 2) == "--");
}

Params parseArgs(const std::vector<std::string>& args) {
	Params p;
	std::unordered_map<std::string, MODE> modes = {{"-u", EXTRACT}, {"-c", COMPRESS}};
	std::unordered_map<std::string, std::optional<std::string>*> flags = {
		{"-f", &p.inputFile}, 
		{"-o", &p.outputFile}, 
		{"--file", &p.inputFile},
	    {"--output", &p.outputFile}
	}; 
	for (size_t i = 0; i < args.size(); ++i) {
		if (!isFlag(args[i]))
			throw CLIException("Paremeter without flag");
		if (modes.count(args[i]) != 0) {
			if (p.mode)
				throw CLIException("Duplicate mode flag");
			p.mode = modes[args[i]];
		} else if (flags.count(args[i]) != 0) {
			std::optional<std::string> &param = *flags[args[i]];
			if (param)
				throw CLIException("Duplicate flag -f or -o");
			if (i + 1 == args.size())
				throw CLIException("Flag without parameter");
			param = std::move(args[++i]);
		} else
			throw CLIException("Unknown flag");
	}
	if (!p.mode || !p.inputFile || !p.outputFile)
		throw CLIException("Not enough flags to run programm");
	return p;
} 

} //namespace CLI
