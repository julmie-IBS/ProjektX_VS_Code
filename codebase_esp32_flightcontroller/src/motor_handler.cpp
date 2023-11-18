

#include "motor_handler.h"



motorhandler::motorhandler()
{
    pinMode(GPIO_MOT_ONE,OUTPUT);   //32 33 26 27
    pinMode(GPIO_MOT_TWO,OUTPUT);
    pinMode(GPIO_MOT_THREE,OUTPUT);
    pinMode(GPIO_MOT_FOUR,OUTPUT);
    CLR_MOT_ONE;
    CLR_MOT_TWO; 
    CLR_MOT_THREE; 
    CLR_MOT_FOUR;    
}




void motorhandler::WAIT_1()
{
   //2.5 us
   for(int ii=0;ii<WAIT_CONSTANT_ONE;ii++)
   {
     
      asm("nop\n\t");
      asm("nop\n\t");
      asm("nop\n\t");
   }
}
void motorhandler::WAIT_2()
{
   //2.5 us
   for(int ii=0;ii<WAIT_CONSTANT_TWO;ii++)
   {
      
      asm("nop\n\t");
      asm("nop\n\t");
      asm("nop\n\t");
   }
}
void motorhandler::WAIT_3()
{
   //1.67 us
   for(int ii=0;ii<WAIT_CONSTANT_THREE;ii++)
   {
      
      asm("nop\n\t");
      asm("nop\n\t");
      asm("nop\n\t");
   }
}

uint16_t motorhandler::createPacket(uint16_t throttlee)
{
    uint16_t  value=0;
    byte crc=0;
    value = throttlee << 1;
    value = value & (~0x0001);   //Tel 0
    //value = value | (0x0001); //Tel 1
    crc = (value ^ (value >> 4) ^ (value >> 8)) & 0x0F;
    value =value << 4;
    value = value & (~0x000F);
    value= value | crc;
    return value;  
}



void motorhandler::writeToMotors(volatile uint16_t* throttle)
{
    uint16_t SendStream1=createPacket(throttle[0]);
    uint16_t SendStream2=createPacket(throttle[1]);
    uint16_t SendStream3=createPacket(throttle[2]);
    uint16_t SendStream4=createPacket(throttle[3]);
    uint16_t testArray[] ={1 ,2 ,4 ,8 ,16 ,32 ,64 ,128 ,256 ,512 ,1024 ,2048,4096 ,8192 ,16384 ,32768};
    
    //delayMicroseconds(104);
    noInterrupts();  
    for(int i=15;i>=0;i--)
    { 

    SET_MOT_ONE;
    SET_MOT_TWO;
    SET_MOT_THREE;
    SET_MOT_FOUR;
    
    WAIT_1(); //2.5us, mark for zeros to go down

    
    
    if ((SendStream1 & testArray[i]) == 0)  
    {
        CLR_MOT_ONE; 
    }
    else
    {
        SET_MOT_ONE;
    }



    if ((SendStream2 & testArray[i]) == 0)  
    {
        CLR_MOT_TWO; 
    }   
    else
    {
        SET_MOT_TWO; 
    }


    if ((SendStream3 & testArray[i]) == 0)  
    {
        CLR_MOT_THREE;  
    }
    else
    {
        SET_MOT_THREE;
    }


    if ((SendStream4 & testArray[i]) == 0) 
    {
        CLR_MOT_FOUR; 
    }
    else
    {
        SET_MOT_FOUR;
    }



    WAIT_2(); //2.5us //mark for ones to go down
    CLR_MOT_ONE;
    CLR_MOT_TWO; 
    CLR_MOT_THREE; 
    CLR_MOT_FOUR;    

    WAIT_3(); //1.67 //downtime before next bit   

    }//for
    interrupts();
    //delayMicroseconds(104);
}


