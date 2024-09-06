
#include "display.h"
#include "./Bilder/panda.h"
#include <U8g2lib.h>


#define GFX_BL 22 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

Arduino_GFX *gfx = new Arduino_GC9A01(bus,DF_GFX_RST, 0 /* rotation */, true /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/


void initDisplay(){
  
  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

gfx->draw16bitRGBBitmap(0,0,Panda_icon,PandaWidth,PandaHeight);
delay(2000);
gfx->fillScreen(BLACK);

}

void textToScreen(int x, int y,const String Text, uint16_t Color, const uint8_t* font){

  // Wähle die Schriftart und -größe
  gfx->setFont(font);
  gfx->setTextSize(1);

  // Wähle die Textfarbe (weiß)
  gfx->setTextColor(Color);

  // Setze die Cursorposition auf (20, 30)
  gfx->setCursor(x, y);

  // Gib den Text "Hallo Welt!" aus
  gfx->print(Text);

}

void centerTextToScreen(int x, int y,const String Text, uint16_t Color ,const uint8_t* font){

  // Wähle die Schriftart und -größe
  /* U8g2 font list: https://github.com/olikraus/u8g2/wiki/fntlistall */
  gfx->setFont(font);
  gfx->setTextSize(1);

  // Wähle die Textfarbe (weiß)
  gfx->setTextColor(Color);

  // Setze die Cursorposition auf (20, 30)
  uint16_t textWidth, textHeight;
  int16_t px, py;
  gfx->getTextBounds(Text ,0,0, &px, &py, &textWidth, &textHeight);
  gfx->setCursor(x-(textWidth/2), y+(textHeight/2));

  // Gib den Text "Hallo Welt!" aus
  gfx->print(Text);

}
