#include <raspPi.h>
#include <sensor_imu.h>
#include <sensor_compass.h>
#include <sensor_nrf24.h>
#include <motor_handler.h>
#include <complementary_filter.h>

void setup() {}




void loop() {

  
  byte sensorarray[33]= {0, 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32};

  motorhandler motorhandler_1 = motorhandler();
  raspPi raspberry = raspPi();
  IMU IMU_1 = IMU(0x68);
  IMU IMU_2 = IMU(0x69);
  //Compass Compass_1 = Compass(0x69);
  Nrf24 Nrf24_1 = Nrf24();
  Nrf24_1.initSensor();
  IMU_1.initSensor();
  IMU_2.initSensor();
  
 
  CompFilter CompFilter_1 = CompFilter(&IMU_1,&IMU_2);
 


//Serial.begin(9600);


unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
delay(2000);



volatile uint16_t throttle_array[4]= {0,0,0,0};
int count=0;


while(1)
{

  
  IMU_1.fetchData();
  IMU_2.fetchData();

  CompFilter_1.calculateValues();

  






throttle_array[0]=0;
throttle_array[1]=0;
throttle_array[2]=0;
throttle_array[3]=0;

if ((count > 10000))

{
  throttle_array[0]=50 + (count/200);
  throttle_array[1]=50+ (count/200);
  throttle_array[2]=50+ (count/200);
  throttle_array[3]=50+ (count/200);
}


if ((count > 35000)) {while(1){}}

if ((count > 36000)) 
{
  throttle_array[0]=0;
  throttle_array[1]=0;
  throttle_array[2]=0;
  throttle_array[3]=0;
  
}




/*
  Serial.print("IMU1z,");

Serial.print(IMU_1.m_imuAccZ);
Serial.print(",");
 Serial.print("IMU2z,");

Serial.print(IMU_2.m_imuAccZ);
Serial.print(",");


 Serial.print("IMU1y,");

Serial.print(IMU_1.m_imuAccY);
Serial.print(",");
 Serial.print("IMU2y,");

Serial.print(IMU_2.m_imuAccY);
Serial.print(",");


 Serial.print("IMU1x,");

Serial.print(IMU_1.m_imuAccX);
Serial.print(",");
 Serial.print("IMU2x,");

Serial.print(IMU_2.m_imuAccX);
Serial.println(",");


*/

motorhandler_1.writeToMotors(throttle_array);


Serial.print("roll,");

Serial.print(CompFilter_1.getRollHat());
Serial.print(",");
Serial.print("pitch,");

Serial.print(CompFilter_1.getPitchHat());
Serial.println(",");



delay(1);

count++;

}







 









//raspberry.sendSensorPaket2Pi(sensorarray,33);
//delay(10);
//raspberry.getThrustPaketfromPi();





}