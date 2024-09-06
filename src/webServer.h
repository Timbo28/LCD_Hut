 #ifndef WEBSERVER_H
#define WEBSERVER_H


//Variables to save values from HTML form
extern String ssid;
extern String pass;
extern String ip;
extern String gateway;

// File paths to save input values permanently
extern const char* ssidPath;
extern const char* passPath;
extern const char* ipPath;
extern const char* gatewayPath ;

bool initWiFi();
void setupWebpage();
void setupAP();
bool get_gif_status();
void print_IP();

#endif
 

