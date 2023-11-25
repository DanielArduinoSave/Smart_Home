//       Sender CELLIER id 909



#include <ESP8266WiFi.h>
#include <espnow.h>
         uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x64, 0x76, 0x64};

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

#define BOARD_ID 909

     bool etatSend;
     unsigned long delayTime = (5*60)*1000;// pour Time en millisecondes
//     unsigned long delayTime = (30)*1000;// pour Time en millisecondes
     unsigned long currentTime = 0;
     unsigned long previousTime = delayTime;

#define detectFuite 3
        bool Fuite;
        bool oldFuite;
        bool signalFuite;
        bool flag;
        

void OnDataSend (uint8_t *mac_addr, uint8_t sendStatus) {
  
     Serial.print("\r\nLast Packet Send Status: ");
     if (sendStatus == 0){
        Serial.println("Delivery success");
        etatSend = true;
     }
     else{
         Serial.println("Delivery fail");
         etatSend = false;
     }
     
}


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

     WiFi.mode(WIFI_STA);
     WiFi.disconnect();

     if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
       return;
     } 

     esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
     esp_now_register_send_cb(OnDataSend);
     esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

     pinMode (detectFuite, INPUT);
     
}


void loop() {

     Fuite = digitalRead (detectFuite);
     if (Fuite != oldFuite) {
         flag = Fuite; oldFuite = Fuite; Serial.print(flag);
         if (Fuite == !false) { 
             flag = true; Serial.println("Ciel, une fuite !");
            } else { 
             flag = false; Serial.println("Tout est OK"); 
            } 
         signalFuite = true;
         delay(500);                         
        }

       
     currentTime = millis();
     if (((currentTime - previousTime) > delayTime) || signalFuite == true) {
        previousTime = currentTime; Serial.println("Cycle verification");
        signalFuite = false; 
        Serial.print(flag);
        myDataSend.id = BOARD_ID;
        myDataSend.flag = flag;
        esp_now_send(0, (uint8_t *) &myDataSend, sizeof(myDataSend));
        }         

}
