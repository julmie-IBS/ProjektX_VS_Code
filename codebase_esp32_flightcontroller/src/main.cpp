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
  IMU IMU_1 = IMU(0x68,-0.041333333333333055 , -0.018166666666666498 , -0.04316666666666685 , 1.0013333333333332 , 1.0075000000000003 , 1.0148333333333335);
  IMU IMU_2 = IMU(0x69,-0.02950000000000008 , -0.029333333333333322 , 0.14666666666666633 , 0.9921666666666669 , 0.9953333333333335 , 1.018333333333333);
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

//motorhandler_1.armMotors();



while(1)
{



  
  IMU_1.fetchData();
  IMU_2.fetchData();
  Nrf24_1.fetchData();


  CompFilter_1.calculateValues(0.0035 , 0.0247);      // Bias Calibration for ACC Values  (AngleBaised) in RAD

  






throttle_array[0]=0;
throttle_array[1]=0;
throttle_array[2]=0;
throttle_array[3]=0;

if ((count > 5000))

{
  throttle_array[0]=     (count/50);
  throttle_array[1]=    (count/50);
  throttle_array[2]=    (count/50);
  throttle_array[3]=     (count/50);
}

if ((count > 15000)) 
{
  motorhandler_1.disarmMotors();
}

if ((count > 15200)) {while(1){}}

if ((count > 26000)) 
{
  throttle_array[0]=0;
  throttle_array[1]=0;
  throttle_array[2]=0;
  throttle_array[3]=0;
  
}



    ////// TEST ACC //////

/*

Serial.print("IMU1z,");
Serial.print(IMU_1.m_imuAccZ, 4);
Serial.print(",");
Serial.print("IMU2z,");
Serial.print(IMU_2.m_imuAccZ, 4);
Serial.print(",");
Serial.print("IMU1y,");
Serial.print(IMU_1.m_imuAccY, 4);
Serial.print(",");
Serial.print("IMU2y,");
Serial.print(IMU_2.m_imuAccY, 4);
Serial.print(",");
Serial.print("IMU1x,");
Serial.print(IMU_1.m_imuAccX, 4);
Serial.print(",");
Serial.print("IMU2x,");
Serial.print(IMU_2.m_imuAccX, 4);
Serial.println(",");

*/




/// GYRO ////    

/*
Serial.print("IMU1x,");                     // X -> Roll Bodyframe
Serial.print(IMU_1.m_imuGyroX);
Serial.print(",");
Serial.print("IMU2x,");
Serial.print(IMU_2.m_imuGyroX);


Serial.print(",");                          // Y -> Pitch Bodyframe
Serial.print("IMU1y,");
Serial.print(IMU_1.m_imuGyroY);
Serial.print(",");
Serial.print("IMU2y,");
Serial.print(IMU_2.m_imuGyroY);


Serial.print(",");


Serial.print("IMU1x,");
Serial.print(IMU_1.m_imuGyroZ);
Serial.print(",");
Serial.print("IMU2x,");
Serial.print(IMU_2.m_imuGyroZ);
Serial.println(",");

*/





///////////////


 //      COMP FILTER TESTING

 

Serial.print("UseACC,");
Serial.print(CompFilter_1.getUseAcc());
Serial.print(",");

Serial.print("roll,");
Serial.print(CompFilter_1.getRollHatDeg());
Serial.print(",");
Serial.print("pitch,");

Serial.print(CompFilter_1.getPitchHatDeg());
Serial.println(",");








motorhandler_1.writeToMotorsSave(throttle_array);




///////////////

/*

motorhandler_1.writeToMotors(throttle_array);


Serial.print("roll,");

Serial.print(CompFilter_1.getRollHat());
Serial.print(",");
Serial.print("pitch,");

Serial.print(CompFilter_1.getPitchHat());
Serial.println(",");

*/

//delay(0.01);

count++;

}



//raspberry.sendSensorPaket2Pi(sensorarray,33);
//delay(10);
//raspberry.getThrustPaketfromPi();





}