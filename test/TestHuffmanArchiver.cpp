#include "HuffmanArchiver.h"

#include "doctest.h"
#include <sstream>

void EndToEndTesting(std::stringstream& input) {
	std::stringstream binaryStreamforCompressData;
	std::stringstream output;
	Archiver::HuffmanArchiver CompressArchiver(input, binaryStreamforCompressData);
	Archiver::Statistics CompressStats = CompressArchiver.compress();
	Archiver::HuffmanArchiver ExtractArchiver(binaryStreamforCompressData, output);
	Archiver::Statistics ExtractStats = ExtractArchiver.extract();
	CHECK(input.str() == output.str());
	CHECK(CompressStats.sizeOfExtractData == ExtractStats.sizeOfExtractData);
	CHECK(CompressStats.sizeOfCompressData == ExtractStats.sizeOfCompressData);
	CHECK(CompressStats.sizeOfExtraData == ExtractStats.sizeOfExtraData);
}

TEST_CASE("Сheck Write/Read of Frequency Table") {
	SUBCASE("Empty table") {
		Archiver::FrequencyTable correctTable;
		Archiver::FrequencyTable testTable;
		std::stringstream s;
		correctTable.write(s);
		s.seekg(0, s.beg);
		testTable.read(s);
		CHECK(testTable.extractTableByMove() == correctTable.extractTableByMove());
	}
	SUBCASE("Non-empty table") {
		Archiver::FrequencyTable correctTable (std::unordered_map<uint8_t, uint64_t>{
			{'a', 100101},
			{'b', 324241},
			{'c', 100000001},
			{'d', 1244212},
			{'e', 1241243},
			{'t', 2343341}
		});
		Archiver::FrequencyTable testTable;
		std::stringstream s;
		correctTable.write(s);
		s.seekg(0, s.beg);
		testTable.read(s);
		CHECK(testTable.extractTableByMove() == correctTable.extractTableByMove());
	}
}

TEST_CASE("End-to-End tests and check statistics") {
	SUBCASE("Empty") {
		std::stringstream input;
		EndToEndTesting(input);
	}
	SUBCASE("One type of symbol, text") {
		uint8_t symbol = 99;
		std::stringstream input;
		for (size_t i = 0; i < 100000; ++i) {
			input << symbol;
		}
		EndToEndTesting(input);
	}
	SUBCASE("One type of symbol, binary") {
		uint8_t symbol = 99;
		std::stringstream input;
		for (size_t i = 0; i < 100000; ++i) {
			input.put(symbol);
		}
		EndToEndTesting(input);
	}
	SUBCASE("Usual data, text") {
		std::stringstream input{"qwrewtrytrytuyiouidsfghjkjlzvxbcvnmn,.retyryjtkuyiuretyyukyiuerwteyruuyiEWARSRDHTFJYGKHdszfDXGGHMJwryetfyjgdgfnhmjr[topeurt98603673471747534"};
		EndToEndTesting(input);
	}
	SUBCASE("Usual data, binary") {
		std::string test = "qwrewtrytrytuyiouidsfghjkjlzvxbcvnmn,.retyryjtkuyiuretyyukyiuerwteyruuyiEWARSRDHTFJYGKHdszfDXGGHMJwryetfyjgdgfnhmjr[topeurt98603673471747534";
		std::stringstream input;
		uint8_t null = 0;
		input.write(test.c_str(), test.length());
		input.put(null);
		EndToEndTesting(input);
	}
}
