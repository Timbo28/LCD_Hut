#include <WiFi.h>  
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPI.h>
#include "LittleFS.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "file.h"
#include "webServer.h"
#include "config.h"
#include "display.h"
#include "GIF.h"

AsyncWebServer server(80);

 // Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";


//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;

// File paths to save input values permanently
const char* ssidPath = "/TextFiles/ssid.txt";
const char* passPath = "/TextFiles/pass.txt";
const char* ipPath = "/TextFiles/ip.txt";
const char* gatewayPath = "/TextFiles/gateway.txt";

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);

String lcd_state;
static bool gif_status = false;

// Initialize WiFi
bool initWiFi() {
  pinMode(APStartPin, INPUT_PULLUP); // Setze den Pin als Eingang mit internem Pull-up-Widerstand

  if(ssid=="" ){
    gfx->fillScreen(BLACK);
    centerTextToScreen(120,70,"!Es wurde kein WLAN ausgewählt!",RED,FONT);
    Serial.println("Undefinierte SSID.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(300);
  //localIP.fromString(ip.c_str());
  //localGateway.fromString(gateway.c_str());


  //if (!WiFi.config(localIP, localGateway, subnet)){
  //  Serial.println("STA Failed to configure");
  //  return false;
 // }
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("connecting");
    
  uint16_t textWidth, textHeight;
  int16_t px, py;
  gfx->getTextBounds( ("Verbindet mit "+ ssid +" ..."),0,0, &px, &py, &textWidth, &textHeight);
  
  if(textWidth > 170){
    centerTextToScreen(120,100,( "Verbindet mit "),WHITE,FONT);
    centerTextToScreen(120,120,(ssid +" ..."),WHITE,FONT);
  }else{
    centerTextToScreen(120,100,( "Verbindet mit "+ ssid +" ..."),WHITE,FONT);
  }


  while(WiFi.status() != WL_CONNECTED) {

    if (digitalRead(APStartPin) == LOW) {
      Serial.println("Failed to connect.");
      return false;
    }
  }

  delay(500);
  return true;
}


// Replaces placeholder with LED state value
String processor(const String& var) {
  if(var == "STATE") {
    if(gif_status) {
      lcd_state = "ON";
    }
    else {
      lcd_state = "OFF";
    }
    return lcd_state;
  }
  return String();
}


void setupWebpage(){
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SD, "/index.html", "text/html", false, processor);
    });

    server.serveStatic("/",SD, "/");

        // Route to set GPIO state to HIGH
    server.on("/gif1", HTTP_GET, [](AsyncWebServerRequest *request) {
      gif_status = true;
      SDtoLittleFS("/gif/Dreieckoid.gif","/gif/Dreieckoid.gif");
      request->send(SD, "/GIF.html", "text/html", false, processor);
    });
    
    server.on("/gif2", HTTP_GET, [](AsyncWebServerRequest *request) {
      gif_status = true;
      SDtoLittleFS("/gif/Dreieckoid.gif","/gif/Pumping_Wave.gif");
      request->send(SD, "/GIF.html", "text/html", false, processor);
    });

    // Route to set GPIO state to LOW
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
      gif_status = false;
      request->send(SD, "/index.html", "text/html", false, processor);
    });
    
    server.begin();
    print_IP();

}


void setupAP(){
    // Connect to Wi-Fi network with SSID and password
    gfx->fillScreen(BLACK);
    centerTextToScreen(120,80,"AP wird gestarted...",WHITE,u8g2_font_courB12_tf);
    // NULL sets an open Access Point
    WiFi.softAP(AP_NAME, NULL);
    IPAddress IP = WiFi.softAPIP();
    centerTextToScreen(120,120,"Gehe im Browser auf:",WHITE,FONT);
    centerTextToScreen(120,140,("http://"+IP.toString()+"/"),WHITE,FONT);
  

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SD, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", SD, "/");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writeFile(LittleFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writeFile(LittleFS, passPath, pass.c_str());
          }
          // HTTP POST ip value
          if (p->name() == PARAM_INPUT_3) {
            ip = p->value().c_str();
            Serial.print("IP Address set to: ");
            Serial.println(ip);
            // Write file to save value
            writeFile(LittleFS, ipPath, ip.c_str());
          }
          // HTTP POST gateway value
          if (p->name() == PARAM_INPUT_4) {
            gateway = p->value().c_str();
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
            // Write file to save value
            writeFile(LittleFS, gatewayPath, gateway.c_str());
          }
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + ip);
      delay(3000);
      ESP.restart();
    });
    server.begin(); 
}

bool get_gif_status(){

  return gif_status;
}

void print_IP(){
    gfx->fillScreen(BLACK);
    centerTextToScreen(120,100,"Gehe im Browser auf:",WHITE,FONT);
    localIP = WiFi.localIP();
    centerTextToScreen(120,130,("http://"+localIP.toString() +"/"),WHITE,FONT);
    Serial.println(localIP);
}