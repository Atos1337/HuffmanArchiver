#pragma once
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "BitBuffer.h"

namespace Huffman {

class HuffmanTree final {
public:
	HuffmanTree(const std::unordered_map<uint8_t, uint64_t>& frequencies);
	std::unordered_map<uint8_t, std::vector<bool>> buildCodes() const;
	uint8_t getSymbol(BitBuffer::Reader& reader) const;
private:
	class HuffmanNode final {
	public:
		HuffmanNode(uint8_t symbol, uint64_t frequency);
		HuffmanNode(uint8_t symbol, uint64_t frequency, std::unique_ptr<HuffmanNode> leftChild, std::unique_ptr<HuffmanNode> rightChild);
		bool isLeaf() const noexcept;
		void getCodes(std::unordered_map<uint8_t, std::vector<bool>>& codes, std::vector<bool>& code) const;
		uint8_t getLeaf(BitBuffer::Reader& reader) const;
		uint64_t getFrequency() const noexcept;
		uint8_t getSymbol() const noexcept;
	private:	
		uint8_t symbol_;
		uint64_t frequency_;
		std::unique_ptr<HuffmanNode> leftChild_;
		std::unique_ptr<HuffmanNode> rightChild_;
	};
	std::unique_ptr<HuffmanNode> root;
};

} //namespace Huffman
