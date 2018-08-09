/*
  Simple example for Receiving and Transmit decimal code for cc1101
  
  https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

int esp;   // for ESP8266 & Arduino setting. 
int pinRx; // int for Receive pin.
int pinTx; // int for Transmit pin.

RCSwitch mySwitch = RCSwitch();

int buttonPin;                 // for button pin.
int buttonState = 0;          // button state
int ccSetRx = 0;             // reset state for Receive
int long value = 5393;      // int to save value
int bits = 24;             // int to save bit number
int prot = 1;             // int to save Protocol number
int puls = 320;          // int to save pulse length

void setup() {
  Serial.begin(9600);

#ifdef ESP8266
esp = 1; pinRx = 4; pinTx = 5;  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
buttonPin = 16; // set button on pin 16 = D0.
#else
esp = 0; pinRx = 0; pinTx = 6;  // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
buttonPin = 4;  // set button on pin D4.
#endif  

//CC1101 Settings:                          (Settings with "//" are optional!)
  ELECHOUSE_cc1101.setESP8266(esp);    // esp8266 & Arduino SPI pin settings. Don´t change this line!
//ELECHOUSE_cc1101.setRxBW(16);       // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(1);    // set channel. steps from Channle spacing.0 - 255 default channel number is 1 for basic frequency.
//ELECHOUSE_cc1101.setChsp(50);     // set Channle spacing (default = 50khz) you can set 25,50,80,100,150,200,250,300,350,405.
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.Init(PA10);    // must be set to initialize the cc1101! set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30.

  pinMode(buttonPin, INPUT);     // set pin4 as input

}
void loop() {

   buttonState = digitalRead(buttonPin);  // read digital pin and save the state to int


   if (buttonState == HIGH) {         // the button is pressed. Set transmit on.

  ccSetRx = 0;                       // set resetstate to 0 for next reinit to Recive
  ELECHOUSE_cc1101.SetTx();         // set Transmit on
  mySwitch.disableReceive();       // Receiver off
  mySwitch.enableTransmit(pinTx); // Transmit on

  mySwitch.setRepeatTransmit(3);  // transmission repetitions.
  mySwitch.setProtocol(prot);    // send Received Protocol
  mySwitch.setPulseLength(puls);// send Received Delay
  mySwitch.send(value, bits);  // send Received value/bits

    Serial.print("Transmit ");
    Serial.print( value );
    Serial.print(" / ");
    Serial.print( bits );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( prot );
    Serial.print(" Delay: ");    
    Serial.println( puls );      
   }

   
  if (buttonState == LOW && ccSetRx == 0){  //the button is not pressed. set cc1101 to Receive.
    
  ELECHOUSE_cc1101.SetRx();         // set Recive on
  mySwitch.disableTransmit();      // set Transmit off
  mySwitch.enableReceive(pinRx);  // Receiver on
  ccSetRx = 1;
   }

  if (buttonState == LOW && ccSetRx == 1) {   //the button is not pressed and set receive is finish. Receive values. 



  if (mySwitch.available()){
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( mySwitch.getReceivedProtocol() );
    Serial.print(" Delay: ");    
    Serial.println( mySwitch.getReceivedDelay() );

    value =  mySwitch.getReceivedValue();        // save received Value
    bits = mySwitch.getReceivedBitlength();     // save received Bitlength
    prot = mySwitch.getReceivedProtocol();     // save received Protocol
    puls =  mySwitch.getReceivedDelay();      // save received pulse length
    
    mySwitch.resetAvailable();
   
   }
   }

}
