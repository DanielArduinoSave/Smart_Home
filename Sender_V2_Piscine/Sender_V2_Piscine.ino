//       Sender PISCINE id 707



#include <OneWire.h>
         int oneWireBus = 4;
         OneWire oneWire(oneWireBus);
#include <DallasTemperature.h>
         DallasTemperature sensors(&oneWire);
         int temp;
         int humi;

#include <ESP8266WiFi.h>
#include <espnow.h>
                                    //  REPLACE WITH RECEIVER MAC Address
          uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x64, 0x76, 0x64};  //  ESP32_New_Home module

#include "MapFloat.h"
#define   PIN_ANALOG A0

#define BOARD_ID 707 // MAC Address: 
    int analogValue;
    int batterie;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
                                int id;
                                int temp;
                                int humi;
                               bool flag;
                                int analogValue;
                                int batterie;
                              } struct_message;

// Create a struct_message called test to store variables to be sent
struct_message myDataSend;
unsigned long timeSleep = (60*60)*1000000;// pour 5 minutes en microsecondes
//unsigned long timeSleep = (30)*1000000;// pour 30 secondes en microsecondes


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  
     Serial.print("\r\nLast Packet Send Status: ");
     if (sendStatus == 0){
                           Serial.println("Delivery success");
                         } else {
                           Serial.println("Delivery fail");
                         }

}

 
void setup() {

     Serial.begin(115200); Serial.println();
     sensors.begin();
      
     // Set device as a Wi-Fi Station
     WiFi.mode(WIFI_STA);
     WiFi.disconnect();

     // Init ESP-NOW
     if (esp_now_init() != 0) {
                                Serial.println("Error initializing ESP-NOW");
                                return;
                              } else { 
                                Serial.println("ESP-NOW initialise");
                              }

     // Set ESP-NOW role
     esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

     // Once ESPNow is successfully init, we will register for Send CB to
     // get the status of Trasnmitted packet
     esp_now_register_send_cb(OnDataSent);
  
     // Register peer
     esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

     
}

 
void loop() {

     // Set values to send
     Serial.println("\n\n");

     // Read the Analog Value of A0 pin.
     analogValue = analogRead(A0);  
     Serial.print("analogValue: "); Serial.print(analogValue);
     // Computation for the Voltage Value using mapFloat
     batterie = (mapFloat(analogValue, 0, 1024, 0, 4.7)*100); 
     Serial.print("\t\tBatterie: "); Serial.println(batterie);
   
     sensors.requestTemperatures();
     float temperatureC = sensors.getTempCByIndex(0);
     Serial.println(temperatureC);
     temp = int (temperatureC);
     Serial.println(temp);
     
     myDataSend.id = BOARD_ID;
     myDataSend.temp = temp;
     myDataSend.humi = humi;
     myDataSend.flag = random(0, 1);
     myDataSend.analogValue = analogValue;
     myDataSend.batterie = batterie;

     // Send message via ESP-NOW
     esp_now_send(0, (uint8_t *) &myDataSend, sizeof(myDataSend));
  
     ESP.deepSleep(timeSleep);
    
}
