#include "Huffman.h"
#include "BitBuffer.h"

#include "doctest.h"
#include <sstream>
#include <vector>

void writeSymbol(const std::vector<bool>& code, BitBuffer::Writer& writer) {
	for (bool b : code) {
		writer.write(b);
	}
}

TEST_CASE("Check construction of tree") {
	SUBCASE("Empty tree") {
		std::unordered_map<uint8_t, uint64_t> frequencies;
		Huffman::HuffmanTree h(frequencies);
		CHECK(h.buildCodes() == std::unordered_map<uint8_t, std::vector<bool>>());
	}
	SUBCASE("One symbol") {
		std::unordered_map<uint8_t, uint64_t> frequencies = {
			{'a', 11010}
		};
		Huffman::HuffmanTree h(frequencies);
		CHECK(h.buildCodes() == std::unordered_map<uint8_t, std::vector<bool>>{{'a', {true}}});
	}
	SUBCASE("First test on complete matching codes") {
		std::unordered_map<uint8_t, uint64_t> frequencies = {
			{'a', 123},
			{'b', 1323},
			{'c', 110},
			{'d', 222}
		};
		std::unordered_map<uint8_t, std::vector<bool>> correctCodes = {
			{'a', {0, 1, 1}},
			{'b', {1}},
			{'c', {0, 1, 0}},
			{'d', {0, 0}}
		};
		Huffman::HuffmanTree h(frequencies);
		std::unordered_map<uint8_t, std::vector<bool>> testCodes = h.buildCodes();
		for (const auto &c : correctCodes) {
			CHECK(testCodes[c.first] == c.second);
		}
	}
	SUBCASE("Second test on complete matching codes") {
		std::unordered_map<uint8_t, uint64_t> frequencies = {
			{'a', 10},
			{'b', 15},
			{'c', 5},
			{'d', 30}
		};
		std::unordered_map<uint8_t, std::vector<bool>> correctCodes = {
			{'a', {1, 1, 1}},
			{'b', {1, 0}},
			{'c', {1, 1, 0}},
			{'d', {0}}
		};
		Huffman::HuffmanTree h(frequencies);
		std::unordered_map<uint8_t, std::vector<bool>> testCodes = h.buildCodes();
		for (const auto &c : correctCodes) {
			CHECK(testCodes[c.first] == c.second);
		}
	}
	SUBCASE("Test on equal length") {
		std::unordered_map<uint8_t, uint64_t> frequencies;
		for (size_t i = 0; i < (static_cast<uint32_t>(1) << 8); ++i)
			frequencies[i] = 1;
		Huffman::HuffmanTree h(frequencies);
		std::unordered_map<uint8_t, std::vector<bool>> testCodes = h.buildCodes();
		uint8_t lengthOfCodes = 8;
		for (const auto &c : testCodes) {
			CHECK(c.second.size() == lengthOfCodes);
		}
	}
	SUBCASE("Test on length ratio") {
		std::unordered_map<uint8_t, uint64_t> frequencies;
		for (size_t i = 0; i < (static_cast<uint32_t>(1) << 8); ++i) {
			frequencies[i] = i + 1;
		}
		Huffman::HuffmanTree h(frequencies);
		std::unordered_map<uint8_t, std::vector<bool>> testCodes = h.buildCodes();
		for (const auto &c : frequencies) {
			if (c.first != (static_cast<uint32_t>(1) << 8) - 1)
				CHECK(testCodes[c.first].size() >= testCodes[c.first + 1].size());
		}
	}
} 

TEST_CASE("Walk in tree") {//в предположении у том, что BitRead и BitWrite протестированы и работают корректно
	SUBCASE("One symbol") {
		std::stringstream s;
		std::unordered_map<uint8_t, uint64_t> frequencies = {
			{'a', 101010}
		};
		Huffman::HuffmanTree h(frequencies);
		std::unordered_map<uint8_t, std::vector<bool>> codes = h.buildCodes();
		BitBuffer::Writer writer(s);
		writeSymbol(codes['a'], writer);
		writeSymbol(codes['a'], writer);
		writer.flush();
		s.seekg(0, s.beg);
		BitBuffer::Reader reader(s);
		CHECK(h.getSymbol(reader) == 'a');
		CHECK(h.getSymbol(reader) == 'a');
	}
	SUBCASE("Otherwise") {
		std::stringstream s;
		std::unordered_map<uint8_t, uint64_t> frequencies;
		for (size_t i = 0; i < (static_cast<uint32_t>(1) << 8); ++i) {
			frequencies[i] = i + 1;
		}
		Huffman::HuffmanTree h(frequencies);
		std::unordered_map<uint8_t, std::vector<bool>> codes = h.buildCodes();
		BitBuffer::Writer writer(s);
		writeSymbol(codes[43], writer);
		writeSymbol(codes[37], writer);
		writeSymbol(codes[22], writer);
		writeSymbol(codes[0], writer);
		writeSymbol(codes[255], writer);
		writeSymbol(codes[137], writer);
		writer.flush();
		s.seekg(0, s.beg);
		BitBuffer::Reader reader(s);
		CHECK(h.getSymbol(reader) == 43);
		CHECK(h.getSymbol(reader) == 37);
		CHECK(h.getSymbol(reader) == 22);
		CHECK(h.getSymbol(reader) == 0);
		CHECK(h.getSymbol(reader) == 255);
		CHECK(h.getSymbol(reader) == 137);
	}
}
