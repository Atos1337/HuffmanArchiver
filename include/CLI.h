#pragma once
#include <vector>
#include <optional>
#include <cstdint>
#include <string>
#include <stdexcept>

namespace CLI {

using CLIException = std::logic_error;

enum MODE{
	COMPRESS,
	EXTRACT
};

struct Params final {
	std::optional<MODE> mode = std::nullopt;
	std::optional<std::string> inputFile = std::nullopt;
	std::optional<std::string> outputFile = std::nullopt;
};

Params parseArgs(const std::vector<std::string>& args);

} //namespace CLI
