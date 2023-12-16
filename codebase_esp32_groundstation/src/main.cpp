#include <Ps3Controller.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>







void setup() {

  

  //Serial.begin(115200);

}



void loop() {


RF24 radio(5,21); // CE, CSN
const byte address[10] = "ADDRESS01";

int8_t PS3_ly=0;

Ps3.begin("34:86:5d:fc:51:da");
  delay(100);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_MIN);
  //radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();



    u_int8_t globalCount=0;

    while (1)
    {
        /* code */
    
    

  byte PS3_lx=Ps3.data.analog.stick.lx;
  byte PS3_ly=Ps3.data.analog.stick.ly;

  byte PS3_rx=Ps3.data.analog.stick.rx;
  byte PS3_ry=Ps3.data.analog.stick.ry;

  byte PS3_buttons = 0;
  PS3_buttons |= Ps3.data.button.triangle << 0;  // triangle button
  PS3_buttons |= Ps3.data.button.square << 1;  // 
  PS3_buttons |= Ps3.data.button.circle << 2;  // 
  PS3_buttons |= Ps3.data.button.cross << 3;  // 
  PS3_buttons |= Ps3.data.button.l1 << 4;  // L1 button
  PS3_buttons |= Ps3.data.button.r1 << 5;  // R1 button
  PS3_buttons |= Ps3.data.button.left << 6;// left button
  PS3_buttons |= Ps3.data.button.right << 7;// rigth button

  //void* ptr=malloc(5);
  //if (ptr != nullptr) {
  //byte* payload = static_cast<byte*>(ptr);

 // Serial.println(sizeof(payload));
  byte payload[6];
  
  payload[0]=PS3_lx;
  payload[1]=PS3_ly;
  payload[2]=PS3_rx;
  payload[3]=PS3_ry;
  //memcpy(payload[4], &PS3_buttons, sizeof(PS3_buttons));
  payload[4]=PS3_buttons;
 


  if (Ps3.isConnected())
  {
    payload[5]=globalCount % 2;
  }
  else
  {
    payload[5]=0;
  }
  
  
  //int8_t payload[sizeof(PS3_ly)];
  //memcpy(payload, &PS3_ly, sizeof(PS3_ly));
  
  radio.write((void*)payload, 6 );

 // Serial.println(); Serial.println(payload[0]);Serial.println(payload[1]);Serial.println(payload[2]);Serial.println(payload[3]);Serial.println(payload[4]);

  
   //Serial.println(sizeof(payload));
  //}
  //free(ptr);
  //ptr = NULL;
  delay(5);
  //delay(500);

  globalCount++;
  }

}