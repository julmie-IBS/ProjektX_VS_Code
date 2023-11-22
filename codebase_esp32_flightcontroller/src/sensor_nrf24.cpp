#include "sensor_nrf24.h"


Nrf24::Nrf24()
{

    const uint8_t m_rf24_address[10] = "ADDRESS01";     //unclear why this cannot be a class member
    this->m_radio = RF24(5,4);
    this->m_radio.begin();
    this->m_radio.openReadingPipe(0, m_rf24_address);
    this->m_radio.setDataRate( RF24_250KBPS );
    this->m_radio.setPALevel(RF24_PA_MIN);
    this->m_radio.startListening();
}




u_int16_t Nrf24::initSensor() 
{
       
    return 0;
}

int Nrf24::triggerMeasurement()
{
    return 0;
}


int Nrf24::fetchData()
{
    
    if (m_radio.available()) 
        {
            m_radio.read(m_dataBuffer, 5);
        }     
    return 0;
    
}


byte* Nrf24::getData()
{
    return m_dataBuffer;
}


int Nrf24::getLength()
{
    return this->m_length;
}

