#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <raspPi.h>

void setup() {
  // put your setup code here, to run once:
 

}

void loop() {
  // put your main code here, to run repeatedly:
raspPi raspberry = raspPi();

int rc=raspberry.init();
//Serial.println("rccode:");
Serial.println(rc);
//Serial.println("ende");
delay(5000);



}