DAY_SOURCES = $(wildcard day*.cpp)
DAY_OBJECTS := $(patsubst %.cpp,build/%.o,$(DAY_SOURCES))
DAY_EXECUTABLES := $(patsubst %.cpp,build/%,$(DAY_SOURCES))

CC := clang++
CFLAGS := -std=c++17 -Wall -Wextra -pedantic -Wno-deprecated-declarations -Wno-vla-extension -O3 -march=native -flto -g3 -ftrapv
LIBS :=

.SUFFIXES:
.PHONY: all clean

all: $(DAY_EXECUTABLES)

$(DAY_EXECUTABLES): build/%: build/%.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(DAY_OBJECTS): build/%.o: %.cpp
	@mkdir -p build
	$(CC) $(CFLAGS) -c -MD -o $@ $<
-include $(DAY_OBJECTS:.o=.d)

clean:
	rm -rf build