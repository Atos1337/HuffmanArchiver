#include "BitBuffer.h"
#include <climits>
#include <cassert>

namespace BitBuffer {

Writer::Writer(std::ostream& output) : out(output) {}

Writer::~Writer() {
	assert(!pos);
}

void Writer::write(bool bit) {
	if (bit) {
		buf ^= (1 << pos);
	}
	++pos;
	if (pos == CHAR_BIT) {
		out.put(buf);
		buf = 0;
		pos = 0;
	}
}

void Writer::flush() {
	if (pos) {
		out.put(buf);
		buf = 0;
		pos = 0;
	}
}

Reader::Reader(std::istream& input) : in(input) {}

bool Reader::getBit() {
		if (!pos)
			buf = in.get();
		bool bit = ((buf >> pos) & 1);
		pos = (pos == 7 ? 0 : pos + 1);
		return bit;
}

} //namespace BitBuffer
