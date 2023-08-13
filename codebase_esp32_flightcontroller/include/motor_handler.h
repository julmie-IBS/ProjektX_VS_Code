


#ifndef Motor_Handler_h
#define Motor_Handler_h


#include "arduino.h"



//KREINDL
#define GPIO_MOSFET ???
#define GPIO_ESC_1 ???
#define GPIO_ESC_2  ???
#define GPIO_ESC_3 ??????
#define GPIO_ESC_4 ??????



class motorhandler  {
public:

    motorhandler();

    void armEsc();
    void disarmEsc();
    void writeEsc(byte*);
 
};




#endif

