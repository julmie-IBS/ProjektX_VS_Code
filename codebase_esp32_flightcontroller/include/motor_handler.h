#ifndef Motor_Handler_h
#define Motor_Handler_h


#include "arduino.h"



#define WAIT_CONSTANT_ONE               54
#define WAIT_CONSTANT_TWO               56                  //57
#define WAIT_CONSTANT_THREE             44                  //43

#define GPIO_MOT_ONE                    32
#define CLR_MOT_ONE                     GPIO.out1_w1tc.val |= 1 //Pin32 2^(32-32)
#define SET_MOT_ONE                     GPIO.out1_w1ts.val |= 1

#define GPIO_MOT_TWO                    33
#define CLR_MOT_TWO                     GPIO.out1_w1tc.val |= 2 //Pin33 2^(33-32)
#define SET_MOT_TWO                     GPIO.out1_w1ts.val |= 2

#define GPIO_MOT_THREE                  26
#define CLR_MOT_THREE                   GPIO.out_w1tc |= 67108864 //Pin26 2^26
#define SET_MOT_THREE                   GPIO.out_w1ts |= 67108864

#define GPIO_MOT_FOUR                   27
#define CLR_MOT_FOUR                    GPIO.out_w1tc |= 134217728 //Pin27: 2^27
#define SET_MOT_FOUR                    GPIO.out_w1ts |= 134217728

#define MAXIMAL_MOTOR_VALUE             1000
#define MINIMAL_MOTOR_VALUE             80

#define MAXIMAL_MOTOR_ADJUSTMENT_RATE   100



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









class motorhandler  {
public:

    motorhandler();
    void writeToMotorsSave(volatile uint16_t* throttle);
    void armMotors();
    void disarmMotors();
    
    
    
    // min max check
 
private:
    void WAIT_1();
    void WAIT_2();
    void WAIT_3();
    uint16_t createPacket(uint16_t throttlee);
    //save last throttle values 
    int m_last_throttles_M1;
    int m_last_throttles_M2;
    int m_last_throttles_M3;
    int m_last_throttles_M4;
    int m_isArmed=0;


    void writeToMotors(volatile uint16_t* throttle);
   
    
    
};




#endif

