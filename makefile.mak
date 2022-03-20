CXX = g++
CXXFLAGS = -g -Wall -std=c++11
OFILES = pe.o decode_cgra.o

all : decode_cgra

pe.o	:	pe.cpp pe.h
	$(CXX) $(CXXFLAGS) -c pe.cpp -o pe.o

decode_cgra.o	:	decode_cgra.cpp pe.h
	$(CXX) $(CXXFLAGS) -c decode_cgra.cpp -o decode_cgra.o

decode_cgra	: $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o decode_cgra

clean:
	@echo "Cleared"
	@/bin/rm -f *.o *.x *.cc~ *.h~ decode_cgra