#ifndef FILE_H
#define FILE_H

#include "LittleFS.h"

// Funktionen oder Deklarationen hier angeben
void initFS();
void initSD() ;
String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void loadWifiParameter();
uint8_t SDtoLittleFS(const char * SDPath,const char * LittleFSPath );

#endif