//       Sender CHAMBRE id 101



#include <ESP8266WiFi.h>
#include <espnow.h>
                                    //  REPLACE WITH RECEIVER MAC Address
          uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x64, 0x76, 0x64};  //  ESP32_New_Home module

#include <Adafruit_Sensor.h>
#include "Adafruit_Si7021.h"
          Adafruit_Si7021 sensor = Adafruit_Si7021();//  I2C = 0x40
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

#define BOARD_ID 101 // MAC Address: 
int BAT = A0;              // ADC pin
float RatioFactor = 4.08;  // Ratio V_réel / V_lu avec R_Factor à 1
int batterie = 0;

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

     Wire.begin();
     
}

 
void loop() {

     // Set values to send
     Serial.println("\n\n");

     float Tvoltage = 0.0;
     float Vvalue = 0.0, Rvalue = 0.0;
     // Read the Analog Value of A0 pin.
     // Cycle de 10 lecturea ADC
     for (unsigned int i=0; i<10; i++) {
          Vvalue = Vvalue+analogRead(BAT);
          delay(5); // ADC stable
         }
     Vvalue = (float)Vvalue / 10.0;           // Moyenne des 10 Lectures
     Rvalue = (float)(Vvalue / 1024.0) * 1; // Conversion en facteur ADC max input
     Tvoltage = Rvalue * RatioFactor;         // Application Ratio calculé
     batterie = int(Tvoltage * 100);
     Serial.print("\t\tBatterie: "); Serial.println(batterie);

     Serial.print("Humidity: ");
     humi = sensor.readHumidity();
     Serial.print(sensor.readHumidity(), 2);
     Serial.print("\t\tTemperature: ");
     temp = sensor.readTemperature();
     Serial.println(sensor.readTemperature(), 2);     
   
     myDataSend.id = BOARD_ID;
     myDataSend.temp = temp;
     myDataSend.humi = humi;
     myDataSend.flag = random(0, 1);
     myDataSend.analogValue = int (Vvalue);
     myDataSend.batterie = batterie;

     // Send message via ESP-NOW
     esp_now_send(0, (uint8_t *) &myDataSend, sizeof(myDataSend));
  
    ESP.deepSleep(timeSleep);
    
}
