#include <cairo.h>
#include "block.hpp"
#include "board.hpp"
#include "tetris.hpp"

double colors_rgb[][2][3] = {
  {{ 0.2901960784, 0.3490196078, 0.6784313725 }, /* blue */
   { 0.4196078431, 0.4862745098, 0.8431372549 }},
  {{ 0.2901960784, 0.6039215686, 0.6784313725 }, /* cyan */
   { 0.4196078431, 0.7607843137, 0.8431372549 }},
  {{ 0.4078431373, 0.6784313725, 0.2901960784 }, /* green */
   { 0.5490196078, 0.8431372549, 0.4196078431 }},
  {{ 0.7490196078, 0.5450980392, 0.2235294118 }, /* orange */
   { 0.9137254902, 0.6980392157, 0.3529411765 }},
  {{ 0.6784313725, 0.2901960784, 0.6509803922 }, /* purple */
   { 0.8431372549, 0.4196078431, 0.8078431373 }},
  {{ 0.6784313725, 0.2901960784, 0.2901960784 }, /* red */
   { 0.8431372549, 0.4196078431, 0.4196078431 }},
  {{ 0.6666666667, 0.6784313725, 0.2901960784 }, /* yellow */
   { 0.8274509804, 0.8431372549, 0.4196078431 }},
};

Block::Block(BlockColor c)
   : color(c)
{
}

Block::Block(BlockColor c, int _x, int _y)
   : color(c)
{
   setPosition(_x, _y);
}

void
Block::setPosition(int _x, int _y)
{
   x = _x;
   y = _y;
}

int
Block::getX()
{
   return x;
}

int
Block::getY()
{
   return y;
}

bool
Block::isAt(int _x, int _y)
{
   return (x == _x && y == _y);
}

void
Block::draw(Board *board, int x, int y)
{
   draw(board->getContext(), x, y, color);
}

void
Block::draw(cairo_t *cr, int x, int y, BlockColor color)
{
   int size;

   size = app->getBlockSize();

   cairo_save(cr);
   cairo_translate(cr, x*size, y*size);

   cairo_set_source_rgb(cr, colors_rgb[color][0][0], colors_rgb[color][0][1], colors_rgb[color][0][2]);
   rounded_rectangle(cr, 0, 0, size, size, size*0.2);
   cairo_fill_preserve(cr);
   cairo_set_source_rgb(cr, 0.14117647, 0.14117647, 0.14117647);
   cairo_set_line_width(cr, 2.0);
   cairo_stroke(cr);

   cairo_set_source_rgb(cr, colors_rgb[color][1][0], colors_rgb[color][1][1], colors_rgb[color][1][2]);
   rounded_rectangle(cr, size*0.15, size*0.15, size*0.7, size*0.7, size*0.2);
   cairo_fill(cr);
   cairo_restore(cr);
}

void
Block::rounded_rectangle(cairo_t *cr, double x, double y, double w,
      double h, double r)
{
   cairo_move_to(cr, x+r, y);
   cairo_line_to(cr, x+w-r, y);
   cairo_curve_to(cr, x+w - r/2, y, x+w, y + r/2, x+w, y+r);
   cairo_line_to(cr, x+w, y+h-r);
   cairo_curve_to(cr, x+w, y+h - r/2, x+w - r/2, y+h, x+w-r, y+h);
   cairo_line_to(cr, x+r, y+h);
   cairo_curve_to(cr, x + r/2, y+h, x, y+h - r/2, x, y+h-r);
   cairo_line_to(cr, x, y+r);
   cairo_curve_to(cr, x, y + r/2, x + r/2, y, x+r, y);
}

