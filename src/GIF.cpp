#include "display.h"
#include "GIF.h"
#include "webServer.h"
#include "SD.h"
#include "FS.h"

#include "SPI.h"
#include "LittleFS.h"
#include "GifClass.h"

String GIFPath = "/gif/Dreieckoid.gif";

static GifClass gifClass;


void runGIF(){

  
  File gifFile = LittleFS.open(GIFPath, "r");

  if (!gifFile || gifFile.isDirectory()){
    Serial.println(F("ERROR: open gifFile Failed!"));
    gfx->println(F("ERROR: open gifFile Failed!"));
  }
  else{
    // read GIF file header
    gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
    if (!gif){
      Serial.println(F("gd_open_gif() failed!"));
    }else{
      uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
      if (!buf){
        Serial.println(F("buf malloc failed!"));
      }else{
        int16_t x = (gfx->width() - gif->width) / 2;
        int16_t y = (gfx->height() - gif->height) / 2;

        Serial.println(F("GIF video start"));
        int32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
        int32_t res = 1;
        int32_t duration = 0, remain = 0;
        while (res > 0 ){
          t_fstart = millis();
          t_delay = gif->gce.delay * 10;
          res = gifClass.gd_get_frame(gif, buf);
          if (res < 0){
            Serial.println(F("ERROR: gd_get_frame() failed!"));
            break;
          }else if (res > 0){
            gfx->drawIndexedBitmap(x, y, buf, gif->palette->colors, gif->width, gif->height);
          }
        }

        gifClass.gd_close_gif(gif);
        free(buf);
      }
    }
  }

}