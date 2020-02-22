#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WiFiServer server(80);

String header;

const int FanPin = 4;
String AutoMode = "off";
String SleepMode = "off";
String LowMode = "off";
String MediumMode = "off";
String HighMode = "off";

int FanSpeed = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(FanPin, OUTPUT);

  WiFiManager wifiManager;
  wifiManager.autoConnect("BlowMe");
  Serial.println("Connected.");
  server.begin();
}

void loop(){
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /Auto/on") >= 0) {
              Serial.println("Automatic Mode");
              AutoMode = "on";
              analogWrite(FanPin, 0);
            }else{
              AutoMode = "off";
            }
            if (header.indexOf("GET /Sleep/on") >= 0) {
              Serial.println("Sleep Mode");
              SleepMode = "on";
              analogWrite(FanPin, 255);
            }else{
              SleepMode = "off";
            }
            if (header.indexOf("GET /Low/on") >= 0) {
              Serial.println("Low Mode");
              analogWrite(FanPin, 510);
              LowMode = "on";
            }else{
              LowMode = "off";
            }
            if (header.indexOf("GET /Medium/on") >= 0) {
              Serial.println("Medium Mode");
              analogWrite(FanPin, 765);
              MediumMode = "on";
            }else{
              MediumMode = "off";
            }
            if (header.indexOf("GET /High/on") >= 0) {
              Serial.println("High Mode");
              analogWrite(FanPin, 1020);
              HighMode = "on";
            }else{
              HighMode = "off";
            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>ESP8266 Web Server</h1>");
            //-----Button----
            client.println("<p>Automatic Mode " + AutoMode + "</p>");      
            if (AutoMode=="off") {
              client.println("<p><a href=\"/Auto/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/Auto/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //------------------
            //-----Button----
            client.println("<p>Sleep Mode " + SleepMode + "</p>");      
            if (SleepMode=="off") {
              client.println("<p><a href=\"/Sleep/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/Sleep/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //------------------
            //-----Button----
            client.println("<p>Low Mode " + LowMode + "</p>");      
            if (LowMode=="off") {
              client.println("<p><a href=\"/Low/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/Low/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //------------------
            //-----Button----
            client.println("<p>Medium Mode " + MediumMode + "</p>");      
            if (MediumMode=="off") {
              client.println("<p><a href=\"/Medium/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/Medium/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //------------------
            //-----Button----
            client.println("<p>High Mode " + HighMode + "</p>");      
            if (HighMode=="off") {
              client.println("<p><a href=\"/High/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/Low/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //------------------
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
