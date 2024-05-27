
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <Adafruit_Sensor.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};// oshadha- CC:50:E3:46:7B:2F  {0xCC, 0x50, 0xE3, 0x46, 0x7B, 0x2F}
                                                                  // sithum - 48:55:19:00:75:FF  {0x48, 0x55, 0x19, 0x00, 0x75, 0xFF}
                                                                  // universal - {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

// Define variables to store incoming readings
float incomingGen;
float incomingCons;

// Updates DHT readings every 10 seconds
const long interval = 500; 

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    float gen;
    float cons;
} struct_message;

typedef struct struct_instruction {
    float relay;
    float led;
} struct_instruction;

// Create a struct_message called SendReadings to hold sensor readings
struct_instruction SendReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingGen = incomingReadings.gen;
  incomingCons = incomingReadings.cons;
}

void printIncomingReadings(){
  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Generation: ");
  Serial.print(incomingGen);
  Serial.println(" kW");
  Serial.print("Consumption: ");
  Serial.print(incomingCons);
  Serial.println(" kW");
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  
    //Set values to send
    SendReadings.relay = 6666;
    SendReadings.led = 6666;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &SendReadings, sizeof(SendReadings));

    // Print incoming readings
    printIncomingReadings();
    delay(interval);
}
