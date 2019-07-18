
# Smart Door :
This is a project I was working on for a long time now, basically, it is a smart door that uses either Bluetooth or keypad to open, open with just putting your hand on the door handle. 
## Features : 

 - Uses Bluetooth or keypad to open.
 - Can change door password via the keypad directly. 
 - Bluetooth is used with a mobile phone with a sense pin " no need to press any buttons, because the sense pin connected directly to the handle". 
 - support Low Power consumption BLE to save power either on phone or the door sensor. 
 - Design from scratch starting with schematic then PCB and code. 
 - All components used in the PCB are widely available on the market.
 - 

## Introduction : 
As mention above this is all my work and it is open source to be used, this is a smart door with the feature that it can open the door only by putting a hand on the door handle with the condition that the BLE advertising device is near with a signal greater than -79 dB. 
Also, it supports opening via a keypad with a controlled password and an admin password to change the password at any time. 

## Schematic : 
![Schematic](https://github.com/abdullahomran9/Smart-Door/blob/master/schematic.png?raw=true)

The circuit on the left is the DC to DC converters, using buck converter switching regulators because it is efficient in comparison with linear regulators in case of battery use. 
There is 2 switching regulators outputting ( 3.3v and 5v), the 3v3 is needed to power the uC and the LCD screen, while the 5v is needed to power the relay. 

On the right is the brain of the project using ESP32, and CN1 is the Keypad connector, and CN3 is the LCD 16x2 connector. 


## PCB : 
![PCB](https://github.com/abdullahomran9/Smart-Door/blob/master/Capture.PNG?raw=true)

This is the PCB work, using very popular component and it is possible to see part numbers via the schematic to use either digikey.com or lcsc.com .


## Code : 
The code is still in the debugging phase, and I will be updating it until I reach the best result, to explain it : 

 1. The device will be looking for an advertiser using 16-bit 0xAAAA Service UUID, To open the door using BLE you must Download nRF connect app on Android phone and add ADVERTISER with a service UUID = AAAA  .
 2. The scanning process will happen only if the sense pin is pressed. 
 3. the Admin password is 5050.
 
## Problems : 
 1. The code still needs some debugging and fixing. 
 2. The components ( Caps and Inductors ) for the voltage regulator are not correct on the PCB and it depends on the Input Voltage. 
 3. Connector CN4 is for resetting and updating the code, but it connected to wrong pins on the ESP32. 
 

## Work needs to be done : 
I must update the schematic and the PCB to fix CN4 problem, also the code needs some user experience and finally, there must be an app on smartphones ( iOS and Android ) to support the Door. 

 
