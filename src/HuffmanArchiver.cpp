#include <vector>
#include <iostream>
#include "HuffmanArchiver.h"
#include "BitBuffer.h"
#include "Huffman.h"

namespace Archiver {

void writeLeInt64(std::ostream& out, uint64_t value) {
	for (size_t i = 0; i < sizeof(uint64_t); ++i) {
		uint8_t tmp = (value >> (8 * i)) & 255;
		out.put(tmp);
	}
}

void readLeInt64(std::istream& in, uint64_t& value) {
	value = 0;
	for (size_t i = 0; i < sizeof(uint64_t); ++i) {
		value |= static_cast<uint64_t>(in.get()) << (8 * i);
	}
}

void writeSymbol(const std::vector<bool>& code, BitBuffer::Writer& writer) {
	for (bool b : code) {
		writer.write(b);
	}
}

FrequencyTable::FrequencyTable(std::unordered_map<uint8_t, uint64_t> frequencies_) : frequencies(std::move(frequencies_)) {} 

FrequencyTable::FrequencyTable(std::istream& in, size_t fileSize) {
	in.seekg(0, in.beg);
	for (size_t i = 0; i < fileSize; ++i) {
		++frequencies[in.get()];
	}
}

void FrequencyTable::write(std::ostream& out) const {
	out.put(static_cast<uint8_t>(frequencies.size()));
	for (const auto &symbol : frequencies) {
		out.put(symbol.first);
		writeLeInt64(out, symbol.second);
	}
}

void FrequencyTable::read(std::istream& in) {
	uint8_t numberOfSymbols = in.get();
	while(numberOfSymbols--) {
		uint8_t symbol = in.get();
		uint64_t frequency = 0;
		readLeInt64(in, frequency);
		frequencies[symbol] = frequency;
	}
}

std::unordered_map<uint8_t, uint64_t> FrequencyTable::extractTableByMove() noexcept {
	return std::move(frequencies);
}

HuffmanArchiver::HuffmanArchiver(std::istream& input, std::ostream& output) : in(input), out(output) {}

void HuffmanArchiver::writeExtraData(const FrequencyTable& table) {
	writeLeInt64(out, sizes.sizeOfExtractData);
	table.write(out);
}

std::unordered_map<uint8_t, uint64_t> HuffmanArchiver::readExtraData() {
	size_t sizeOfExtractData = 0;
	FrequencyTable table;
	readLeInt64(in, sizeOfExtractData);
	sizes.sizeOfExtractData = sizeOfExtractData;
	table.read(in);
	return table.extractTableByMove();
}

Statistics HuffmanArchiver::compress() {
	using Huffman::HuffmanTree;
	using BitBuffer::Writer;

	in.seekg(0, in.end);
	sizes.sizeOfExtractData = in.tellg();
	in.seekg(0, in.beg);

	FrequencyTable frequencies(in, sizes.sizeOfExtractData);
	writeExtraData(frequencies);

	HuffmanTree tree(frequencies.extractTableByMove());
	std::unordered_map<uint8_t, std::vector<bool>> codes = tree.buildCodes();

	Writer writer(out);
	sizes.sizeOfExtraData = out.tellp();
	in.seekg(0, in.beg);
	for (size_t i = 0; i < sizes.sizeOfExtractData; ++i) {
		uint8_t symbol = in.get();
		writeSymbol(codes[symbol], writer);
	}
	writer.flush();

	size_t sizeOfCompressFile = out.tellp();
	sizes.sizeOfCompressData = sizeOfCompressFile - sizes.sizeOfExtraData;
	return std::move(sizes);
} 

Statistics HuffmanArchiver::extract() {
	using Huffman::HuffmanTree;
	using BitBuffer::Reader;

	in.seekg(0, in.end);
	size_t sizeOfCompressFile = in.tellg();
	in.seekg(0, in.beg);

	HuffmanTree h(readExtraData());
	Reader reader(in);
	sizes.sizeOfExtraData = in.tellg();
	for (size_t i = 0; i < sizes.sizeOfExtractData; ++i) {
		uint8_t symbol = h.getSymbol(reader);
		out.put(symbol);
	}

	sizes.sizeOfCompressData = sizeOfCompressFile - sizes.sizeOfExtraData;
	return std::move(sizes);
}

} //namespace Archiver
