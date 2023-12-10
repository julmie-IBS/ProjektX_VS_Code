


#include "PID.h"









PIDController::PIDController(CompFilter* CompFilter_1, Nrf24* Nrf24_1, motorhandler* motorhandler_1)

    {
        m_CompFilter_1    = CompFilter_1;
        m_Nrf24_1         = Nrf24_1;
        m_motorhandler_1  = motorhandler_1;


    }

void PIDController::armPID()
{

    m_PIDisArmed=1;
    m_roll_prevError=0;
    m_roll_integrationError=0;
    m_pitch_prevError=0;
    m_pitch_integrationError=0;

}


void PIDController::disarmPID()
{

    m_PIDisArmed=0;
    m_roll_prevError=0;
    m_roll_integrationError=0;
    m_pitch_prevError=0;
    m_pitch_integrationError=0;

}




uint8_t PIDController::calculateValues() 
{

    // GET NRF24DATA
    float SP_nrf24_roll = this->m_Nrf24_1->getRoll();           // expected Value -100  <->  100
    float SP_nrf24_pitch = this->m_Nrf24_1->getPitch();         // expected Value -100  <->  100
    float SP_nrf24_yaw = this->m_Nrf24_1->getYaw();             // expected Value -100  <->  100
    float SP_nrf24_thrust = this->m_Nrf24_1->getThrust();       // expected Value    0  <->  100

    // MAP NRF24 DATA
    SP_nrf24_roll=SP_nrf24_roll/20;                         // -5° <-> 5°
    SP_nrf24_pitch=SP_nrf24_pitch/20;                       // -5° <-> 5°
    SP_nrf24_yaw = SP_nrf24_yaw;                            // -100  <->  100        
    //SP_nrf24_thrust=SP_nrf24_thrust/6.66 + 10;             //   0  <->  100    ->   10  ->   25
    SP_nrf24_thrust=SP_nrf24_thrust/8 + 12;             //   0  <->  100    ->   10  ->   25

    // GET COMP MEASUREMENT
    float MeasurePV_COMPFILTER_roll  = this->m_CompFilter_1->getRollHatDeg();         // expected Value -100  <->  100
    float MeasurePV_COMPFILTER_pitch = this->m_CompFilter_1->getPitchHatDeg();        // expected Value -100  <->  100


    if (m_PIDisArmed ==0)
    {
        if ((abs(MeasurePV_COMPFILTER_roll) > 1.5)||(abs(MeasurePV_COMPFILTER_pitch) > 1.5))
        {
            armPID();
        }

    }



    // PID FOR ROLL

    float error_roll = SP_nrf24_roll - MeasurePV_COMPFILTER_roll;
    m_roll_integrationError = m_roll_integrationError + error_roll;
    float derivative_roll = error_roll - m_roll_prevError;
    float output_roll = m_roll_kp * error_roll   +    m_roll_ki * m_roll_integrationError     +    m_roll_kd * derivative_roll;

    m_roll_prevError = error_roll;



    // PID FOR ROLL

    float error_pitch = SP_nrf24_pitch - MeasurePV_COMPFILTER_pitch;
    m_pitch_integrationError = m_pitch_integrationError + error_pitch;
    float derivative_pitch = error_pitch - m_pitch_prevError;
    float output_pitch = m_pitch_kp * error_pitch   +    m_pitch_ki * m_pitch_integrationError     +    m_pitch_kd * derivative_pitch;

    m_pitch_prevError=error_pitch;

    float M1 = 0;
    float M2 = 0;
    float M3 = 0;
    float M4 = 0;


    if (m_PIDisArmed == 0)
    {

        M1 = SP_nrf24_thrust;
        M2 = SP_nrf24_thrust;
        M3 = SP_nrf24_thrust;
        M4 = SP_nrf24_thrust;

    }

    else
    {
        /*
        M1 = SP_nrf24_thrust/6.66  + 10  +  output_pitch  -  output_roll;  
        M2 = SP_nrf24_thrust/6.66  + 10  +  output_pitch  +  output_roll;
        M3 = SP_nrf24_thrust/6.66  + 10  -  output_pitch  -  output_roll;
        M4 = SP_nrf24_thrust/6.66  + 10  -  output_pitch  +  output_roll;
        
        M1 = SP_nrf24_thrust  +  output_pitch;                 
        M2 = SP_nrf24_thrust  +  output_pitch  +  output_roll;
        M3 = SP_nrf24_thrust;  
        M4 = SP_nrf24_thrust                   +  output_roll;
        */



        M1 = SP_nrf24_thrust  +  output_pitch  -  output_roll;  
        M2 = SP_nrf24_thrust  +  output_pitch  +  output_roll;
        M3 = SP_nrf24_thrust  -  output_pitch  -  output_roll;
        M4 = SP_nrf24_thrust  -  output_pitch  +  output_roll;


        

    }

    Serial.println(M1);
    Serial.println(M2);
    Serial.println(M3);
    Serial.println(M4);

     

    if (M1 > 100) {M1 = 100;}
    if (M1< 0 ) 
    {
        M1 = 0;
        Serial.print("DEBUG1");
        m_motorhandler_1->disarmMotors();
        
        
        return 0;
    
    }



     if (M2 > 100) {M2 = 100;}
    if (M2< 0 ) 
    {
        M2 = 0;
        Serial.print("DEBUG2");
        m_motorhandler_1->disarmMotors();
        return 0;
    }

    if (M3 > 100) {M3 = 100;}
    if (M3< 0 ) 
    {
        M3 = 0;
        Serial.print("DEBUG3");
        m_motorhandler_1->disarmMotors();
        return 0;
    }

    if (M4 > 100) {M4 = 100;}
    if (M4< 0 ) 
    {
        M4 = 0;
        Serial.print("DEBUG4");
        m_motorhandler_1->disarmMotors();
        return 0;
    }



    
Serial.print("SP_nrf24_thrust,");
    Serial.print(SP_nrf24_thrust);
    Serial.print(",");


Serial.print("output_pitch,");
    Serial.print(output_pitch);
    Serial.print(",");

    Serial.print("output_roll,");
    Serial.print(output_roll);
    Serial.print(",");







    m_Motor_dshotValues[0]=m_motorhandler_1->getDshotLookup((int)(M1*10));
    m_Motor_dshotValues[1]=m_motorhandler_1->getDshotLookup((int)(M2*10));
    m_Motor_dshotValues[2]=m_motorhandler_1->getDshotLookup((int)(M3*10));
    m_Motor_dshotValues[3]=m_motorhandler_1->getDshotLookup((int)(M4*10));

    
    
    


    Serial.print("M1,");
    Serial.print(m_Motor_dshotValues[0]);
    Serial.print(",");

    Serial.print("M2,");
    Serial.print(m_Motor_dshotValues[1]);
    Serial.print(",");
    Serial.print("M3,");

    Serial.print(m_Motor_dshotValues[2]);
      Serial.print(",");
    Serial.print("M4,");

    Serial.print(m_Motor_dshotValues[3]);
    Serial.println(",");





    return 1;
       


        
    }