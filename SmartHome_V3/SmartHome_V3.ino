

//      SmartHome_V3

/*
#define ILI9488_DRIVER
        marron Vcc
        rouge  Gnd
#define bleu   TFT_MOSI 27
#define violet TFT_SCLK 14
#define orange TFT_CS   25
#define vert   TFT_DC   26
#define jaune  TFT_RST  -1//  RST 3,3vcc
#define N/C    TFT_MISO 12

   NEW  MAC Address:  0x10, 0x52, 0x1C, 0x64, 0x76, 0x64
*/


#include "FS.h"
#include "SPIFFS.h"  // For ESP32 only
#define   FS_NO_GLOBALS

//____________________________________________________________________________ 
#include <SPI.h>
#include <TFT_eSPI.h>
          TFT_eSPI tft = TFT_eSPI();
#include "TFT_eSPI.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch

//____________________________________________________________________________  
#include <WiFi.h>
#include <esp_now.h>

#include <esp_wifi.h>
// Set your new MAC Address
uint8_t newMACAddress[] = {0x10, 0x52, 0x1C, 0x64, 0x76, 0x64};

int id;
int analogValue;
int batterie;
bool displayData = false;

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
struct_message myDataReceve;

//____________________________________________________________________________  

     unsigned long delayTime_05 = (5*60)*1000;// pour 5 minutes en millisecondes
     unsigned long delayTime_10 = (10*60)*1000;// pour 5 minutes en millisecondes
     unsigned long delayTime_60 = (60*60)*1000;// pour 1 heure en millisecondes
//     unsigned long delayTime = (30)*1000;// pour Time en millisecondes
     unsigned long currentTime = 0;
     unsigned long previousTime_chambre = 0;
     unsigned long previousTime_salon = 0;
     unsigned long previousTime_bain = 0;   
     unsigned long previousTime_bureau = 0;  
     unsigned long previousTime_garage = 0; 
     unsigned long previousTime_pergola = 0; 
     unsigned long previousTime_piscine = 0; 
     unsigned long previousTime_fosse = 0;     
     unsigned long previousTime_celier = 0;
     unsigned long previousTime_carport = 0;          

