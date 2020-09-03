#include "BitBuffer.h"

#include "doctest.h"
#include <sstream>
#include <unordered_map>
#include <cstring>
#include <cstdint>
#include <vector>
#include <climits>

TEST_CASE("Check BitWrite") {
	SUBCASE("Without flush") {
		std::stringstream s;
		std::vector<bool> bites = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1};
		BitBuffer::Writer writer(s);
		for (bool b : bites) {
			writer.write(b);
		}
		s.seekg(0, s.beg);
		CHECK(s.get() == 0b01011010);
		CHECK(s.get() == 0b10011110);
		CHECK(s.get() == 0b10101001);
	}
	SUBCASE("With flush") {
		std::stringstream s;
		std::vector<bool> bites = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1};
		BitBuffer::Writer writer(s);
		for (bool b : bites) {
			writer.write(b);
		}
		writer.flush();
		s.seekg(0, s.beg);
		CHECK(s.get() == 0b01011010);
		CHECK(s.get() == 0b10011110);
		CHECK(s.get() == 0b10101001);
		CHECK(s.get() == 0b01101001);
		CHECK(s.get() == 0b11);
	}
} 

TEST_CASE("Check BitRead") {
	uint8_t symbol1 = 0b00101010;
	uint8_t symbol2 = 0b110;
	std::stringstream s;
	s.put(symbol1);
	s.put(symbol2);
	BitBuffer::Reader reader(s);
	s.seekg(0, s.beg);
	CHECK(!reader.getBit());
	CHECK(reader.getBit());
	CHECK(!reader.getBit());
	CHECK(reader.getBit());
	CHECK(!reader.getBit());
	CHECK(reader.getBit());
	CHECK(!reader.getBit());
	CHECK(!reader.getBit());
	CHECK(!reader.getBit());
	CHECK(reader.getBit());
	CHECK(reader.getBit());
	CHECK(!reader.getBit());
	CHECK(!reader.getBit());
	CHECK(!reader.getBit());
}
