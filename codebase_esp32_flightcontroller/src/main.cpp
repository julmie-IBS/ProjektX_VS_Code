#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <raspPi.h>

void setup() {
  // put your setup code here, to run once:
 

}

raspPi raspberry = raspPi();
byte sensorarray[33]= {0, 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32};
void loop() {
  // put your main code here, to run repeatedly:


raspberry.sendSensorPaket2Pi(sensorarray,33);
delay(10);
//raspberry.getThrustPaketfromPi()





}