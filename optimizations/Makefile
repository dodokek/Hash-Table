ifeq ($(OS),Windows_NT)
	OUTPUT_FILE_NAME = main.exe
else
    OUTPUT_FILE_NAME = main
endif

CC = g++
CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -D_DEBUG -D_EJUDGE_CLIENT_SIDE
IFLAGS = -I./include/ 

#main

SRC_DIR = ./
OBJ_DIR = ./lib/
ASM_DIR = ./asm/

SRC   = main.cpp hash_table.cpp file_utils.cpp string_utils.cpp
OBJ   = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRC)) 

#linking
all : $(OBJ) murmurasm.o
	$(CC) $(IFLAGS) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME)

#main
$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp murmurasm.o crc32.o
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -O2 -mavx2 -c $^ -o $@

murmurasm.o : $(ASM_DIR)murmurasm.s 
	nasm -f elf64 -g $(ASM_DIR)murmurasm.s -o murmurasm.o

crc32.o : $(ASM_DIR)crc32.s 
	nasm -f elf64 -g $(ASM_DIR)crc32.s -o crc32.o

clear: 
	rm -f $(OBJ_DIR)*.o