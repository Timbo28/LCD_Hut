#include "LittleFS.h"
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "file.h"
#include "webServer.h"
#include "display.h"


void initFS() {
  if (!LittleFS.begin()) {
    gfx->fillScreen(BLACK);
    gfx->setCursor(5,110);
    gfx->println("An error has occurred with LittleFS");
    gfx->setCursor(5,120);
    gfx->println("Restarting ! ");
    delay(3000);
    ESP.restart();
  }
  Serial.println("LittleFS mounted successfully");
}

void initSD() {
  if (!SD.begin(17)) {
    gfx->fillScreen(BLACK);
    gfx->setCursor(5,110);
    gfx->println("An error has occurred with SD card");
    gfx->setCursor(5,120);
    gfx->println("Restarting ! ");
    delay(3000);
    ESP.restart();
  }
  Serial.println("SD card mounted successfully");
}


// Read File from LittleFS
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

void loadWifiParameter(){
  ssid = readFile(LittleFS, ssidPath);
  pass = readFile(LittleFS, passPath);
  ip = readFile(LittleFS, ipPath);
  gateway = readFile (LittleFS, gatewayPath);
  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(ip);
  Serial.println(gateway);

}

uint8_t SDtoLittleFS(const char * SDPath,const char * LittleFSPath ){
  File sdFile;
  File littlefsFile;
  uint8_t buffer[5120];
  size_t bytesRead;

  sdFile = SD.open(SDPath, FILE_READ);
  if (!sdFile) {
    Serial.println("Fehler beim Öffnen der GIF-Datei auf der SD-Karte.");
    return 0;
  }
  
  littlefsFile = LittleFS.open(LittleFSPath, FILE_WRITE);
  if (!littlefsFile) {
    Serial.println("Fehler beim Öffnen der GIF-Datei auf LittleFS.");
    return 0;
  }


  while ((bytesRead = sdFile.read(buffer, 5120)) > 0) {
    littlefsFile.write(buffer, bytesRead);
  }

  sdFile.close();
  littlefsFile.close();

  return 1;
}