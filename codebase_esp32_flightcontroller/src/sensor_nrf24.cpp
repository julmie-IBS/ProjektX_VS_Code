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

            m_PS3_lx=m_dataBuffer[0];
            m_PS3_ly=m_dataBuffer[1];
            m_PS3_rx=m_dataBuffer[2];
            m_PS3_ry=m_dataBuffer[3];
            m_PS3_buttons=m_dataBuffer[4];
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

float Nrf24::getThrust()
{
   if(m_PS3_ly<6)
        return 0.0;     
   else 
        return (m_PS3_ly-5)*(100.0/123.0); 
}

float Nrf24::getYaw()
{
   if(abs(m_PS3_lx)<6)
        return 0.0;     
   else
   { if(m_PS3_lx>0) return (m_PS3_lx-5)*(100.0/123.0);
     else           return (m_PS3_lx+5)*(100.0/123.0);
   }       
}
float Nrf24::getRoll()
{
   if(abs(m_PS3_rx)<6)
        return 0.0;     
   else
   { if(m_PS3_rx>0) return (m_PS3_rx-5)*(100.0/123.0);
     else           return (m_PS3_rx+5)*(100.0/123.0);
   }       
}

float Nrf24::getPitch()
{
   if(abs(m_PS3_ry)<6)
        return 0.0;     
   else
   { if(m_PS3_ry>0) return (m_PS3_ry-5)*(100.0/123.0)*(-1.0);
     else           return (m_PS3_ry+5)*(100.0/123.0)*(-1.0);
   }       
}