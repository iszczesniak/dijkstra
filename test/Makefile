TESTS = generic

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

CXXFLAGS := -std=c++20
CXXFLAGS := $(CXXFLAGS) -I ../
CXXFLAGS := $(CXXFLAGS) -I generic_dijkstra
CXXFLAGS := $(CXXFLAGS) -I graph
CXXFLAGS := $(CXXFLAGS) -I standard_dijkstra
CXXFLAGS := $(CXXFLAGS) -I units

# Use the C++ linker
LINK.o = $(LINK.cc)

# Run the tests.
run: $(TESTS)
	@for i in $(TESTS); do echo "Running" $$i; ./$$i; done

count:
	wc -l *.hpp *.cc

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf $(TESTS)

depend:
	g++ -MM $(CXXFLAGS) *.cc > dependencies

include dependencies
