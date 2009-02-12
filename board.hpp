#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <gtk/gtk.h>
#include <cairo.h>
#include <vector>
#include "tile.hpp"
#include "block.hpp"

using std::vector;

class Tile;

class Board {

public:

   Board();

   void setContext(cairo_t *c);
   cairo_t *getContext();
   int getWidth();
   int getHeight();

   void resize(int width, int height);
   void setWidget(GtkWidget *w);
   void setPreviewWidget(GtkWidget *w);

   void draw();
   void redraw();
   void redrawPreview();

   void rotate();
   void moveDown();
   void moveLeft();
   void moveRight();
   void drop();
   void nextMove();

   static TileType randomTile();
   bool newTile(TileType t);

   void     setNextTile(TileType t);
   TileType getNextTile();


   bool isPositionFree(int x, int y);
   void addBlock(Block *b);

   void clearLines();
   void reset();
   void updateBlockSize();
   

private:
   void removeLine(int y);

   // vector<Tile*> tiles;
   // vector<Block*> blocks;
   Tile *dropping;
   TileType next;
   cairo_t *cr;
   int width, height;
   Block ***blocks;
   GtkWidget *w_board, *w_next;

};

int random(int m);

#endif /* _BOARD_HPP */
