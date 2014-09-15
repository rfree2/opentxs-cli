
cmake-debug:
	mkdir -p build/debug
	# -D COTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1  -D COTIRE_VERBOSE=ON
	( cd build/debug && cmake -D CMAKE_BUILD_TYPE=Debug ../.. )

build-debug: cmake-debug
	( cd build/debug && $(MAKE) )

all-debug: build-debug

debug: all-debug

all: debug

run-debug: debug
	( cd build/debug && echo "how to start the program?" )

run: run-debug

.PHONY: run run-debug all debug all-debug build-debug cmake-debug

