CFLAGS = -Wall -Wextra -Werror
CCOMPILE = g++ -std=c++17 -O3 -Iinclude

all: hw_03 test_hw_03

hw_03: obj src/main.cpp obj/HuffmanArchiver.o obj/BitBuffer.o obj/Huffman.o obj/CLI.o
	$(CCOMPILE) -o hw_03 $(CFLAGS) src/main.cpp obj/HuffmanArchiver.o obj/BitBuffer.o obj/Huffman.o obj/CLI.o
	
test_hw_03: obj test/TestMain.cpp obj/TestCLI.o obj/TestBitBuffer.o obj/TestHuffmanArchiver.o obj/TestHuffman.o obj/CLI.o obj/BitBuffer.o obj/HuffmanArchiver.o obj/Huffman.o
	$(CCOMPILE) -o test_hw_03 $(CFLAGS) test/TestMain.cpp obj/TestCLI.o obj/TestBitBuffer.o obj/TestHuffmanArchiver.o obj/TestHuffman.o obj/CLI.o obj/BitBuffer.o obj/HuffmanArchiver.o obj/Huffman.o

clean: 
	rm -f hw_03
	rm -f test_hw_03
	rm -rf obj

obj: 
	mkdir obj
	
obj/BitBuffer.o: src/BitBuffer.cpp obj
	$(CCOMPILE) -c $(CFLAGS) src/BitBuffer.cpp -o obj/BitBuffer.o

obj/Huffman.o: src/Huffman.cpp obj/BitBuffer.o obj
	$(CCOMPILE) -c $(CFLAGS) src/Huffman.cpp -o obj/Huffman.o
	
obj/HuffmanArchiver.o: src/HuffmanArchiver.cpp obj/Huffman.o obj/BitBuffer.o obj
	$(CCOMPILE) -c $(CFLAGS) src/HuffmanArchiver.cpp -o obj/HuffmanArchiver.o
	
obj/CLI.o: src/CLI.cpp obj
	$(CCOMPILE) -c $(CFLAGS) src/CLI.cpp -o obj/CLI.o
	
obj/TestCLI.o: obj test/TestCLI.cpp obj/CLI.o
	$(CCOMPILE) -c $(CFLAGS) test/TestCLI.cpp -o obj/TestCLI.o
	
obj/TestBitBuffer.o: obj test/TestBitBuffer.cpp obj/BitBuffer.o
	$(CCOMPILE) -c $(CFLAGS) test/TestBitBuffer.cpp -o obj/TestBitBuffer.o
	
obj/TestHuffmanArchiver.o: obj test/TestHuffmanArchiver.cpp obj/HuffmanArchiver.o
	$(CCOMPILE) -c $(CFLAGS) test/TestHuffmanArchiver.cpp -o obj/TestHuffmanArchiver.o

obj/TestHuffman.o: obj test/TestHuffman.cpp obj/Huffman.o obj/BitBuffer.o
	$(CCOMPILE) -c $(CFLAGS) test/TestHuffman.cpp -o obj/TestHuffman.o
