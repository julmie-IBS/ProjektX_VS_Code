

#ifndef PID_h
#define PID_h

#include <complementary_filter.h>
#include <sensor_nrf24.h>
#include <motor_handler.h>
#include <complementary_filter.h>



class PIDController {
public:
    PIDController(CompFilter* CompFilter_1, Nrf24* Nrf24_1, motorhandler* motorhandler_1);
    void calculateValues();
    void armPID();
    void disarmPID();

private:

    CompFilter* m_CompFilter_1;
    Nrf24* m_Nrf24_1;
    motorhandler* m_motorhandler_1;

    int m_PIDisArmed=0;

    float m_roll_kp=0.01;
    float m_roll_ki=0.01;
    float m_roll_kd;
    float m_roll_prevError=0;
    float m_roll_integrationError=0;



    float m_pitch_kp=0.01;
    float m_pitch_ki=0.01;
    float m_pitch_kd;
    float m_pitch_prevError=0;
    float m_pitch_integrationError=0;



};

#endif

