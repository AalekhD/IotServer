 
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#include<string.h>
/* Set these to your desired credentials. */
#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F);
const char *ssid = "dlink";
const char *password = "";

const char *host = "http://www.sachinkale.com";  

HTTPClient http;    //Declare object of class HTTPClient

#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN D1
#define SS_PIN  D2
#define ServoPin D0

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
/*
String card1 = "71 9E 29 2E";
String card2 = "4A 11 CC 73";
String card3 = "9A A9 A6 D5";
String card4 = "F5 01 D0 65";
String card5 = "F5 C8 4F 11";
String card6 = "05 CA 4B 11";
String card7 = "45 D1 50 11";
String card8 = "8B BB EA 03";
String card9 = "65 4E 43 11";
*/
String cards[] = {"71 9E 29 2E" ,"F5 01 D0 65" , "65 4E 43 11",  "05 CA 4B 11" , "45 D1 50 11" , "8B BB EA 03", "4A 11 CC 73"};
int card_swipped;
bool valid_card;
String lcd_string="";
int last_valid_card;
Servo myservo;

void setup() {
  Serial.begin(115200);
  initialize_RFID();
  display_Lcd();
   myservo.attach(ServoPin);
   Website_Connection();
//while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  card_swipped = 0;
  valid_card = true;
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

}

void display_Lcd()
{
   Wire.begin(2,0);
  lcd.begin(16,2);   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
 lcd.setCursor(0, 0); // Move the cursor characters to the right and
                      // zero characters down (line 1).
 lcd.print("RFID CARD");  // Print HELLO to the screen, starting at 0,0.

  
 lcd.setCursor(0, 1);    // Move the cursor to the next line and print
 lcd.print("SWIPPING MACHINE"); 

  
}
void initialize_RFID()
{
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

 }

void Website_Connection(){
   WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  
  http.begin("http://192.168.0.104:5000/update/API_key=GTW89NF3/mac=6c:rf:7f:2b:0e:g8/field=2/data=7922798626");              //Specify request destination
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  //http.begin("http://192.168.43.128/c4yforum/postdemo.php");              //Specify request destination
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   
  int httpCode = http.GET();   //Send the request
  String payload = http.getString();    //Get the response payload
  Serial.println("httpcode");
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println("Payload");
  Serial.println(payload);    //Print request response payload
  http.end();  //Close connection
  
  }
  
void loop() {

  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
    Serial.println(content.substring(1));
  
  content.toUpperCase();
  String card = content.substring(1);
  /***start comparing the cards with stored cards **/
  
  for(int i =card_swipped ; i<7 ; i++)
  { 
          
          if(cards[0] ==  content.substring(1))
           {
              card_swipped=0;       
              lcd_string = "Cards Reset";
              Serial.println(lcd_string);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(lcd_string);
              break;
           } 
          else if(cards[i] == content.substring(1))
          {
            Serial.println("card checking: "+String(cards[i] ));
            card_swipped = i;
            valid_card = true;
          
          }
          else
          {
            valid_card = false;
          }
          
        if(valid_card == true)
        {
          lcd_string = "card swipped:"+String(card_swipped);
          Serial.println(lcd_string);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(lcd_string);
          last_valid_card = card_swipped;
          myservo.write(0);
          delay(1000);
          myservo.write(90);
          delay(1000);
          break;
       }
        else
       {
          lcd_string ="Wrong card swipd"; 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(lcd_string);
          lcd_string = "lst crd swipd:"+String(last_valid_card);
          lcd.setCursor(0,1);
          lcd.print(lcd_string);
       }
       
  }// end of loop comparing cards
  
  card = "";
  delay(3000);
 
}

void send_data(){
    http.begin("www.sachinkale.com",80,"/BookMe/index.php/JSController/sendToServer/31/9823169652");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  //http.begin("http://192.168.43.128/c4yforum/postdemo.php");              //Specify request destination
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   
  int httpCode = http.GET();   //Send the request
  String payload = http.getString();    //Get the response payload
  Serial.println("httpcode");
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println("Payload");
  Serial.println(payload);    //Print request response payload
  http.end();  //Close connection

  }
