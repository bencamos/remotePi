#include <ESP8266mDNS.h>
#include <ESP8266mDNS_Legacy.h>
#include <LEAmDNS.h>
#include <LEAmDNS_lwIPdefs.h>
#include <LEAmDNS_Priv.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <cstddef>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <ESP8266Ping.h>
#include "FS.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#define atoa(x) #x
#define LED1 D4
#define LED2 D0
//digitalWrite(LED1, HIGH);
ESP8266WebServer server(80);
WiFiClient client;
File fsUploadFile;
String texttype = "text/html";
int fileSizeCap = 3000;

void loop() {
        server.handleClient();
}
