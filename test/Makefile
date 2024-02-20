PROGS = generic standard
SRCS_generic = generic.cc
SRCS_standard = standard.cc

CXX = clang++-17

#CXXFLAGS += -g -Wno-deprecated
CXXFLAGS += -O3 -Wno-deprecated

CXXFLAGS += -std=c++20
CXXFLAGS += -I ../
CXXFLAGS += -I generic_dijkstra
CXXFLAGS += -I graph
CXXFLAGS += -I props
CXXFLAGS += -I standard_dijkstra
CXXFLAGS += -I units

.include <bsd.prog.mk>
