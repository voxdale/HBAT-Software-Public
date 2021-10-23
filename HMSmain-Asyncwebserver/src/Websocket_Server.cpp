/*
  asyncserver.cpp - HBAT asyncserver library
  Copyright (c) 2021 Zacariah Austin Heim.
*/
#include <WebSocketsServer.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

#define DEBUG 0

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x) Serial.printf(x)

#else
#define debug(x)
#define debugln(x)
#define debugf(x)
#endif

uint8_t temprature_sens_read();
const char* ssid = "LoveHouse2G";
const char* password = "";

WebSocketsServer webSocket = WebSocketsServer(8080);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      debug.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        debug.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      debug.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      webSocket.sendTXT(num, "This is a test of sample text");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      debug.printf("[%u] get binary length: %u\n", num, length);
      //hexdump(payload, length);

      // send message to client
      webSocket.sendBIN(num, payload, length);
      break;
  }
}

void setup()
{
  Serial.begin(115200);
  debug.println();

  WiFi.begin(ssid, password);

  debug.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    debug.print(".");
  }
  debug.println();

  debug.print("Connected, IP address: ");
  debug.println(WiFi.localIP());

  debug.println('\n');
  debug.println("Connection established!");
  debug.print("IP address:\t");
  debug.println(WiFi.localIP());                   // Send the IP address of the ESP to the computer

  // port and URL
  webSocket.begin();
  // event handler
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  debug.print("Temperature: ");

  // Convert raw core temperature in F to Celsius degrees
  debug.print((temprature_sens_read() - 32) / 1.8);
  debug.println(" C");
  delay(1000);
  webSocket.loop();
}