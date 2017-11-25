CC=       	gcc
CFLAGS= 	-g -gdwarf-2 -std=gnu99 -Wall
LIBRARIES=      lib/libmalloc-ff.so \
		lib/libmalloc-nf.so \
		lib/libmalloc-bf.so \
		lib/libmalloc-wf.so
TESTS=	bin/test_00 \
		bin/test_01 \
		bin/test_02 \
		bin/test_03

CPP				= g++
CPPFLAGS		= -g -gdwarf-2 -std=gnu++11 -Wall -Iinclude #-DNDEBUG=1
LD				= g++
LDFLAGS			= -Llib

# These are macros for linking to already compiled gtest libraries.
#GTEST_INC_DIR	= contrib/gtest/googletest/include
#GTEST_LNK_PATH	= build/googlemock/gtest
#GTEST_LNK_FLAGS	= -lgtest -lgtest_main
#GTEST			= build/googlemock/gtest

# Unit tests macros.
#UNIT_FLAGS 		= -I$(GTEST_INC_DIR)
#UNIT_LNK_FLAGS	= -L$(GTEST_LNK_PATH) $(GTEST_LNK_FLAGS) $(LNKFLAGS) -pthread
#UNIT_SOURCE		= $(wildcard src/unit/*.cpp)
#UNIT_HEADERS	= $(wildcard src/unit/*.h)
#UNIT_OBJECTS	= $(UNIT_SOURCE:.cpp=.o)
#UNIT			= bin/client_unit


TARGETS			= $(LIBRARIES) $(TESTS)

all:    		$(TARGETS)

lib/libmalloc-ff.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=0 -o $@ $< $(LDFLAGS)

lib/libmalloc-nf.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=1 -o $@ $< $(LDFLAGS)

lib/libmalloc-bf.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=2 -o $@ $< $(LDFLAGS)

lib/libmalloc-wf.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=3 -o $@ $< $(LDFLAGS)

bin/test_00: tests/test_00.o
	@$(CC) $(CFLAGS) -o $@ $^

tests/test_00.o: tests/test_00.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $^

bin/test_01: tests/test_01.o
	@$(CC) $(CFLAGS) -o $@ $^

tests/test_01.o: tests/test_01.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $^

bin/test_02: tests/test_02.o
	@$(CC) $(CFLAGS) -o $@ $^

tests/test_02.o: tests/test_02.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $^

bin/test_03: tests/test_03.o
	@$(CC) $(CFLAGS) -o $@ $^

tests/test_03.o: tests/test_03.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $^

#$(UNIT):	$(UNIT_OBJECTS)
#	$(LD) -o $@ $(UNIT_OBJECTS) $(LDFLAGS) $(UNIT_LNK_FLAGS)

#$(GTEST):	contrib/gtest
#	(mkdir build; cd build; cmake ../contrib/gtest; make)

#src/unit%.o:	src/unit/%.cpp
#	$(CPP) $(CPPFLAGS) $(UNIT_FLAGS) -o $@ -c $<

# Add clean for object
clean:
	rm -fr build $(TARGETS) tests/*.o

.PHONY: all clean
