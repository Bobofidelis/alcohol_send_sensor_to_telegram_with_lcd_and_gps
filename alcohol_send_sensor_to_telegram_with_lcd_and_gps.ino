#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
// include the library code:
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

const char* ssid = "myjoy";
const char* password = "iloveyou";
#define gas 15

#define relay 14
#define buzzer 4


//#define BOTtoken "6831581865:AAH4wqYjtw7TCmQtDiPo-f8dEF6viuI1TC8"
//#define CHAT_ID "2014246209"

#define BOTtoken "7180448767:AAGjH02Em7XiPdAo8Xye9ys0BGmP7ixu6Hc"
#define CHAT_ID "5064519983"


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200); 
  Serial2.begin(9600);
  pinMode (15, INPUT);
   pinMode (4, OUTPUT);
    pinMode (14, OUTPUT);
    
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
    
   // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("OLA PROJECT");
  
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("ALCOHOL CAR");
  digitalWrite(relay, LOW);
  delay(3000);
}

void loop() {
  //int alcohol = HIGH;
  int alcohol = digitalRead(gas);
  Serial.println(alcohol);
  displayInfo();

  if(alcohol == 0) {
        lcd.clear();
         lcd.print("ALCOHOL DETECTED");
         digitalWrite(relay, LOW);
         digitalWrite(buzzer, HIGH);
           delay(1000);
          String message = "ALERT!! Alcohol detected!\n";
          bot.sendMessage(CHAT_ID, message, "");
          bot.sendMessage(CHAT_ID, "Alcohol in car detected!\nLocation: https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6), "");
          delay(1000);
     }
     else if(alcohol == 1){
             lcd.clear();
         lcd.print(" DRIVE SAFELY ");
            digitalWrite(relay, HIGH);
          digitalWrite(buzzer, LOW);
          delay(1000);
     }
}

void displayInfo()
{
   while (Serial2.available() > 0)
   if (gps.encode(Serial2.read()))
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }  
  else
  {
    Serial.print(F("INVALID"));
  }
}
