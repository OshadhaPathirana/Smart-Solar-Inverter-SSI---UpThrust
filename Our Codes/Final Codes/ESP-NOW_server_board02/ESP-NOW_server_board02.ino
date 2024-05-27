
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <Adafruit_Sensor.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress1[] = {0x48, 0x55, 0x19, 0x00, 0x75, 0xFF};// oshadha(server)- CC:50:E3:46:7B:2F  {0xCC, 0x50, 0xE3, 0x46, 0x7B, 0x2F}
uint8_t broadcastAddress2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};// sithum(b_Add1) - 48:55:19:00:75:FF  {0x48, 0x55, 0x19, 0x00, 0x75, 0xFF}
                                                                  // universal - {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

// Define variables to store incoming readings
int incomingGen;
int incomingCons;

// Updates DHT readings every 10 seconds
const long interval = 500; 

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure

typedef struct struct_instruction {
    int id;
    int relay;
    int led;
} struct_instruction;

// Create a struct_message called SendReadings to hold sensor readings
struct_instruction SendReadings;

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
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  memcpy(&myData, incomingData, sizeof(myData));
  
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  
  int gen = boardsStruct[myData.id-1].x;
  int cons = boardsStruct[myData.id-1].y;
  
  Serial.print(myData.id);
  Serial.print("x");
  Serial.print(gen);
  Serial.print("x");
  Serial.print(cons);
  Serial.println();

  if ( myData.id == 2 && gen<cons ) {

    SendReadings.id = 3;
    SendReadings.relay = 1;
    SendReadings.led = 1;
    
    esp_now_send(broadcastAddress1, (uint8_t *) &SendReadings, sizeof(SendReadings));

    Serial.println("relay ON");
  }
  else{
    SendReadings.id = 3;
    SendReadings.relay = 0;
    SendReadings.led = 0;
    
    esp_now_send(broadcastAddress1, (uint8_t *) &SendReadings, sizeof(SendReadings));
  }
  
}

 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  
  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  
    // Print incoming readings
    //printIncomingReadings();
    //delay(interval);
}
