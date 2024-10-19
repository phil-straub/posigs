# Compiler ##########################################################################################

# gcc 14.2.1 and clang 18.1.8 seem to work
cc ::= gcc

# compiler flags shared between debug and release build
# (generate position independent code, use GNU/C23 standard, compile to object file without linking)
CC_FLAGS_BASE ::= -fPIC -std=gnu2x -pthread -c

# flags for debug build (include debug information, optimization level 2, compiler warnings)
CC_FLAGS_DEBUG ::= $(CC_FLAGS_BASE) -g -O2 -Wall -Wpedantic -Wextra -Wno-unused-parameter

# flags for release build (strip symbols, optimization level 3)
CC_FLAGS_RELEASE ::= $(CC_FLAGS_BASE) -s -O3

# change this to $(CC_FLAGS_RELEASE) for release build
CC_FLAGS ::= $(CC_FLAGS_DEBUG)

CC ::= $(cc) $(CC_FLAGS)

# Linker ############################################################################################

ld ::= $(cc)

LD ::= $(ld)

# Object File and Executable ########################################################################
.PHONY: all lib

all: build/posigs

build/posigs: build/main.o build/posigs.o
	$(LD) build/main.o build/posigs.o -o build/posigs

build/main.o: src/*.h src/main.c | build
	$(CC) src/main.c -o build/main.o

build/posigs.o: src/*.h src/posigs.c | build
	$(CC) src/posigs.c -o build/posigs.o

build:
	mkdir -p build

lib: build/posigs.o

# Phony Targets #####################################################################################
.PHONY: clean run

clean:
	rm -f -r build/*

run: build/posigs
	build/posigs
