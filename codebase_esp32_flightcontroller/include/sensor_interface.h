#ifndef Sensor_Interface_h
#define Sensor_Interface_h


#include "arduino.h"





class ISensor 
{
  public:
    
    
    // initialize sensor, set required parameters on sensor side eg: 
    virtual u_int16_t initSensor() = 0;

    // instructs the sensor to start a measurement
    virtual int triggerMeasurement() = 0;

    // fetch and store the data block into the object
    virtual int fetchData() = 0;

    // returns a byte pointer pointing at the beginning of the data block.
    virtual byte* getData() = 0;

    // returns the lenght of the data block.
    virtual int getLength() = 0;



};
  
#endif