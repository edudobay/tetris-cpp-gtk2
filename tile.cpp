#include <cmath>
#include <cstdio>
#include <cstring>
#include "tetris.hpp"
#include "tile.hpp"

char tile_masks[][4][TILE_SIZE][TILE_SIZE] = {
   /* TILE_I */
   { { {0, 0, 0, 0},
       {1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0} } },

   /* TILE_J */
   { { {1, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 0} },

     { {0, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} } },

   /* TILE_L */
   { { {0, 0, 1, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 1, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0} } },
     
   /* TILE_O */
   { { {0, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0} } },

   /* TILE_S */
   { { {0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 0} } },

   /* TILE_T */
   { { {0, 1, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} } },

   /* TILE_Z */
   { { {1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0} },

     { {0, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0} },

     { {0, 0, 1, 0},
       {0, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0} } },
};

Tile::Tile(TileType _type)
   : type(_type)
{
   switch (type) {
   case TILE_I:
      _init(4, 1, colorForType(type));
      break;

   case TILE_J:
      _init(3, 2, colorForType(type));
      break;

   case TILE_L:
      _init(3, 2, colorForType(type));
      break;

   case TILE_O:
      _init(2, 2, colorForType(type));
      break;

   case TILE_S:
      _init(3, 2, colorForType(type));
      break;

   case TILE_T:
      _init(3, 2, colorForType(type));
      break;

   case TILE_Z:
      _init(3, 2, colorForType(type));
      break;

   }

}

BlockColor
Tile::colorForType(TileType t)
{
   switch (t) {
      case TILE_I: return COLOR_PURPLE;
      case TILE_J: return COLOR_RED;
      case TILE_L: return COLOR_GREEN;
      case TILE_O: return COLOR_ORANGE;
      case TILE_S: return COLOR_YELLOW;
      case TILE_T: return COLOR_BLUE;
      case TILE_Z: return COLOR_CYAN;
   }

   fprintf(stderr, "invalid type! should not get here\n");
   return (BlockColor)0;
}

void
Tile::_init(int _width, int _height, BlockColor _color)
{
   width = _width;
   height = _height;
   color = _color;
   rotation = ROT_0;
}

void
Tile::draw(cairo_t *cr, int x, int y, TileType t, Rotation r, BlockColor c)
{
   int i, j;

   for (i = 0; i < TILE_SIZE; i++)
      for (j = 0; j < TILE_SIZE; j++)
         if (tile_masks[t][r][i][j]) {
            Block::draw(cr, x+j, y+i, c);
         }
}

void
Tile::draw(cairo_t *cr, int x, int y, TileType t)
{
   draw(cr, x, y, t, ROT_0, colorForType(t));
}

void
Tile::draw(cairo_t *cr, int x, int y)
{
   draw(cr, x, y, type, rotation, color);
}

void
Tile::draw()
{
   draw(board->getContext(), x, y);
}

bool
Tile::setPosition(int _x, int _y)
{
   if (posTest(_x, _y)) {
      x = _x;
      y = _y;
      return true;
   }

   return false;
}

void
Tile::rotate()
{
   Rotation next = (rotation == ROT_270) ? ROT_0 : (Rotation)((int)rotation + 1);
   if (posTest(x, y, next))
      rotation = next;
}

int
Tile::getX()
{
   return x;
}

int
Tile::getY()
{
   return y;
}

int
Tile::getWidth()
{
   if (rotation == ROT_90 || rotation == ROT_270)
      return height;
   else
      return width;
}

int
Tile::getHeight()
{
   if (rotation == ROT_90 || rotation == ROT_270)
      return width;
   else
      return height;
}

int
Tile::getTop(Rotation rot)
{
   int i, j;
   for (i = 0; i < TILE_SIZE; i++)
      for (j = 0; j < TILE_SIZE; j++)
         if (tile_masks[type][rot][i][j])
            return i;

   fprintf(stderr, "warning: empty tile found for type %d rotation %d\n", type, rot);
   return 0;
}

int
Tile::getTop()
{
   return getTop(rotation);
}

int
Tile::getLeft(Rotation rot)
{
   int i, j;
   for (j = 0; j < TILE_SIZE; j++)
      for (i = 0; i < TILE_SIZE; i++)
         if (tile_masks[type][rot][i][j])
            return j;

   fprintf(stderr, "warning: empty tile found for type %d rotation %d\n", type, rot);
   return 0;
}

int
Tile::getLeft()
{
   return getLeft(rotation);
}

int
Tile::getRight(Rotation rot)
{
   int i, j;
   for (j = TILE_SIZE-1; j >= 0; j--)
      for (i = 0; i < TILE_SIZE; i++)
         if (tile_masks[type][rot][i][j])
            return j;

   fprintf(stderr, "warning: empty tile found for type %d rotation %d\n", type, rot);
   return 0;
}

int
Tile::getRight()
{
   return getRight(rotation);
}

bool
Tile::moveDown()
{
   if (posTest(x, y+1, rotation)) {
      y++;
      return true;
   }
   return false;
}

void
Tile::moveRight()
{
   // if (x + getRight() + 1 < board->getWidth())
   if (posTest(x+1, y, rotation))
      x++;
}

void
Tile::moveLeft()
{
   // if (x + getLeft() - 1 >= 0)
   if (posTest(x-1, y, rotation))
      x--;
}

bool
Tile::posTest(int _x, int _y, Rotation rot)
{
   int i, j;
   for (i = 0; i < TILE_SIZE; i++)
      for (j = 0; j < TILE_SIZE; j++)
         if (tile_masks[type][rot][i][j] && !board->isPositionFree(_x+j, _y+i))
            return false;

   return true;
}

bool
Tile::posTest(int _x, int _y)
{
   return posTest(_x, _y, rotation);
}

void
Tile::setBoard(Board *b)
{
   board = b;
}

void
Tile::decay()
{
   int i, j;
   for (i = 0; i < TILE_SIZE; i++)
      for (j = 0; j < TILE_SIZE; j++)
         if (tile_masks[type][rotation][i][j])
            board->addBlock(new Block(color, x+j, y+i));
}

