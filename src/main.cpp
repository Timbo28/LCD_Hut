#include "WiFi.h"

#include "display.h"
#include "file.h"
#include "webServer.h"
#include "GIF.h"
#include "config.h"
void setup() {
  Serial.begin(115200);

  initDisplay();

  initFS();
  initSD();

  // Load values saved in LittleFS
  loadWifiParameter();


 if(initWiFi()) {
   setupWebpage();
  }else {
    setupAP();
  }


}

void loop() {

 // if(get_gif_status()){
    runGIF();
    
  //  if(!get_gif_status()){
   //  print_IP();
   // }
 // }

}





