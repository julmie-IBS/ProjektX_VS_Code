#ifndef Sensor_Nrf24_h
#define Sensor_Nrf24_h

#include "sensor_interface.h"
#include "arduino.h"
#include <RF24.h>
#include <nRF24L01.h>


/* #define CONFIGURATION_REGISTER_A 0x00
#define CONFIGURATION_REGISTER_B 0x01
#define IDENTIFICATION_REGISTER_A 0x0A
#define MODE_REGISTER 0x02
#define SINGLE_MEASUREMENT_MODE  0x01
#define DATA_OUTPUT_X_MSB_REGISTER 0x03 */


class Nrf24 : public ISensor {
public:
    Nrf24();
    u_int16_t initSensor() override;        
    int triggerMeasurement() override;
    int fetchData() override;
    byte* getData() override;
    int getLength() override;

    float getThrust();
    float getYaw();
    float getPitch();
    float getRoll();

    byte m_PS3_buttons=0;

private:

    RF24 m_radio ;
    int m_length = 6;
    byte m_dataBuffer[6];

    int8_t m_PS3_lx=0;
    int8_t m_PS3_ly=0;
    int8_t m_PS3_rx=0;
    int8_t m_PS3_ry=0;
    int8_t m_PS3_lastHeartbeatValue=0;
    int8_t m_PS3_badHeartbeatCount=0;

    


};


#endif