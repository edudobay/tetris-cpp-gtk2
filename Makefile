CXXFLAGS=-Wall $(shell pkg-config gtk+-2.0 cairo --cflags)
LDFLAGS=$(shell pkg-config gtk+-2.0 cairo --libs)

all: tetris

tetris: main.o tile.o board.o block.o tetris.o gui.o
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main.cpp board.hpp gui.hpp tetris.hpp

tile.o: tile.cpp tile.hpp board.hpp block.hpp tetris.hpp

board.o: board.cpp board.hpp block.hpp tile.hpp tetris.hpp

block.o: block.cpp block.hpp board.hpp tetris.hpp

tetris.o: tetris.cpp tetris.hpp board.hpp gui.hpp

# vim:set ts=4 sw=4 noet:
