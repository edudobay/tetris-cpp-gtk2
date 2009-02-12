#ifndef _BLOCK_HPP
#define _BLOCK_HPP


enum BlockColor {
   COLOR_BLUE,
   COLOR_CYAN,
   COLOR_GREEN,
   COLOR_ORANGE,
   COLOR_PURPLE,
   COLOR_RED,
   COLOR_YELLOW
};


class Board;

class Block {

public:
   Block(BlockColor c);
   Block(BlockColor c, int _x, int _y);

   void setPosition(int _x, int _y);
   bool isAt(int _x, int _y);

   int getX();
   int getY();

   void draw(Board *board, int x, int y);
   static void draw(cairo_t *cr, int x, int y, BlockColor color);
   static void rounded_rectangle(cairo_t *cr,
         double x, double y, double w, double h, double r);

private:
   BlockColor color;
   int x, y;
};

#endif /* _BLOCK_HPP */
