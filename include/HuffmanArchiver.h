#pragma once
#include <fstream>
#include <unordered_map>
#include "BitBuffer.h"

namespace Archiver {

struct Statistics final {
	size_t sizeOfExtraData = 0;
	size_t sizeOfExtractData = 0;
	size_t sizeOfCompressData = 0;
};

class FrequencyTable final {
public:
	FrequencyTable(std::unordered_map<uint8_t, uint64_t> frequencies_ = {});
	FrequencyTable(std::istream& in, size_t fileSize);
	void write(std::ostream& out) const;
	void read(std::istream& in);
	std::unordered_map<uint8_t, uint64_t> extractTableByMove() noexcept;//после вызова этого метода дальнейшее использование этого объекта не предполагается
private:
	std::unordered_map<uint8_t, uint64_t> frequencies;
};

class HuffmanArchiver final {
public:
	HuffmanArchiver(std::istream& input, std::ostream& output);
	Statistics compress();
	Statistics extract();
private:
	void writeExtraData(const FrequencyTable& table);
	std::unordered_map<uint8_t, uint64_t> readExtraData();
	std::istream& in;
	std::ostream& out;
	Statistics sizes;
};

} //namespace Archiver
