CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic
SRC      = $(wildcard src/*.cpp)
HDR      = $(wildcard src/*.h)

aquita: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o aquita

run: aquita
	./aquita

clean:
	rm -f aquita

.PHONY: run clean
