# HuffmanArchiver
Console archiver

# Build
* Run:
```bash
make Archiver
```
* Executable file is Archiver in current directory

# Command Line Options
* Supports four command line flags:
    - ```-c```: compress
    - ```-u```: extract
    - ```-f```, ```--file <path>```: input filename
    - ```-o```, ```--output <path>```: output filename
* Parameter value(if there's) should be written after whitespace
* Parameters may be in any order, excess parameters are prohibited

# How to use
Just run executable file with correct flags. For example, if you want to compress input.txt, run:
```bash
./Archiver -c -f input.txt -o result.bin
```