//____________________________________________________________________________  


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

     memcpy(&myDataReceve, incomingData, sizeof(myDataReceve));
     Serial.print("Bytes received: ");
     Serial.println(len); delay(50);
     Serial.print("idModule: ");
     Serial.println(myDataReceve.id); delay(50);
     Serial.print("Temperature: ");
     Serial.println(myDataReceve.temp); delay(50);
     Serial.print("Humidite: ");
     Serial.println(myDataReceve.humi); delay(50);
     Serial.print("Etat Flag: ");
     Serial.println(myDataReceve.flag); delay(50);
     Serial.print("Lecture analogValue: ");
     Serial.println(myDataReceve.analogValue);
     Serial.print("Batterie: ");
     Serial.println(myDataReceve.batterie); delay(50);
     Serial.println();     
     displayData = true;


     if (myDataReceve.id == 101 && displayData == true) {
         drawBmp("/chambre_100.bmp", 0, 220);
         drawBmp("/chambre.bmp", 0, 0);
         tft.drawString("chambre", 100, 9, FONT4);   
         tft.fillRect(80, 30, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 35, FONT6); tft.drawString("o", 155, 30, FONT4);
         drawBmp("/humid_20.bmp", 155, 50);
         tft.drawString(String(myDataReceve.humi), 180, 50, FONT4); tft.drawString("%", 210, 50, FONT4);
         if (myDataReceve.analogValue >= 888) {
             tft.drawRoundRect(0, 0, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.analogValue >= 864 && myDataReceve.batterie < 888) {          
             tft.drawRoundRect(0, 0, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.analogValue < 864) {          
             tft.drawRoundRect(0, 0, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;
         previousTime_chambre = currentTime;      
        }

     if (myDataReceve.id == 202 && displayData == true) {
         drawBmp("/salon.bmp", 240, 0);
         tft.drawString("salon", 340, 9, FONT4);          
         tft.fillRect(320, 30, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 390, 35, FONT6); tft.drawString("o", 395, 30, FONT4);
         drawBmp("/humid_20.bmp", 395, 50);
         tft.drawString(String(myDataReceve.humi), 420, 50, FONT4); tft.drawString("%", 450, 50, FONT4);
         if (myDataReceve.batterie >= 888) {
             tft.drawRoundRect(240, 0, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.batterie >= 864 && myDataReceve.batterie < 888) {          
             tft.drawRoundRect(240, 0, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.batterie < 864) {          
             tft.drawRoundRect(240, 0, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;
         previousTime_salon = currentTime;      
        }

     if (myDataReceve.id == 303 && displayData == true) {
         drawBmp("/bain.bmp", 0, 80);
         tft.drawString("bain", 100, 89, FONT4);          
         tft.fillRect(80, 110, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 115, FONT6); tft.drawString("o", 155, 110, FONT4);
         drawBmp("/humid_20.bmp", 155, 130);
         tft.drawString(String(myDataReceve.humi), 180, 130, FONT4); tft.drawString("%", 210, 130, FONT4);
         if (myDataReceve.batterie >= 888) {
             tft.drawRoundRect(0, 80, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.batterie >= 864 && myDataReceve.batterie < 888) {          
             tft.drawRoundRect(0, 80, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.batterie < 864) {          
             tft.drawRoundRect(0, 80, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;
         previousTime_bain = currentTime;      
        }

     if (myDataReceve.id == 404 && displayData == true) {
         drawBmp("/bureau.bmp", 240, 80);
         tft.drawString("bureau", 340, 89, FONT4);          
         tft.fillRect(320, 110, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 390, 115, FONT6); tft.drawString("o", 395, 110, FONT4);
         drawBmp("/humid_20.bmp", 395, 130);
         tft.drawString(String(myDataReceve.humi), 420, 130, FONT4); tft.drawString("%", 450, 130, FONT4);
         if (myDataReceve.analogValue >= 888) {
             tft.drawRoundRect(240, 80, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.analogValue >= 864 && myDataReceve.batterie < 888) {          
             tft.drawRoundRect(240, 80, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.analogValue < 864) {          
             tft.drawRoundRect(240, 80, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;   
         previousTime_bureau = currentTime;
        }

     if (myDataReceve.id == 505 && displayData == true) {
         if (myDataReceve.flag == false) {         
             drawBmp("/garageFerme.bmp", 0, 160);
             tft.setTextColor(TFT_LIGHTGREY);             
             tft.drawString("garage", 100, 169, FONT4);            
           } else {             
             drawBmp("/garageOuvert.bmp", 0, 160);
             tft.setTextColor(TFT_RED);
             tft.drawString("garage", 100, 169, FONT4);
             tft.setTextColor(TFT_LIGHTGREY);  
           }
         tft.fillRect(80, 190, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 195, FONT6); tft.drawString("o", 155, 190, FONT4);
         drawBmp("/humid_20.bmp", 155, 210);
         tft.drawString(String(myDataReceve.humi), 180, 210, FONT4); tft.drawString("%", 210, 210, FONT4);
         tft.drawRoundRect(0, 160, 80, 80, 5, TFT_GREEN);
         displayData = false;
         previousTime_garage = currentTime;               
        }

     if (myDataReceve.id == 707 && displayData == true) {
         drawBmp("/piscine.bmp", 0, 240);
         tft.drawString("piscine", 100, 249, FONT4);          
         tft.fillRect(80, 270, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 275, FONT6); tft.drawString("o", 155, 270, FONT4);
//         drawBmp("/humid_20.bmp", 155, 290);
//         tft.drawString("89", 180, 290, FONT4); tft.drawString("%", 210, 290, FONT4);
         if (myDataReceve.batterie >= 600) {
             tft.drawRoundRect(0, 240, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.batterie >= 550 && myDataReceve.batterie < 600) {          
             tft.drawRoundRect(0, 240, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.batterie < 550) {          
             tft.drawRoundRect(0, 240, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;
         previousTime_piscine = currentTime;      
        }

     if (myDataReceve.id == 909 && displayData == true) {
         if (myDataReceve.flag == false) {         
             drawBmp("/celierAlarme.bmp", 320, 240);      
           } else {             
             drawBmp("/celierOk.bmp", 320, 240);
           }
         displayData = false;
         previousTime_celier = currentTime;               
        }

/*  
     delay(3000);
     drawBmp("/fosseAlarme.bmp", 240, 240);
//     tft.drawRoundRect(240, 240, 80, 80, 5, TFT_RED);
     delay(3000);

     drawBmp("/celierAlarme.bmp", 320, 240);
//     tft.drawRoundRect(320, 240, 80, 80, 5, TFT_GREEN);
     delay(3000);

     drawBmp("/carportFerme.bmp", 400, 240);
//     tft.drawRoundRect(400, 240, 80, 80, 5, TFT_ORANGE);
     delay(3000);

     drawBmp("/garageFerme.bmp", 0, 160);
//     tft.drawRoundRect(0, 160, 80, 80, 5, TFT_ORANGE);     
     delay(3000);
*/ 
       
}

//____________________________________________________________________________  


void setup() {

     Serial.begin(115200); Serial.println(); Serial.println();

  WiFi.mode(WIFI_STA);
  
  Serial.print("[OLD] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  // ESP32 Board add-on before version < 1.0.5
  //esp_wifi_set_mac(ESP_IF_WIFI_STA, &newMACAddress[0]);
  
  // ESP32 Board add-on after version > 1.0.5
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

     if (!SPIFFS.begin()) {
         Serial.println("SPIFFS initialisation failed!");
         tft.drawString("Erreur SPIFFS !", 290, 30, 4);
         while (1) yield();
        }
     Serial.println("\r\nSPIFFS Ok");

     //Set device as a Wi-Fi Station
     WiFi.mode(WIFI_STA);

     //Init ESP-NOW
     if (esp_now_init() != ESP_OK) {
         Serial.println("Error initializing ESP-NOW");
         return;
        } else { 
          Serial.println("ESP-NOW initialise");
        }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
     esp_now_register_recv_cb(OnDataRecv);

     tft.init();
     tft.setRotation(1);
     tft.fillScreen(TFT_BLACK);
     tft.setTextColor(TFT_LIGHTGREY); 

     drawBmp("/smart_home.bmp", 0, 0);
     delay(5000);
     tft.fillScreen(TFT_BLACK);     
     
     drawBmp("/chambre.bmp", 0, 0);
     tft.drawString("chambre", 100, 9, FONT4);          
     
     drawBmp("/salon.bmp", 240, 0);
     tft.drawString("salon", 340, 9, FONT4);          

     drawBmp("/bain.bmp", 0, 80);
     tft.drawString("bain", 100, 89, FONT4);          

     drawBmp("/bureau.bmp", 240, 80);
     tft.drawString("bureau", 340, 89, FONT4);          

     drawBmp("/garageOuvert.bmp", 0, 160);
     tft.drawString("garage", 100, 169, FONT4);          

     drawBmp("/pergola.bmp", 240, 160);
     tft.drawString("pergola", 340, 169, FONT4);          
/*     tft.fillRect(320, 190, 155, 45, TFT_BLACK);
     tft.drawString(" 23", 320, 195, FONT6); tft.drawString("o", 395, 190, FONT4);
     drawBmp("/humid_20.bmp", 395, 210);
     tft.drawString("55", 420, 210, FONT4); tft.drawString("%", 450, 210, FONT4);
     tft.drawRoundRect(240, 160, 80, 80, 5, TFT_GREEN);
*/
     drawBmp("/piscine.bmp", 0, 240);
     tft.drawString("piscine", 100, 249, FONT4);          
/*     tft.fillRect(80, 270, 155, 45, TFT_BLACK);
     tft.drawString(" 29", 80, 275, FONT6); tft.drawString("o", 155, 270, FONT4);
     drawBmp("/humid_20.bmp", 155, 290);
     tft.drawString("89", 180, 290, FONT4); tft.drawString("%", 210, 290, FONT4);
     tft.drawRoundRect(0, 240, 80, 80, 5, TFT_GREEN);
*/
     drawBmp("/fosseAlarme.bmp", 240, 240);
//     tft.drawRoundRect(240, 240, 80, 80, 5, TFT_GREEN);

     drawBmp("/celierAlarme.bmp", 320, 240);
//     tft.drawRoundRect(320, 240, 80, 80, 5, TFT_ORANGE);

     drawBmp("/carportOuvert.bmp", 400, 240);
//     tft.drawRoundRect(400, 240, 80, 80, 5, TFT_RED);
            
}


void loop() {
/*
     currentTime = millis();

     if ((currentTime - previousTime_chambre) > delayTime_05) {
         previousTime_chambre = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 0, 0);
         tft.drawRoundRect(0, 0, 80, 80, 5, TFT_CYAN);
         delay(50);  
        }
     if ((currentTime - previousTime_salon) > delayTime_05) {
         previousTime_salon = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 240, 0);
         tft.drawRoundRect(240, 0, 80, 80, 5, TFT_CYAN);  
         delay(50); 
        }
     if ((currentTime - previousTime_bain) > delayTime_05) {
         previousTime_bain = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 0, 80);
         tft.drawRoundRect(0, 80, 80, 80, 5, TFT_CYAN);  
         delay(50); 
        }     
     if ((currentTime - previousTime_bureau) > delayTime_05) {
         previousTime_bureau = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 240, 80);
         tft.drawRoundRect(240, 80, 80, 80, 5, TFT_CYAN);  
         delay(50); 
        }
     if ((currentTime - previousTime_garage) > delayTime_05) {
         previousTime_garage = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 0, 160);
         tft.drawRoundRect(0, 160, 80, 80, 5, TFT_CYAN);  
         delay(50); 
        }
     if ((currentTime - previousTime_piscine) > delayTime_60) {
         previousTime_piscine = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 0, 240);
         tft.drawRoundRect(0, 240, 80, 80, 5, TFT_CYAN);  
         delay(50); 
        }
     if ((currentTime - previousTime_celier) > delayTime_10) {
         previousTime_celier = currentTime; Serial.println("Depassement data time"); 
         drawBmp("/signal.bmp", 320, 240);
         tft.drawRoundRect(320, 240, 80, 80, 5, TFT_CYAN);  
         delay(50); 
        }
*/                        
}
