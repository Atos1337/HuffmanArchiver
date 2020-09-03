#include "CLI.h"

#include "doctest.h"
#include <vector>
#include <string>

TEST_CASE("Check exceptions") {
	SUBCASE("Parameter without flag1") {
		std::vector<std::string> args = {
			"file.txt"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Parameter without flag2") {
		std::vector<std::string> args = {
			"-c",
			"-f",
			"file.txt",
			"-test.txt",
			"-o",
			"result.bin"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	} 
	SUBCASE("Duplicate mode flag") {
		std::vector<std::string> args = {
			"-u",
			"-c"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Duplicate flag -f") {
		std::vector<std::string> args = {
			"-c",
			"-f",
			"file.txt",
			"-f",
			"-other.txt",
			"-o"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Duplicate flag -o") {
		std::vector<std::string> args = {
			"-c",
			"-o",
			"file.txt",
			"-o",
			"-other.txt",
			"-f"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Flag -f without parameter") {
		std::vector<std::string> args = {
			"-c",
			"-o",
			"file.bin",
			"--file"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Unknown flag") {
		std::vector<std::string> args = {
			"--ffdsd",
			"-u",
			"-o"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Unknown flag") {
		std::vector<std::string> args = {
			"-f",
			"file.txt",
			"-o",
			"result.bin",
			"-c",
			"--svsvs"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
	SUBCASE("Not enough flags to run programm") {
		std::vector<std::string> args = {
			"-f",
			"file.txt",
			"-o",
			"result.bin"
		};
		CHECK_THROWS_AS(CLI::parseArgs(args), const CLI::CLIException &);
	}
}

TEST_CASE("Check run result") {
	SUBCASE("-mode -input -output") {
		std::vector<std::string> args = {
			"-c",
			"-f",
			"file.txt",
			"-o",
			"result.bin"
		};
		CLI::Params p = CLI::parseArgs(args);
		CHECK(p.inputFile.value() == "file.txt");
		CHECK(p.outputFile.value() == "result.bin");
		CHECK(p.mode.value() == CLI::COMPRESS);
	}
	SUBCASE("-output -mode -input") {
		std::vector<std::string> args = {
			"--output",
			"result.bin",
			"-c",
			"-f",
			"file.txt"
		};
		CLI::Params p = CLI::parseArgs(args);
		CHECK(p.inputFile.value() == "file.txt");
		CHECK(p.outputFile.value() == "result.bin");
		CHECK(p.mode.value() == CLI::COMPRESS);
	}
	SUBCASE("-input -output -mode") {
		std::vector<std::string> args = {
			"--file",
			"file.txt",
			"-o",
			"result.bin",
			"-u"
		};
		CLI::Params p = CLI::parseArgs(args);
		CHECK(p.inputFile.value() == "file.txt");
		CHECK(p.outputFile.value() == "result.bin");
		CHECK(p.mode.value() == CLI::EXTRACT);
	}
}
