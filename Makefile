CXX=clang++
INCLUDES=-Iincludes/
CXXFLAGS=-std=c++20 -gdwarf-4 -fstandalone-debug -Wall -Wextra -Werror -pedantic $(INCLUDES)
# CXXFLAGS=-std=c++20 -gdwarf-4 -fstandalone-debug -Wall -Wextra -Werror -pedantic -fsanitize=address,null -fno-omit-frame-pointer $(INCLUDES)

exec: bin/exec

bin/exec: ./src/driver.cc ./src/word.cpp ./includes/word.hpp ./includes/flashcard.hpp ./src/flashcard.cpp ./includes/de_dictionary.hpp ./src/de_dictionary.cpp
	$(CXX) $(CXXFLAGS) ./src/driver.cc ./src/flashcard.cpp ./src/word.cpp ./src/de_dictionary.cpp -o $@

.DEFAULT_GOAL := exec
.PHONY: clean exec tests

clean:
	rm -fr bin/* 