#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "DisplayBoard";
const char *password = "Welcome12#";

IPAddress local_IP(192,168,4,100);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer webserver(80);

static const char PROGMEM INDEX_HTML[] = R"rawliteral(  
<!DOCTYPE html>
<html>
<head>
</head>
<style>
<body {background-color: #ff832bff;}
</style>
<h1><b>NOTICE BOARD </b></h1> <br><br><br>
<form method = "post" action= "/save"> ENTER THE NOTICE HERE: <br>
<input type=text name=NOTICE IS><br>
<input type=submit>
<input type=reset></form>
</html>
)rawliteral";

//------------------------------------------------
void handleRoot() {
  webserver.send(200, "text/html", INDEX_HTML);
}

//------------------------------------------------
void handleNotFound() {
  webserver.send(404, "text/plain", "Page not found ...");
}

//------------------------------------------------
void handleSave() {
  String str = "Settings Saved ...\r\n";

  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      str += webserver.argName(i) + " = " + webserver.arg(i) + "\r\n";
    }
    Serial.println("~"+webserver.arg(0)+"^");
  }
  webserver.send(200, "text/plain", str.c_str());
}

//------------------------------------------------
void setup() {
  delay(1000);
  Serial.begin(115200);
  //Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAPConfig(local_IP, gateway, subnet);
  delay(2000);
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  //Serial.print("HotSpt IP:");
  //Serial.println(myIP);
  delay(1000);

  webserver.on("/", handleRoot);
  webserver.on("/save", handleSave);
  webserver.onNotFound(handleNotFound);

  webserver.begin();
  //Serial.println("Web server has started");
}

//------------------------------------------------
void loop() {
  webserver.handleClient();
}
