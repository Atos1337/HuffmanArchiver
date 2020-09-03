#include "Huffman.h"
#include <cassert>
#include <queue>

namespace Huffman {

HuffmanTree::HuffmanNode::HuffmanNode(uint8_t symbol, uint64_t frequency) : 
	symbol_(symbol), frequency_(frequency),
	leftChild_(nullptr), 
	rightChild_(nullptr) {}

HuffmanTree::HuffmanNode::HuffmanNode(uint8_t symbol, uint64_t frequency,
	std::unique_ptr<HuffmanNode> leftChild, 
	std::unique_ptr<HuffmanNode> rightChild) :
	symbol_(symbol), frequency_(frequency),
	leftChild_(std::move(leftChild)),
	rightChild_(std::move(rightChild)) {}

bool HuffmanTree::HuffmanNode::isLeaf() const noexcept{
	return !leftChild_ && !rightChild_;
}

void HuffmanTree::HuffmanNode::getCodes(std::unordered_map<uint8_t, std::vector<bool>>& codes, std::vector<bool>& code) const {
	if (this->isLeaf()) {
		codes[symbol_] = (code.size() ? code : std::vector<bool>(1, true));
		return;
	}
	code.push_back(false);
	leftChild_->getCodes(codes, code);
	code.pop_back();
	code.push_back(true);
	rightChild_->getCodes(codes, code);
	code.pop_back();
}

uint8_t HuffmanTree::HuffmanNode::getLeaf(BitBuffer::Reader& reader) const {
	if (isLeaf())
		return symbol_;
	if (reader.getBit() == false)
		return leftChild_->getLeaf(reader);
	else
		return rightChild_->getLeaf(reader);
}

uint64_t HuffmanTree::HuffmanNode::getFrequency() const noexcept{
	return frequency_;
}

uint8_t HuffmanTree::HuffmanNode::getSymbol() const noexcept {
	return symbol_;
}

HuffmanTree::HuffmanTree(const std::unordered_map<uint8_t, uint64_t>& frequencies) : root(nullptr) {
	auto compare = [](const HuffmanNode *a, const HuffmanNode *b){
		if (a->getFrequency() == b->getFrequency()) {
			if (a->getSymbol() == b->getSymbol())
				return false;
			return a->getSymbol() < b->getSymbol();
		}
		return a->getFrequency() > b->getFrequency();
	};
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(compare)> q(compare);
	for (const auto& symbol : frequencies) {
		q.push(new HuffmanNode(symbol.first, symbol.second));
	}
	uint8_t unique_number = 0;//для однозначности построения
	while(q.size() > 1) {
		HuffmanNode *node1 = q.top();
		q.pop();
		HuffmanNode *node2 = q.top();
		q.pop();
		q.push(new HuffmanNode(unique_number++, node1->getFrequency() + node2->getFrequency(), 
			std::unique_ptr<HuffmanNode>(node1), std::unique_ptr<HuffmanNode>(node2)));//явный вызов конструктора, потому что make_unique нельзя вызвать от указателя
	}
	if (!q.empty())
		root = std::unique_ptr<HuffmanNode>(q.top());
}

std::unordered_map<uint8_t, std::vector<bool>> HuffmanTree::buildCodes() const {
	std::unordered_map<uint8_t, std::vector<bool>> codes;
	std::vector<bool> code;
	if (root)
		root->getCodes(codes, code);
	return codes;
}

uint8_t HuffmanTree::getSymbol(BitBuffer::Reader& reader) const {
	assert(root);
	return root->getLeaf(reader);
}

} //namespace Huffman

