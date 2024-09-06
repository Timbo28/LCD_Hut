#ifndef DISPLAY_H
#define DISPLAY_H


#include <Arduino_GFX_Library.h>
// Declare the gfx variable as extern
extern Arduino_GFX *gfx;

// Funktionen oder Deklarationen hier angeben
void initDisplay();
void centerTextToScreen(int x, int y,const String Text, uint16_t Color, const uint8_t* font);
void textToScreen(int x, int y,const String Text,uint16_t Color, const uint8_t* font);

#endif