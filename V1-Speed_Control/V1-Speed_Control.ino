#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"
std::unique_ptr<ESP8266WebServer> server;
void handleRoot() {
  String html ="<!DOCTYPE html> <html> <head> <title>Arduino Ajax LED Button Control</title> <script> //http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html# strLED1 = \"\"; strLED2 = \"\"; var LED2_state = 0; function GetArduinoIO() { nocache = \"&nocache=\" + Math.random() * 1000000; var request = new XMLHttpRequest(); request.onreadystatechange = function() { if (this.readyState == 4) { if (this.status == 200) { if (this.responseXML != null) { var count; // LED 1 if (this.responseXML.getElementsByTagName('LED')[0].childNodes[0].nodeValue === \"checked\") { document.LED_form.LED1.checked = true; } else { document.LED_form.LED1.checked = false; } // LED 2 if (this.responseXML.getElementsByTagName('LED')[1].childNodes[0].nodeValue === \"on\") { document.getElementById(\"LED2\").innerHTML = \"LED 2 is ON (D7)\"; LED2_state = 1; } else { document.getElementById(\"LED2\").innerHTML = \"LED 2 is OFF (D7)\"; LED2_state = 0; } } } } } // send HTTP GET request with LEDs to switch on/off if any request.open(\"GET\", \"ajax_inputs\" + strLED1 + strLED2 + nocache, true); request.send(null); setTimeout('GetArduinoIO()', 1000); strLED1 = \"\"; strLED2 = \"\"; } function GetButton1() { if (LED2_state === 1) { LED2_state = 0; strLED2 = \"&LED2=0\"; } else { LED2_state = 1; strLED2 = \"&LED2=1\"; } } </script> <style> .IO_box { float: left; margin: 0 20px 20px 0; border: 1px solid blue; padding: 0 5px 0 5px; width: 120px; } h1 { font-size: 120%; color: blue; margin: 0 0 10px 0; } h2 { font-size: 85%; color: #5734E6; margin: 5px 0 5px 0; } p, form, button { font-size: 80%; color: #252525; } .small_text { font-size: 70%; color: #737373; } </style> </head> <body onload=\"GetArduinoIO()\"> <h1>Hepason Fan</h1> <div class=\"IO_box\"> <h2>Sleep Mode</h2> <button type=\"button\" id=\"SleepMode\" onclick=\"GetButton1()\">Sleep Mode</button><br /><br /> </div> <div class=\"IO_box\"> <h2>Low Mode</h2> <button type=\"button\" id=\"LowMode\" onclick=\"GetButton2()\">Low Mode</button><br /><br /> </div> <div class=\"IO_box\"> <h2>Medium Mode</h2> <button type=\"button\" id=\"MediumMode\" onclick=\"GetButton3()\">Medium Mode</button><br /><br /> </div> <div class=\"IO_box\"> <h2>High Mode</h2> <button type=\"button\" id=\"HighMode\" onclick=\"GetButton4()\">High Mode</button><br /><br /> </div> </body> </html>";
  server->send(200, "html", html);
}
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

int MosfetControl = 4;

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect();
  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));
  server->on("/", handleRoot);
  server->on("/inline", []() {
    server->send(200, "text/plain", "this works as well");
  });
  server->onNotFound(handleNotFound);
  server->begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());

  Serial.println("Connected, starting program");
  pinMode(MosfetControl, OUTPUT);
  Serial.println("");
  Serial.println("Booting up....");
  for (int MosfetPwm = 100; MosfetPwm <= 500; MosfetPwm++) {
      analogWrite(MosfetControl, MosfetPwm);
      delay(10);
  }
  delay(5000);
  Serial.println("Boot up done. Starting program");
}

void loop() {
  for (int MosfetPwm = 255; MosfetPwm <= 1023; MosfetPwm++) {
    server->handleClient();
    if (MosfetPwm % 255 == 0){
      analogWrite(MosfetControl, MosfetPwm);
      switch (MosfetPwm) {
        case 255:
          Serial.println("Sleep Mode");
          delay(10000);
          break;
        case 510:
          Serial.println("Low Mode");
          delay(6000);
          break;
        case 765:
          Serial.println("Medium Mode");
          delay(4000);
          break;
        case 1020:
          Serial.println("High Mode");
          delay(2000);
          break;
        default:
          Serial.println("You shouldn't be seeing this.....");
          break;
      }
      Serial.println("-------------------------");
    }
    delay(20);
  }
}
