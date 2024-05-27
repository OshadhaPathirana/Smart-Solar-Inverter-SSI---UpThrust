/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp8266-nodemcu/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
//esp-now libraries
#include <ESP8266WiFi.h>
#include <espnow.h>
//web server libraries
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int id;
    int x;
    int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

// Create an array with all the structures
struct_message boardsStruct[2] = {board1, board2};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}
//////////////////////////////////////////////////////////////////////////////////////////
String temp;
String hum;

// Your WiFi credentials.
// Set password to "" for open networks.

const char* ssid = "SLT_FIBRE";
const char* password = "osha1619";


ESP8266WebServer server(80);


void handleRoot() {

  server.send(200, "text/plain", "hello from esp8266!");

}

void handleNotFound() {


  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";

  }

  server.send(404, "text/plain", message);
}
//////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    server.on("/", handleRoot); // http://localIPAddress/
  server.on("/dht-temp", []() // http://localIPAddress/dht-temp

  {

    //int t = dht.readTemperature();
    int t = 10;//boardsStruct[1].x; /////////////////////////////////////////// data interchange
    temp = String(t);
    server.send(200, "text/plain", temp);

  });

  server.on("/dht-hum", []()  // http://localIPAddress/dht-hum

  {

    //int h = dht.readHumidity();
    int h = 10;//boardsStruct[1].y; /////////////////////////////////////////// data interchange
    hum = String(h); 
    server.send(200, "text/plain", hum);

  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
//////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void loop(){
  // Access the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  */

  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  server.on("/", handleRoot); // http://localIPAddress/
  server.on("/dht-temp", []() // http://localIPAddress/dht-temp

  {

    //int t = dht.readTemperature();
    int t = 10;//boardsStruct[1].x; /////////////////////////////////////////// data interchange
    temp = String(t);
    server.send(200, "text/plain", temp);

  });

  server.on("/dht-hum", []()  // http://localIPAddress/dht-hum

  {

    //int h = dht.readHumidity();
    int h = 10;//boardsStruct[1].y; /////////////////////////////////////////// data interchange
    hum = String(h); 
    server.send(200, "text/plain", hum);

  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
//////////////////////////////////////////////////////////////////////////////////////////////////////////

  server.handleClient();
  
  
}
