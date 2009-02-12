#ifndef _TILE_HPP
#define _TILE_HPP

//#include <gtk/gtk.h>
#include <cairo.h>
#include "block.hpp"

#define TILE_SIZE 4

//#define MAX(a,b) (((a)>(b))?(a):(b))

class Board;

enum TileType {
   TILE_I,
   TILE_J,
   TILE_L,
   TILE_O,
   TILE_S,
   TILE_T,
   TILE_Z
};

enum Rotation {
   ROT_0,
   ROT_90,
   ROT_180,
   ROT_270
};

#include "board.hpp"

class Tile {

public:
   Tile(TileType _type);

   bool setPosition(int _x, int _y);
   int  getX();
   int  getY();
   int  getWidth();
   int  getHeight();

   int  getTop();
   int  getTop(Rotation rot);
   int  getLeft();
   int  getLeft(Rotation rot);
   int  getRight();
   int  getRight(Rotation rot);

   void rotate();
   bool moveDown();
   void moveLeft();
   void moveRight();

   static BlockColor colorForType(TileType t);

   static void draw(cairo_t *cr, int x, int y, TileType t, Rotation r, BlockColor c);
   static void draw(cairo_t *cr, int x, int y, TileType t);
   void draw(cairo_t *cr, int x, int y);
   void draw();

   void decay();

   bool posTest(int _x, int _y, Rotation rot);
   bool posTest(int _x, int _y);

   void setBoard(Board *b);


private:
   void _init(int _width, int _height, BlockColor _color);
   static void loadSvgFiles();

   int width, height;
   int x, y;
   TileType type;
   BlockColor color;
   Rotation rotation;
   Board *board;




};

#endif /* _TILE_HPP */
