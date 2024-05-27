#include <ESP8266WiFi.h>
#include <espnow.h>

#define ldr_pin A0

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xCC, 0x50, 0xE3, 0x46, 0x7B, 0x2F}; //CC:50:E3:46:7B:2F

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 2

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id;
    int gen;
    int cons;
} struct_message;

// Create a struct_message called test to store variables to be sent
struct_message myData;

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
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}
 
void loop() {
    //int val= analogRead(ldr_pin);
    //Serial.println(val);
    
  if (analogRead(ldr_pin)<100) {
   // grater consumption >>> dark
    myData.id = BOARD_ID;
    myData.gen = random(10, 14);
    myData.cons = random(15, 20);
    
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    //analogWrite(led_pin,100);
    Serial.println("Excess power -");

  }
  else{
    // grater generation >>> light
    myData.id = BOARD_ID;
    myData.gen = random(15, 20);
    myData.cons = random(10, 14);
    
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    //analogWrite(led_pin,255);
    Serial.println("Excess power +");
  }
  delay(500);
}
