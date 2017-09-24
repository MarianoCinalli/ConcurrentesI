# Variables
 SOURCES := $(shell find src -name '*.cpp')
 TEMP := $(subst src/,obj/,$(SOURCES))
 OBJECTS := $(subst .cpp,.o,$(TEMP))
 COMPILER := g++
 LIBRARIES := 
 LINKING_FLAGS := $(LIBRARIES) -Wall -I headers/ -o
 COMPILING_FLAGS := -g -std=c++11 -Wall -I headers/ -c -o

# Target
 all: directories main

 main: $(OBJECTS)
	$(COMPILER) $(LINKING_FLAGS) $@ $^ $(LIBRARIES)

 directories:
	mkdir -p ./obj
	find src -type d | sed -e 's/src/obj/g' | xargs mkdir -p

# Pattern
 obj/%.o: src/%.cpp
	$(COMPILER) $(COMPILING_FLAGS) $@ $<

# Clean object files and binary
 clean:
	rm -r ./obj
	rm main

.PHONY: all clean main