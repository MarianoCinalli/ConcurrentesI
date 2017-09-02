# Variables
 SOURCES := $(shell find src -name '*.cpp')
 TEMP := $(subst src/,obj/,$(SOURCES))
 OBJECTS := $(subst .cpp,.o,$(TEMP))
 COMPILER := g++
 LIBRARIES := -lSDL2
 LINKING_FLAGS := $(LIBRARIES) -Wall -I headers/ -o
 COMPILING_FLAGS := $(LIBRARIES) -Wall -I headers/ -c -o

# Target
 all: directories main

 main: $(OBJECTS)
	$(COMPILER) $(LINKING_FLAGS) $@ $^

 directories:
	mkdir ./obj
	find src -type d | tr src obj | xargs mkdir -p

# Pattern
 obj/%.o: src/%.cpp
	$(COMPILER) $(COMPILING_FLAGS) $@ $<

# Clean object files and binary
 clean:
	rm -r ./obj
	rm main

.PHONY: all clean main

