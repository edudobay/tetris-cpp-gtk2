#include <cmath>
#include <cstdio>
#include <glib.h>

#include "tetris.hpp"
#include "board.hpp"
#include "gui.hpp"

Tetris *app;

extern "C" bool
drop_tile (Board *board)
{
   board->moveDown();

   return true;
}


Tetris::Tetris()
{
   block_size = 32;

   timeout = 0;
   timeout_id = 0;

   lines = 0;
   level = 0;
   score = 0;

   started = false;
   paused = false;
   game_over = false;
}

int
Tetris::getBlockSize()
{
   return block_size;
}

void
Tetris::setBlockSize(int s)
{
   if (s <= 0) {
      fprintf(stderr, "assertion `block_size < 0' failed at setBlockSize\n");
      return;
   }

   block_size = s;
}

void
Tetris::setSensibleBlockSize()
{
   int w, h, bw, bh, size;

   window->getSize(&w, &h);
   bw = board->getWidth();
   bh = board->getHeight();

   size = 32;

   while ((bw + 4) * size + 50 >= w || bh * size + 120 >= h)
      size--;

   if (size > 0)
      block_size = size;

   board->updateBlockSize();
}

int
Tetris::getTimeout()
{
   return timeout;
}

void
Tetris::clearTimeout()
{
   if (timeout_id > 0)
      g_source_remove (timeout_id);

   timeout_id = 0;
   timeout = 0;
}

void
Tetris::setTimeout(int ms)
{
   clearTimeout();

   timeout = ms;
   timeout_id = g_timeout_add (timeout, (GSourceFunc) drop_tile, board);
}

void
Tetris::setBoard(Board *b)
{
   board = b;
}

void
Tetris::addLines(int n)
{
   lines += n;

   switch (n) {
      case 1: score += 40 * level;
              break;

      case 2: score += 100 * level;
              break;

      case 3: score += 300 * level;
              break;

      case 4: score += 1200 * level;
              break;

      default:
         fprintf(stderr, "unexpected number: %d for Tetris::addLines\n", n);
         return;
   }

   if (n > 0)
      setLevel(lines / 10 + 1);

   window->updateScore();
}

int
Tetris::getLines()
{
   return lines;
}

int
Tetris::getLevel()
{
   return level;
}

int
Tetris::timeoutForLevel(int lv)
{
   int tm;

   tm = (int) (1000 * pow(0.8, lv - 1));
   if (tm < 10)
      tm = 10;

   return tm;
}

void
Tetris::setLevel(int lv)
{
   if (lv == level)
      return;

   level = lv;

   if (running())
      setTimeout (timeoutForLevel(lv));
}

int
Tetris::getScore()
{
   return score;
}

void
Tetris::setWindow(MainWindow *w)
{
   window = w;
}

void
Tetris::gameOver()
{
   clearTimeout();
   game_over = true;
   board->redraw();
}

void
Tetris::reset()
{
   clearTimeout();
   board->reset();

   lines = 0;
   level = 0;
   score = 0;
   window->updateScore();

   started = false;
   paused = false;
   game_over = false;
}

void
Tetris::stop()
{
   reset();
   board->redraw();
   board->redrawPreview();
}

void
Tetris::start()
{
   reset();

   board->newTile(Board::randomTile());
   board->setNextTile(Board::randomTile());

   level = 1;
   started = true;

   board->redraw();
   board->redrawPreview();
   window->updateScore();
   setTimeout (timeoutForLevel(level));
}

void
Tetris::togglePaused()
{
   if (paused)
      unpause();
   else
      pause();
}

void
Tetris::pause()
{
   clearTimeout();
   paused = true;
   board->redraw();
}

void
Tetris::unpause()
{
   setTimeout (timeoutForLevel(level));
   paused = false;
   board->redraw();
}


bool
Tetris::running()
{
   return (started && !paused);
}

bool
Tetris::isPaused()
{
   return paused;
}

bool
Tetris::hasStarted()
{
   return started;
}

bool
Tetris::isGameOver()
{
   return game_over;
}
