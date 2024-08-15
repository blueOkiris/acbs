# Initial build system for ACBS. Will eventually use itself

# Settings

## Compiler

CPPC :=				g++
CPPFLAGS :=			-Wall -Werror -O2 -std=c++17 -Iinclude
LD :=				g++
LDFLAGS :=

## Project

OBJNAME :=			acbs
SRC :=				$(wildcard src/*.cpp)
HFILES :=			$(wildcard include/*.hpp)
OBJS :=				$(subst .cpp,.o,$(subst src/,obj/,$(SRC)))

# Targets

## Helpers

.PHONY: all
all: $(OBJNAME)

.PHONY: clean
clean:
	rm -rf obj/
	rm -rf $(OBJNAME)

## Main

obj/%.o: src/%.cpp $(HFILES)
	mkdir -p obj/
	$(CPPC) -o $@ $(CPPFLAGS) -c $<

$(OBJNAME): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

