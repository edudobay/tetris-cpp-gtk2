#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <cairo.h>
#include "tetris.hpp"
#include "gui.hpp"
#include "tile.hpp"

static gboolean board_expose (GtkWidget *widget, GdkEventExpose *event);
static gboolean next_expose (GtkWidget *widget, GdkEventExpose *event);
static gboolean on_key_press (GtkWidget *widget, GdkEventKey *event);

GtkWidget *create_bold_label (const char *text);
GtkWidget *create_label (const char *text);

/*******************************************************************************
 * MainWindow class implementation
 ******************************************************************************/

MainWindow::MainWindow()
{
   GtkWidget *label;
   GtkWidget *hbox, *vbox, *table;

   int size = app->getBlockSize();

   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW(window), "Tetris");
   gtk_widget_add_events (window, GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
   // gtk_widget_set_app_paintable (window, TRUE);
   gtk_container_set_border_width (GTK_CONTAINER(window), 10);

   w_board = gtk_drawing_area_new ();
   gtk_widget_show (w_board);

   w_next = gtk_drawing_area_new ();
   gtk_widget_set_size_request (w_next, 10 + 4*size, 10 + 4*size);
   gtk_widget_show (w_next);

   table = gtk_table_new (3, 2, FALSE);
   gtk_table_set_row_spacings (GTK_TABLE(table), 3);
   gtk_table_set_col_spacings (GTK_TABLE(table), 6);
   gtk_container_set_border_width (GTK_CONTAINER(table), 10);
   gtk_widget_show (table);
   
   label = create_bold_label ("Level:");
   lbl_level = create_label ("0");
   gtk_table_attach (GTK_TABLE(table), label,     0, 1, 0, 1, GTK_FILL,
         (GtkAttachOptions)0, 0, 0);
   gtk_table_attach (GTK_TABLE(table), lbl_level, 1, 2, 0, 1,
         (GtkAttachOptions)(GTK_FILL | GTK_EXPAND),
         (GtkAttachOptions)0, 0, 0);

   label = create_bold_label ("Lines:");
   lbl_lines = create_label ("0");
   gtk_table_attach (GTK_TABLE(table), label,     0, 1, 1, 2, GTK_FILL,
         (GtkAttachOptions)0, 0, 0);
   gtk_table_attach (GTK_TABLE(table), lbl_lines, 1, 2, 1, 2,
         (GtkAttachOptions)(GTK_FILL | GTK_EXPAND),
         (GtkAttachOptions)0, 0, 0);

   label = create_bold_label ("Score:");
   lbl_score = create_label ("0");
   gtk_widget_show (lbl_score);
   gtk_table_attach (GTK_TABLE(table), label,     0, 1, 2, 3, GTK_FILL,
         (GtkAttachOptions)0, 0, 0);
   gtk_table_attach (GTK_TABLE(table), lbl_score, 1, 2, 2, 3,
         (GtkAttachOptions)(GTK_FILL | GTK_EXPAND),
         (GtkAttachOptions)0, 0, 0);
   
   vbox = gtk_vbox_new (FALSE, 10);
   gtk_box_pack_start (GTK_BOX(vbox), w_next, FALSE, FALSE, 0);
   gtk_box_pack_start (GTK_BOX(vbox), table, FALSE, TRUE, 0);
   gtk_widget_show (vbox);

   hbox = gtk_hbox_new (FALSE, 10);
   gtk_box_pack_start (GTK_BOX(hbox), w_board, FALSE, FALSE, 0);
   gtk_box_pack_start (GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
   gtk_widget_show (hbox);
   gtk_container_add (GTK_CONTAINER(window), hbox);

   g_signal_connect (G_OBJECT(w_board), "expose-event",
         G_CALLBACK(board_expose), NULL);

   g_signal_connect (G_OBJECT(w_next), "expose-event",
         G_CALLBACK(next_expose), NULL);

   g_signal_connect (G_OBJECT(window), "key-press-event",
         G_CALLBACK(on_key_press), NULL);

   g_signal_connect (G_OBJECT(window), "delete-event",
         gtk_main_quit, NULL);

}

void
MainWindow::show()
{
   gtk_widget_show (window);
}

void
MainWindow::getSize(int *width, int *height)
{
   GdkScreen *scr;

   scr = gtk_window_get_screen (GTK_WINDOW(window));
   *width = gdk_screen_get_width (scr);
   *height = gdk_screen_get_height (scr);
}

void
MainWindow::updateScore()
{
   char buf[20];

   sprintf(buf, "%d", app->getLevel());
   gtk_label_set_text(GTK_LABEL(lbl_level), buf);

   sprintf(buf, "%d", app->getLines());
   gtk_label_set_text(GTK_LABEL(lbl_lines), buf);

   sprintf(buf, "%d", app->getScore());
   gtk_label_set_text(GTK_LABEL(lbl_score), buf);
}


/*******************************************************************************
 * Other GUI functions
 ******************************************************************************/

GtkWidget *
create_bold_label (const char *text)
{
   GtkWidget *label;
   PangoAttrList *attrs;
   PangoAttribute *attr;

   label = gtk_label_new (text);
   gtk_misc_set_alignment (GTK_MISC(label), 0, 0.5);

   attrs = gtk_label_get_attributes (GTK_LABEL(label));
   if (attrs == NULL)
      attrs = pango_attr_list_new ();

   attr = pango_attr_weight_new (PANGO_WEIGHT_BOLD);
   pango_attr_list_insert (attrs, attr);
   gtk_label_set_attributes (GTK_LABEL(label), attrs);
   pango_attr_list_unref (attrs);

   gtk_widget_show (label);
   return label;
}

GtkWidget *
create_label (const char *text)
{
   GtkWidget *label;

   label = gtk_label_new (text);
   gtk_misc_set_alignment (GTK_MISC(label), 1, 0.5);

   gtk_widget_show (label);
   return label;
}

void
draw_text_center (cairo_t *cr, const char *msg, double width, double height)
{
   cairo_text_extents_t extents;
   double x, y;

   cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size (cr, 64.0);
   cairo_text_extents (cr, msg, &extents);

   x = width/2 - (extents.width/2 + extents.x_bearing);
   y = height/2 - (extents.height/2 + extents.y_bearing);

   cairo_move_to (cr, x, y);
   cairo_text_path (cr, msg);

   cairo_set_source_rgb (cr, 1, 1, 1);
   cairo_fill_preserve (cr);

   cairo_set_source_rgb (cr, 0, 0, 0);
   cairo_set_line_width (cr, 2.0);
   cairo_stroke (cr);
}

static gboolean
board_expose (GtkWidget *widget, GdkEventExpose *event)
{
   cairo_t *cr;
   int width, height;
   Board *board = app->board;

   cr = gdk_cairo_create (widget->window);
   width = widget->allocation.width;
   height = widget->allocation.height;

   /* 1b4c8d -> 000005 */
   cairo_set_source_rgb(cr, 0.1058823529, 0.2980392157, 0.5529411765);
   cairo_paint(cr);
   cairo_rectangle(cr, 2.5, 2.5, width - 5.0, height - 5.0);
   cairo_set_source_rgb(cr, 0, 0, 0.129);
   cairo_set_line_width(cr, 5.0);
   cairo_stroke(cr);

   cairo_translate(cr, 5.0, 5.0);

   board->setContext(cr);

   if (app->hasStarted()) {
      board->draw();

      if (app->isPaused()) {
         draw_text_center (cr, "Paused", width, height);
      } else if (app->isGameOver()) {
         draw_text_center (cr, "Game over", width, height);
      }
   }

   cairo_destroy (cr);

   return FALSE;
}

static gboolean
next_expose (GtkWidget *widget, GdkEventExpose *event)
{
   cairo_t *cr;
   TileType t;
   Board *board = app->board;

   cr = gdk_cairo_create (widget->window);

   cairo_set_source_rgb (cr, 0, 0, 0);
   cairo_paint (cr);
   cairo_translate (cr, 5.0, 5.0);
   
   if (app->hasStarted()) {
      t = board->getNextTile();
      Tile::draw (cr, 0, 0, t);
      cairo_destroy (cr);
   }

   return FALSE;
}

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event)
{
   Board *board = app->board;
   bool running = app->running();

   switch (event->keyval) {
   case GDK_Up:
      if (running)
         board->rotate();
      break;

   case GDK_Down:
      if (running)
         board->moveDown();
      break;

   case GDK_Left:
      if (running)
         board->moveLeft();
      break;

   case GDK_Right:
      if (running)
         board->moveRight();
      break;

   case GDK_space:
      if (running)
         board->drop();
      break;

   case GDK_c:
      app->stop();
      break;

   case GDK_n:
      app->start();
      break;

   case GDK_p:
      if (app->hasStarted())
         app->togglePaused();
      break;

   case GDK_q:
      gtk_main_quit();
      break;

   case GDK_h:
      printf("Hello, world!\n");
      break;

   default:
      break;
   }

   return FALSE;
}


