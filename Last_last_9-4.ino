/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/
#include<EEPROM.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define EEPROM_SIZE 8
char password[4];
char initial_password[4],new_password[4];
char* master_password = "5050";//passo=word defult 1234 
int i=0;

char key_pressed=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','F'}};
  
byte rowPins[ROWS] ={25,14,12,13}; //connect to the row pinouts of the keypad
byte colPins[COLS] =  {33,32,27,26}; //connect to the column pinouts of the keypad

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);




int atemp;
static BLEUUID serviceUUID("0000aaaa-0000-1000-8000-00805f9b34fb");
int scanTime = 10; //In seconds
#define Buzzer 18 // buzzer GPIO18
#define LED 19
#define DOOR 23
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
RTC_DATA_ATTR int bootCount = 0;
int doorlock=0;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan

      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
       //Serial.printf("22222222 %s ", advertisedDevice.getServiceUUID().toString().c_str());
          if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {
    Serial.print("Found our device!  address: ");
    Serial.println(advertisedDevice.getRSSI());
    delay(500);
  doorlock = 1;
       pBLEScan->stop();
    }

    }
};

void unlockDoor()
{
    Serial.printf("from void");
    digitalWrite(LED, HIGH);
    lcd.clear();
    lcd.print("Open");
    digitalWrite(Buzzer, HIGH);
        digitalWrite(DOOR, HIGH);
     delay(3000);
    Serial.println("off");
    digitalWrite(DOOR, LOW);
    digitalWrite(LED, LOW);
     digitalWrite(Buzzer, LOW);  
}
void Buzzing(){

    digitalWrite(LED, HIGH);
    digitalWrite(Buzzer, HIGH);
     delay(900);
    digitalWrite(LED, LOW);
     digitalWrite(Buzzer, LOW);  
}
void Worngcode()
{
 Buzzing();
 Buzzing();
 Buzzing();
 Buzzing();
 Buzzing();
 atemp++;
if (atemp>3){
  delay(3000);
  atemp=0; 
  Serial.println("InWorng");
}
}

#define TOUTCH_PIN T0 // ESP32 Pin D4
int touch_value = 100;
void setup() {
  

  Serial.begin(115200);
    while (!EEPROM.begin(EEPROM_SIZE)) {
        true;}
  lcd.begin();
  lcd.print("Abdullah Omran");

  delay(2000);
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);

    Serial.println(EEPROM.read(0));
    Serial.println(EEPROM.read(1));
    Serial.println(EEPROM.read(2));
    Serial.println(EEPROM.read(3));
  Serial.println("Scanning...");
    pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
    pinMode(DOOR, OUTPUT);
  digitalWrite(DOOR, LOW);
    pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

}

void loop() {
    touch_value = touchRead(TOUTCH_PIN);
  if (touch_value < 40)
  {
  lcd.clear();
  lcd.print("Hello");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan

  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(scanTime);

  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  }
 key_pressed = keypad_key.getKey();
  if(key_pressed=='#')
    change();

  if (key_pressed)
  {
    password[i++]=key_pressed;
    lcd.print(key_pressed);
      }

  if(i==4)

  {

    delay(200);
    for(int j=0;j<4;j++)
      initial_password[j]=EEPROM.read(j);
    if(!(strncmp(password, initial_password,4)))

    {

      lcd.clear();
      lcd.print("Pass Accepted");
      delay(2000);
      unlockDoor();
      lcd.setCursor(0,1);
      lcd.print("Pres # to change");
      delay(2000);

      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0,1);
      i=0;}

   else if (!(strncmp(password, master_password,4))){
      for(int j=0;j<4;j++)
       EEPROM.write(j, j+49);
      for(int j=0;j<4;j++)
       initial_password[j]=EEPROM.read(j);
       Serial.println("Hard reset");
       i=0;}

    

    else

    {
      lcd.clear();
      lcd.print("Wrong Password");
      Worngcode();
      lcd.setCursor(0,1);
      lcd.print("Pres # to Change");
      delay(2000);
      
      lcd.clear();
      lcd.print("Enter Password");
      lcd.setCursor(0,1);
      i=0;
    }
  }

}

void change()

{

  int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<4)

  {
    char key=keypad_key.getKey();
    if(key)

    {
      new_password[j++]=key;
      lcd.print(key);

       

    }
    key=0;
  }
  delay(500);




  if((strncmp(new_password, initial_password, 4)))

  {
    lcd.clear();
    lcd.print("Wrong Password");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    delay(1000);

  }

  else

  {

    j=0;
    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=keypad_key.getKey();
      if(key)
      {
        initial_password[j]=key;
        lcd.print(key);
        EEPROM.write(j,key);
        EEPROM.commit();
            Serial.println(EEPROM.read(j));
        j++;
      }
    }
    lcd.print("Pass Changed");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  key_pressed=0;

} 


