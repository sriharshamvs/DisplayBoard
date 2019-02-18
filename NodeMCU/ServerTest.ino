#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Startrek";
const char* password = "Face fade12#";

IPAddress local_IP(192,168,0,222);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer webserver(80);

//------------------------------------------------
/*static const char PROGMEM singleLine[] = R"rawliteral(  
<!DOCTYPE html>
<html>
<head>
<title>Single Line Input</title>
</head>
<body>
<form method="post" action="/save" >
<label for='name'>Serial Printer (Put Text here: ): </label>
<br><br>
<input type='text' id='name' name='name' required minlength='0' maxlength='800' size='100'>
<input type="submit" name="clk_action" value="accept">
</form>
</body>
</html>
)rawliteral";
*/
//------------------------------------------------
/*static const char PROGMEM INDEX_HTML[] = R"rawliteral(  
<!DOCTYPE html>
<html>
<head>
<title>Clock Settings</title>
</head>
<body>
<form method="post" action="/save" >
Turn on time:<br><input name="onTime" type="text" size="16" value="" ><br><br>
Turn off time:<br><input name="offTime" type="text" size="16" value="" ><br><br> 
<input type="submit" name="clk_action" value="accept">
</form>
</body>
</html>
)rawliteral";
*/

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

  //Serial.print("number of arguments "); 
  //Serial.println(webserver.args());                    // number of arguments

  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      str += webserver.argName(i) + " = " + webserver.arg(i) + "\r\n";
      //Serial.println("Arg "+ String(i)+"="+ webserver.argName(i));     
      //Serial.println("Arg "+ String(i)+"="+ webserver.arg(i));
    }
    //Serial.println("Arg "+ String(0)+"="+ webserver.arg(0));
    Serial.println("~"+webserver.arg(0)+"^");
  }
  webserver.send(200, "text/plain", str.c_str());
}

//------------------------------------------------
/*void handleSingleLine() {
  webserver.send(200, "text/html", singleLine);
}*/

//------------------------------------------------
void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  webserver.on("/", handleRoot);
  webserver.on("/save", handleSave);
  webserver.onNotFound(handleNotFound);
//  webserver.on("/single", handleSingleLine);

  webserver.begin();
  Serial.println("Web server has started");
}

//------------------------------------------------
void loop() {
  webserver.handleClient();

}
