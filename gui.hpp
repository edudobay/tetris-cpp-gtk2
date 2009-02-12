#ifndef _GUI_HPP
#define _GUI_HPP

#include <gtk/gtk.h>

class MainWindow {

public:
   MainWindow();

   void show();
   void updateScore();

   void getSize(int *width, int *height);


   GtkWidget *w_board, *w_next;

private:
   GtkWidget *window;
   GtkWidget *lbl_score, *lbl_lines, *lbl_level;
};


/* ************************************************************************** */

class ConfigDialog {

public:
   ConfigDialog();

   int getBlockSize();

private:
   GtkWidget *dialog;

};

#endif /* _GUI_HPP */
