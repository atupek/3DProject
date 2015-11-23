#include "bitmap/bitmap_image.hpp"

int main()
{
   bitmap_image image(200,200);

   // set background to orange
   image.set_all_channels(255,255,255);

   image_drawer draw(image);

   //draw.pen_width(3);
   //draw.pen_color(255,0,0);
   //draw.circle(image.width() / 2, image.height() / 2,50);

   draw.pen_width(1);
   draw.pen_color(0,0,0);
   //draw.rectangle(50,50,150,150);
   draw.plot_pixel(50, 50);

   for(int i = 0; i < 100; i++)
   {
      draw.pen_width(1);
      draw.pen_color(0, 0, 0);
      draw.plot_pixel(i, i);
   }

   image.save_image("output.bmp");

   return 0;
}
    