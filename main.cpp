//#include <cmath>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

#include "gui.hpp"
#include "tetris.hpp"
#include "board.hpp"

int main (int argc, char *argv[])
{
   MainWindow *w;
   Board *board;

   gtk_init (&argc, &argv);

   app = new Tetris();
   w = new MainWindow();
   w->show();
   app->setWindow(w);

   board = new Board();
   board->setWidget(w->w_board);
   board->setPreviewWidget(w->w_next);
   board->resize(14, 20);

   srand(getpid() * 20 * time(NULL));

   app->setBoard(board);
   app->setSensibleBlockSize();
   w->updateScore();

   gtk_main ();

   return 0;
}

