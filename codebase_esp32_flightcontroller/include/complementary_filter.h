#ifndef Complementary_Filter_h
#define Complementary_Filter_h

#include "arduino.h"
#include <sensor_imu.h>


#define SAMPLETIME 0.005

class CompFilter {
public:
    CompFilter(IMU* IMU_1, IMU* IMU_2);  // Deklaration des Konstruktors
    void calculateValues();
    float getRollHatDeg();  
    float getPitchHatDeg();
    float getRollHatRad();  
    float getPitchHatRad();



private:

    IMU* m_IMU_1;
    IMU* m_IMU_2;
    float pitchHat;
    float rollHat;

    float pitchHatLastCycle=0;
    float rollHatLastCycle=0;



    float Acc_Roll_IMU1;         
    float Acc_Roll_IMU2;         
    float Acc_Pitch_IMU1;        
    float Acc_Pitch_IMU2;        


    int16_t calculateAccRoll();
    int16_t calculateAccPitch();

};



#endif