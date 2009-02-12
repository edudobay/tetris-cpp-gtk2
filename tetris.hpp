#ifndef _TETRIS_HPP
#define _TETRIS_HPP

class Board;
class MainWindow;

class Tetris {

public:
   Tetris();

   int  getBlockSize();
   void setBlockSize(int s);
   void setSensibleBlockSize();

   int  getTimeout();
   void setTimeout(int ms);
   void clearTimeout();
   static int timeoutForLevel(int lv);

   void addLines(int n);
   int  getLines();

   int  getScore();
   void gameOver();

   int  getLevel();
   void setLevel(int lv);

   void setBoard(Board *b);
   void setWindow(MainWindow *w);

   void reset();
   void start();
   void stop();
   void togglePaused();
   void pause();
   void unpause();

   bool running();
   bool isPaused();
   bool hasStarted();
   bool isGameOver();


   Board *board;

private:

   int block_size;
   int timeout; /* piece falling timeout, in milliseconds */
   int level, lines, score;
   unsigned timeout_id;
   MainWindow *window;

   bool paused, started, game_over;

};

extern Tetris *app;

#endif /* _TETRIS_HPP */
