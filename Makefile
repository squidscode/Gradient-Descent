GCC= g++
FLAGS= -std=c++11 -o ./$@ -g
SRCS= $(wildcard test/*.cpp)
DEST= $(patsubst %.cpp,%,$(SRCS))
EXCLUDE= "*.json" "*.cpp" "Makefile" "test/ " "*.h" ".gitignore"
FIND_FLAGS= -maxdepth 1 -type f -not -name "." $(patsubst %,-a -not -name %, $(EXCLUDE))

all: $(DEST)

%: %.cpp
	$(GCC) $(FLAGS) $< ./gradient_descent.cpp

.PHONY: clean run-tests

clean:
	find ./test $(FIND_FLAGS) -exec rm -r {} \;

run-tests:
	$(patsubst %.cpp,%,$(wildcard ./test/*.cpp))
