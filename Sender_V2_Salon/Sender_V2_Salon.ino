//       Sender SALON id 202



#include <ESP8266WiFi.h>
#include <espnow.h>
                                    //  REPLACE WITH RECEIVER MAC Address
          uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x64, 0x76, 0x64};  //  ESP32_New_Home module

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
          Adafruit_BME280 bme;
          int temp;
          int humi;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define   i2c_Address 0x3c
#define   SCREEN_WIDTH 128 // OLED display width, in pixels
#define   SCREEN_HEIGHT 64 // OLED display height, in pixels
#define   OLED_RESET -1   //   QT-PY / XIAO
          Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "MapFloat.h"
#define   PIN_ANALOG A0

#define BOARD_ID 202 // MAC Address: 
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

unsigned long timeSleep = (5*60)*1000000;// pour 5 minutes en microsecondes
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

     bme.begin(0x76);
     
}

 
void loop() {

     // Set values to send
     Serial.println("\n\n");

     // Read the Analog Value of A0 pin.
     analogValue = analogRead(A0);  
     Serial.print("analogValue: "); Serial.print(analogValue);
     // Computation for the Voltage Value using mapFloat
     batterie = (mapFloat(analogValue, 0, 980, 0, 6.5)*100); 
     Serial.print("\t\tBatterie: "); Serial.println(batterie);

     Serial.print("Humidity: ");
     humi = bme.readHumidity();
     Serial.print(bme.readHumidity(), 2);
     Serial.print("\t\tTemperature: ");
     temp = bme.readTemperature();
     Serial.println(bme.readTemperature(), 2);     
   
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
