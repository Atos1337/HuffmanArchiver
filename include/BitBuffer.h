#pragma once
#include <fstream> 
#include <unordered_map>
#include <vector>

namespace BitBuffer {

class Writer final {
public:
	Writer(std::ostream& output);
	void write(bool bit);
	void flush();
	~Writer();

	Writer(const Writer&) = delete;
	Writer(Writer&&) = delete;
	Writer& operator=(const Writer&) = delete;
	Writer& operator=(Writer&&) = delete;
private:
	std::ostream& out;
	uint8_t buf = 0;
	uint8_t pos = 0;
};

class Reader final {
public:
	Reader(std::istream& input);
	bool getBit();
private:
	std::istream& in;
	uint8_t buf = 0;
	uint8_t pos = 0;
};

} //namespace BitBuffer