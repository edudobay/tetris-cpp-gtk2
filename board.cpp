#include "tetris.hpp"
#include "board.hpp"
#include "tile.hpp"

Board::Board()
   : width(14), height(20)
{
   blocks = NULL;
}

void
Board::setContext(cairo_t *c)
{
   cr = c;
}

cairo_t *
Board::getContext()
{
   return cr;
}

void
Board::draw()
{
   int x, y;
   int block_size = app->getBlockSize();

   if (cr == NULL) {
      fprintf(stderr, "No cairo context bound!\n");
      return;
   }

   cairo_save(cr);
   cairo_rectangle(cr, 0, 0, width * block_size, height * block_size);
   cairo_clip(cr);

   if (dropping)
      dropping->draw();

   for (y = 0; y < height; y++)
      for (x = 0; x < width; x++)
         if (blocks[y][x] != NULL)
            blocks[y][x]->draw(this, x, y);
   
   cairo_restore(cr);
}

void
Board::rotate()
{
   if (dropping) {
      dropping->rotate();
      redraw();
   }
}

void
Board::moveDown()
{
   if (dropping) {
      if (dropping->moveDown())
         redraw();
      else
         nextMove();
   }
}

void
Board::moveLeft()
{
   if (dropping) {
      dropping->moveLeft();
      redraw();
   }
}

void
Board::moveRight()
{
   if (dropping) {
      dropping->moveRight();
      redraw();
   }
}

void
Board::drop()
{
   if (dropping) {

      while (dropping->moveDown()) ;

      nextMove();
   }
}

void
Board::nextMove()
{
   if (dropping) {
      dropping->decay();
      dropping = NULL;
      clearLines();

      redraw();

      newTile(next);
      next = randomTile();
      redrawPreview();
   }
}

void
Board::removeLine(int y)
{
   int i, j;
   Block** line;

   line = blocks[y];
   for (j = 0; j < width; j++)
      line[j] = NULL;

   for (i = y-1; i >= 0; i--)
      blocks[i+1] = blocks[i];

   blocks[0] = line;
}

void
Board::clearLines()
{
   int x, y, lines = 0;
   bool full;

   for (y = height-1; y >= 0; ) {
      full = true;
      for (x = 0; x < width; x++)
         if (blocks[y][x] == NULL) {
            full = false;
            break;
         }

      if (full) {
         lines++;
         removeLine(y);
      }
      else y--;
   }

   if (lines)
      app->addLines(lines);
   
}

void
Board::redraw()
{
   gtk_widget_queue_draw (w_board);
}

void
Board::redrawPreview()
{
   gtk_widget_queue_draw (w_next);
}

void
Board::setWidget(GtkWidget *w)
{
   w_board = w;
}

void
Board::setPreviewWidget(GtkWidget *w)
{
   w_next = w;
}

void
Board::resize(int w, int h)
{
   int i, j;
   int block_size = app->getBlockSize();

   if (blocks) {
      for (i = 0; i < height; i++)
         delete[] blocks[i];
      delete[] blocks;
   }

   width = w;
   height = h;

   blocks = new Block**[height];
   for (i = 0; i < height; i++) {
      blocks[i] = new Block*[width];
      for (j = 0; j < width; j++)
         blocks[i][j] = NULL;
   }

   /* add 10 pixels for border */
   gtk_widget_set_size_request(w_board,
         width * block_size + 10,
         height * block_size + 10);


   redraw();
}

void
Board::updateBlockSize()
{
   int size = app->getBlockSize();

   gtk_widget_set_size_request (w_next, 10 + 4*size, 10 + 4*size);
   gtk_widget_set_size_request (w_board, width*size + 10, height*size + 10);
}

int
Board::getWidth()
{
   return width;
}

int
Board::getHeight()
{
   return height;
}

int
random(int m)
{
   double num = (double)rand() / RAND_MAX * m;
   return (int)num;
}

TileType
Board::randomTile()
{
   int t = random(7);
   return (TileType)t;
}

bool
Board::newTile(TileType t)
{
   int px, py;
   Tile *tile;
  
   tile = new Tile(t);
   px = (width - tile->getWidth()) / 2;
   py = 0;

   tile->setBoard(this);

   if (tile->setPosition(px, py)) {
      dropping = tile;
      return true;
   }
   else {
      dropping = NULL;
      app->gameOver();
      return false;
   }
}

void
Board::addBlock(Block *b)
{
   int x, y;

   // blocks.push_back(b);

   x = b->getX();
   y = b->getY();
   if (x >= 0 && x < width && y >= 0 && y < height)
      blocks[y][x] = b;
}

bool
Board::isPositionFree(int x, int y)
{
   /* refuse positions outside the board */
   if (x < 0 || x >= width || y < 0 || y >= height)
      return false;

   return (blocks[y][x] == NULL);
}

void
Board::setNextTile(TileType t)
{
   next = t;
}

TileType
Board::getNextTile()
{
   return next;
}

void
Board::reset()
{
   int x, y;

   for (y = 0; y < height; y++)
      for (x = 0; x < width; x++)
         blocks[y][x] = NULL;

   dropping = NULL;
   next = randomTile();
}
