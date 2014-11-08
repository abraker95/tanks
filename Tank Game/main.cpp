#include <iostream>
#include <allegro5/allegro.h>

int main()
{
 ALLEGRO_DISPLAY *display = NULL;
 if(!al_init()) return -1;

 display = al_create_display(1024, 720);
 if(!display) return -1;

 al_clear_to_color(al_map_rgb(0, 0, 0));
 
 // render

 al_flip_display();
 al_rest(10.0);
 al_destroy_display(display);

 return 0;
}