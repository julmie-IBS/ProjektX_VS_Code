#include "complementary_filter.h"

//https://www.youtube.com/watch?v=RZd6XDx5VXo&t=220s
// todo
// i2c maybe wrong (0x3C >> 1) ????????



    
    // constructor call
    CompFilter::CompFilter(IMU* IMU_1, IMU* IMU_2)
    {
        this->m_IMU_1=IMU_1;
        this->m_IMU_2=IMU_2;
    }

    float CompFilter::getRollHatDeg()
    {
        return (this->rollHat)*57.2958;
    }  


    float CompFilter::getPitchHatDeg()
    {
        return (this->pitchHat)*57.2958;
    }

    float CompFilter::getRollHatRad()
    {
        return this->rollHat;
    }  


    float CompFilter::getPitchHatRad()
    {
        return this->pitchHat;
    }





    
    
    void CompFilter::calculateValues()
    {
      
        /////////////////////////////  ACCELEROMETER  //////////////////////////////

        float AccX_IMU1 = this->m_IMU_1->m_imuAccX;
        float AccX_IMU2 = this->m_IMU_2->m_imuAccX;
        float AccY_IMU1 = this->m_IMU_1->m_imuAccY;
        float AccY_IMU2 = this->m_IMU_2->m_imuAccY;
        float AccZ_IMU1 = this->m_IMU_1->m_imuAccZ;
        float AccZ_IMU2 = this->m_IMU_2->m_imuAccZ;
        float Acc_Roll_IMU1 = atanf(AccY_IMU1/AccZ_IMU1);          
        float Acc_Roll_IMU2 = atanf(AccY_IMU2/AccZ_IMU2);          
        float Acc_Pitch_IMU1 = -asinf(AccX_IMU1/1);          
        float Acc_Pitch_IMU2 = -asinf(AccX_IMU2/1);          
        //TODO Check why -asinf ???????  YT  6:06



        ///////////////////////////  GYRO  ////////////////////////////////////////

        ///////////////   IMU 1 /////////////////////////
        float imuGyroX_Roll_imu1_rps =  this->m_IMU_1->m_imuGyroX;      //Roll
        float imuGyroY_Pitch_imu1_rps = this->m_IMU_1->m_imuGyroY;      //Pitch 
        float imuGyroZ_Yaw_imu1_rps =   this->m_IMU_1->m_imuGyroZ;      //Yaw
        // p -> Roll      q -> Pitch   r -> Yaw
        float rollHatRefFrame_imu1_rps = imuGyroX_Roll_imu1_rps + tanf(pitchHatLastCycle) * (sinf(rollHatLastCycle) * imuGyroY_Pitch_imu1_rps + cosf(rollHatLastCycle) * imuGyroZ_Yaw_imu1_rps);
        float pitchHatRefFrame_imu1_rps =                                                    cosf(rollHatLastCycle) * imuGyroY_Pitch_imu1_rps - sinf(rollHatLastCycle) * imuGyroZ_Yaw_imu1_rps;
        // Integrate 
       
        float rollHatRefFrame_imu1_rad = rollHatLastCycle + rollHatRefFrame_imu1_rps * SAMPLETIME;
        float pitchHatRefFrame_imu1_rad = pitchHatLastCycle + pitchHatRefFrame_imu1_rps * SAMPLETIME;


        ///////////////   IMU 2 /////////////////////////
        float imuGyroX_Roll_imu2_rps =  this->m_IMU_2->m_imuGyroX;      //Roll
        float imuGyroY_Pitch_imu2_rps = this->m_IMU_2->m_imuGyroY;      //Pitch 
        float imuGyroZ_Yaw_imu2_rps =   this->m_IMU_2->m_imuGyroZ;      //Yaw
        // p -> Roll      q -> Pitch   r -> Yaw
        float rollHatRefFrame_imu2_rps = imuGyroX_Roll_imu2_rps + tanf(pitchHatLastCycle) * (sinf(rollHatLastCycle) * imuGyroY_Pitch_imu2_rps + cosf(rollHatLastCycle) * imuGyroZ_Yaw_imu2_rps);
        float pitchHatRefFrame_imu2_rps =                                                    cosf(rollHatLastCycle) * imuGyroY_Pitch_imu2_rps - sinf(rollHatLastCycle) * imuGyroZ_Yaw_imu2_rps;
        


        // Integrate 
        float rollHatRefFrame_imu2_rad = rollHatLastCycle + rollHatRefFrame_imu2_rps * SAMPLETIME;
        float pitchHatRefFrame_imu2_rad = pitchHatLastCycle + pitchHatRefFrame_imu2_rps * SAMPLETIME;




        //Complementary Filter

        
        rollHat  =   0.49 * rollHatRefFrame_imu1_rad + 0.49 * rollHatRefFrame_imu2_rad  +  Acc_Roll_IMU1 * 0.01 + Acc_Roll_IMU2 * 0.01;
        pitchHat =   0.49 * pitchHatRefFrame_imu1_rad + 0.49 * pitchHatRefFrame_imu2_rad +  Acc_Pitch_IMU1 * 0.01 + Acc_Pitch_IMU2 *0.01;
        rollHatLastCycle = rollHat;
        pitchHatLastCycle = pitchHat;
        

        /*
        rollHat  =  Acc_Roll_IMU1 * 0.5 + Acc_Roll_IMU2 * 0.5;
        pitchHat =  Acc_Pitch_IMU1 *0.5 + Acc_Pitch_IMU2 *0.5;
        rollHatLastCycle = rollHat;
        pitchHatLastCycle = pitchHat;
        */







    }

    
   