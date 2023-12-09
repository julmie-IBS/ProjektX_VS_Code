


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




void PIDController::calculateValues() 
{

    // GET NRF24DATA
    float SP_nrf24_roll = this->m_Nrf24_1->getRoll();           // expected Value -100  <->  100
    float SP_nrf24_pitch = this->m_Nrf24_1->getPitch();         // expected Value -100  <->  100
    float SP_nrf24_yaw = this->m_Nrf24_1->getYaw();             // expected Value -100  <->  100
    float SP_nrf24_thrust = this->m_Nrf24_1->getThrust();       // expected Value    0  <->  100

    // MAP NRF24 DATA
    SP_nrf24_roll=SP_nrf24_roll/10;                         // -10° <-> 10°
    SP_nrf24_pitch=SP_nrf24_pitch/10;                       // -10° <-> 10°
    SP_nrf24_yaw = SP_nrf24_yaw;                            // -100  <->  100        
    SP_nrf24_thrust=SP_nrf24_thrust;                        //   0  <->  100


    // GET COMP MEASUREMENT
    float MeasurePV_COMPFILTER_roll  = this->m_CompFilter_1->getRollHatDeg();         // expected Value -100  <->  100
    float MeasurePV_COMPFILTER_pitch = this->m_CompFilter_1->getPitchHatDeg();        // expected Value -100  <->  100



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

        M1 = SP_nrf24_thrust/6.66  + 10;
        M2 = SP_nrf24_thrust/6.66  + 10;
        M3 = SP_nrf24_thrust/6.66  + 10;
        M4 = SP_nrf24_thrust/6.66  + 10;

    }

    else
    {
        /*
        M1 = SP_nrf24_thrust/6.66  + 10  +  output_pitch  -  output_roll;  
        M2 = SP_nrf24_thrust/6.66  + 10  +  output_pitch  +  output_roll;
        M3 = SP_nrf24_thrust/6.66  + 10  -  output_pitch  -  output_roll;
        M4 = SP_nrf24_thrust/6.66  + 10  -  output_pitch  +  output_roll;
        */

        M1 = SP_nrf24_thrust/6.66  + 10  +  output_pitch;                 
        M2 = SP_nrf24_thrust/6.66  + 10  +  output_pitch  +  output_roll;
        M3 = SP_nrf24_thrust/6.66  + 10;  
        M4 = SP_nrf24_thrust/6.66  + 10                   +  output_roll;

    }
    



    
/*
            .
           / \    Front
            |
            |

       M2       M1
         \     /
          X---X
          X---X
          X---X
          X---X
         /     \
       M4       M3
*/


        




       

    /*

        double error = setpoint - processVariable;
        integral += error;
        double derivative = error - prevError;

        double output = kp * error + ki * integral + kd * derivative;

        // Clamping the output within the specified range
        

        prevError = error;

        // PITCH


        // MOTORMIXING

    */

        
    }