#include <ESP8266WiFi.h>

#include <DHT.h>

#include <WiFiClient.h>

#include <ESP8266WebServer.h>

#include <ESP8266mDNS.h>


/************************* Pin Definition *********************************/



//DHT11 for reading temperature and humidity value

#define DHTPIN   D7
String temp;
String hum;
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);


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



void setup(){

  // Debug console

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

  dht.begin();

  server.on("/", handleRoot); // http://localIPAddress/
  server.on("/dht-temp", []() // http://localIPAddress/dht-temp

  {

    //int t = dht.readTemperature();
    int t = -4;
    temp = String(t);
    server.send(200, "text/plain", temp);

  });


  server.on("/dht-hum", []()  // http://localIPAddress/dht-hum

  {

    //int h = dht.readHumidity();
    int h = 10;
    hum = String(h);
    server.send(200, "text/plain", hum);

  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

}


void loop(){

  server.handleClient();

}
