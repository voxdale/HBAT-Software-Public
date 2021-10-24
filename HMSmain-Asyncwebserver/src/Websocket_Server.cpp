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

#define DEBUGMODE 1
#if DEBUGMODE == 1
#define debugserial Serial1
#else
#define debugserial(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();
const char* ssid = "LoveHouse2G";
const char* password = "";

WebSocketsServer webSocket = WebSocketsServer(8080);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      debugserial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        debugserial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      debugserial.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      webSocket.sendTXT(num, "This is a test of sample text");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      debugserial.printf("[%u] get binary length: %u\n", num, length);
      //hexdump(payload, length);

      // send message to client
      webSocket.sendBIN(num, payload, length);
      break;
  }
}

void setup()
{
  Serial.begin(115200);
  debugserial.println();

  WiFi.begin(ssid, password);

  debugserial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    debugserial.print(".");
  }
  debugserial.println();

  debugserial.print("Connected, IP address: ");
  debugserial.println(WiFi.localIP());

  debugserial.println('\n');
  debugserial.println("Connection established!");
  debugserial.print("IP address:\t");
  debugserial.println(WiFi.localIP());                   // Send the IP address of the ESP to the computer

  // port and URL
  webSocket.begin();
  // event handler
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  debugserial.print("Temperature: ");

  // Convert raw core temperature in F to Celsius degrees
  debugserial.print((temprature_sens_read() - 32) / 1.8);
  debugserial.println(" C");
  delay(1000);
  webSocket.loop();
}