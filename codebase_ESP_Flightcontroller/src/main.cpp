#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>


#define CLR GPIO.out1_w1tc.val = 1 //Pin32
#define SET GPIO.out1_w1ts.val = 1

//#define CLR GPIO.out_w1tc = 4 //Pin2
//#define SET GPIO.out_w1ts = 4

#define MinimumThrottle 63 //>61
#define MaximumThrottle 500

RF24 radio(5, 4); // CE, CSN
const byte address[10] = "ADDRESS01";

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int8_t throttle_buffer=0;


static inline uint16_t createPacket(uint16_t throttlee){
  uint8_t csum = 0;
  throttlee <<= 1;
  // Indicate as command if less than 48
  if (throttlee <48)
    throttlee |= 1;
  uint16_t csum_data = throttlee;
  for (byte i=0; i<3; i++){
    csum ^= csum_data;
    csum_data >>= 4;
  }
  csum &= 0xf;
  return (throttlee<<4)|csum;
}


void sendZero(){
//2.5 + 4.17    =6.67
   SET;
   for(int ii=0;ii<60;ii++){ //45
__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");//5
    }
   CLR;
   for(int ii=0;ii<86;ii++){ //68 //86 //87
__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");//5
    }
  // SET;
}
 
void sendOne(){
   //5 + 1.67    =6.67
   SET;
   for(int ii=0;ii<119;ii++){ //85 //118 //120
__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");//5
    }
   CLR;
   for(int ii=0;ii<32;ii++){ //25 //31 //33
__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");__asm__("nop\n\t");//5
    }
  // SET;
}

void do_it() {

    // int ScaleFactor=round((MaximumThrottle-MinimumThrottle)/128);
    int ScaleFactor=round((MaximumThrottle)/128);
     int analogSum=0;
     uint16_t throttle= MinimumThrottle;
     uint16_t tlm;
     uint16_t CRC;
     int i=0;
     //int ii=0;
     
     uint16_t SendStream=0;
     uint16_t testArray[] ={1 ,2 ,4 ,8 ,16 ,32 ,64 ,128 ,256 ,512 ,1024 ,2048,4096 ,8192 ,16384 ,32768};


    // throttle_buffer=abs(Ps3.data.analog.stick.ly);

  if (radio.available()) {
    //char txt[32] = "";
    //radio.read(&throttle_buffer, sizeof(throttle_buffer));
    //Serial.println(throttle_buffer);
    //delay(1000);
         byte payload[5];
     radio.read(payload, 5);
     int8_t PS3_lx=payload[0];
     int8_t PS3_ly=payload[1];
     int8_t PS3_rx=payload[2];
     int8_t PS3_ry=payload[3];
     byte PS3_buttons=payload[4];

     throttle_buffer=abs(PS3_lx);
    
  }//radio.printDetails();
     
   //throttle_buffer=0;
/*   if(throttle_buffer<round(MinimumThrottle/ScaleFactor)){
    throttle=MinimumThrottle;
   }
   else{
    throttle= throttle_buffer*ScaleFactor;//+MinimumThrottle;
   }
*/
   throttle= throttle_buffer*ScaleFactor+MinimumThrottle;
   if(throttle<MinimumThrottle) throttle=MinimumThrottle;
   if(throttle>MaximumThrottle) throttle=MaximumThrottle;
   
  Serial.println(throttle);

/*
  //Serial.println(analogRead(34), DEC);
  for(int iii=0;iii<1000;iii++){
    analogSum=analogSum+analogRead(35);
  }
  Serial.print(analogSum, DEC);
  analogSum=0;
  Serial.print(",");  

 */
   //Serial.println(((throttle_buffer)));
   //Serial.println(((throttle)));
  // Serial.println(throttle, DEC);
   //throttle=75;
   SendStream=createPacket(throttle);

   noInterrupts();
   for(i=15;i>=0;i--)
   { //Serial.println(((i)));
     //Serial.println(((SendStream)),BIN);
     //Serial.println(testArray[i],BIN);
     //Serial.println(((SendStream) & testArray[i]),BIN);
     if (((SendStream) & testArray[i]) == 0)
      { sendZero(); 
      }
     else
      { sendOne();
      }
   }//for
   interrupts();

}

/*
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  do_it();
  portEXIT_CRITICAL_ISR(&timerMux);
}
*/

void setup() {
 // SPI.begin(18,19,23,5);
 pinMode(32,OUTPUT);
 // pinMode(2,OUTPUT);

 /*
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10100, true);
  timerAlarmEnable(timer);
  */
 
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //radio.printDetails();

}
///*
void loop() {

do_it();
delay(10);


// MJ add stuff
//I added yours
// kdkdkd
//jksdhfkjdsf

  // changes by github
  // add some test


}
