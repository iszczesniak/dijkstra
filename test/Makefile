SRCS != ls *.cc
PROGS = $(SRCS:R)

CXX = clang++-19

# CXXFLAGS += -g -Wno-deprecated
CXXFLAGS += -O3 -Wno-deprecated

CXXFLAGS += -std=c++2c
CXXFLAGS += -I ../
CXXFLAGS += -I generic_dijkstra
CXXFLAGS += -I graph
CXXFLAGS += -I props
CXXFLAGS += -I standard_dijkstra
CXXFLAGS += -I units

# Run the tests.
all: $(PROGS)
	@for i in $(PROGS); do echo "Running" $$i; ./$$i; done
