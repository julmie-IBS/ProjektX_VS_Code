#include <raspPi.h>
#include <sensor_imu.h>
#include <sensor_compass.h>
#include <sensor_nrf24.h>
#include <motor_handler.h>
#include <complementary_filter.h>
#include <PID.h>




  u_int8_t getNextState(u_int8_t state, Nrf24* Nrf24_1)
   {
   if((state==0) && ((Nrf24_1->m_PS3_buttons & 0x01)==0x01)){  //triangle
    return 1; 
   }
   if((Nrf24_1->m_PS3_buttons & 0x08)==0x08){ //cross
    return 0;
   }

   return 0;
  } 




void setup() {}




void loop() {




  
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
  PIDController PID_1 = PIDController(&CompFilter_1,&Nrf24_1,&motorhandler_1);
 


//Serial.begin(9600);


unsigned long previousMillis = 0;
unsigned long currentMillis = 0;




volatile uint16_t throttle_array[4]= {0,0,0,0};
int count=0;

//motorhandler_1.armMotors();


u_int8_t state = 0;


while(1)
{
  IMU_1.fetchData();
  IMU_2.fetchData();
  Nrf24_1.fetchData();

  //CompFilter_1.calculateValues(0.0035 , 0.0247);      // Bias Calibration for ACC Values  (AngleBaised) in RAD
  
  switch(state)
  {

    case 0:

    PID_1.disarmPID();
    motorhandler_1.disarmMotors();
    break;
  
    case 1:  

    motorhandler_1.armMotors();             //  1000 x writeToMotors(0,0,0,0)        add arm flag in moterhandler class do this only ones
    CompFilter_1.calculateValues(0,0);
    PID_1.calculateValues();
    motorhandler_1.writeToMotorsSave(PID_1.m_Motor_dshotValues);
    break;

    default:
    PID_1.disarmPID();
    motorhandler_1.disarmMotors();
    

  }
  
    state=getNextState(state, &Nrf24_1);

}
}