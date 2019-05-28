#include"J1850VPW.h"
//Timer1 is being used for the J1850VPW library
//change powerOn=flase;
j1850vpw j;
const int PIN_OUT = 9; 
const int PIN_IN=8;
const int Power=4;
const int Ignition=5; 
unsigned long counter1=0;
int counter2=0;
unsigned long counter3=0;
boolean unit_is_up_and_running=false;
boolean begin=true;
boolean powerOn=false;
boolean a=false,b=false,c=false,d=false,e=false;
boolean ignitionOn=false;
boolean ignitionOff;
String inString="";
boolean engOn=false;
boolean engOff=false;
boolean dtc=false;
boolean h_dtc=false;
boolean c_dtc=false;
boolean rlc=false;
boolean admc=false;
boolean smpm=false;
boolean bvr=false;
boolean torq=false;
boolean ivc=false;
boolean send_speed=false;
char inChar;
int speed;
int remainder;
void setup() {

pinMode(PIN_OUT, OUTPUT);
pinMode(PIN_IN,INPUT);
pinMode(Power, OUTPUT);
pinMode(Ignition,OUTPUT);
digitalWrite(Power,HIGH);
digitalWrite(Ignition,HIGH); 
pinMode(12,OUTPUT);
j.init(8, 9,1);
}
/*TIMSK0 |= B00000010;//bit 1 set to call the interrupt on an OCR1A match
  OCR1A=255;
}
ISR(TIMER0_COMPA_vect){ //Interrupt Service Routine, Timer/Counter1 Compare Match A
  counter1++;
  if(a){
  j.j1850vpw_send(06,0x88,0x29,0x10,0x02,0x00,0x00); 
  a=false;
 }
 else if(b){
  j.j1850vpw_send(04,0x88,0x53,0x10,0x04);
  b=false;
 }
 else if(c){
  j.j1850vpw_send(5,0xa8,0x35,0x40,0x31,0x03);
  c=false;
 }
 else if(d){
  j.j1850vpw_send(6,0xaa,0xeb,0x60,0x20,0xc9,0x00); 
  d=false;
 }
 else if(e){
  j.j1850vpw_send(5,0x28,0xff,0x40,0x06,0x07); 
  e=false;
 }
 ///////////////////////////////////////////
if(engOn==true){
    j.j1850vpw_send(4,0x88,0x53,0x10,0x84);
    engOn=false;
   }
   else if(engOff==true){
    j.j1850vpw_send(4,0x88,0x53,0x10,0x04);
    engOff=false;
   }
   else if(dtc==true){
    j.j1850vpw_send(7,0x6c,0x30,0xf1,0x19,0x02,0xff,0x00);
    dtc=false;
   }
   else if(h_dtc==true){
    j.j1850vpw_send(7,0x6c,0x30,0xf1,0x19,0x10,0xff,0x00);
    h_dtc=false;
   }
   else if(c_dtc==true){
    j.j1850vpw_send(4,0x6c,0x30,0xf1,0x14);
    c_dtc=false;
   }
   else if(rlc==true){
    j.j1850vpw_send(11,0x6c,0x30,0xf1,0x2A,0x01,0x0E,0xFF,0xFF,0xFF,0xFF,0xFF);
    rlc=false;
   }
   else if (admc==true){
    j.j1850vpw_send(11,0x6c,0x30,0xf1,0x2A,0x01,0x02,0xFF,0xFF,0xFF,0xFF,0xFF);
    admc=false;
   }
   else if(smpm==true){
    j.j1850vpw_send(11,0x6c,0x30,0xf1,0x2A,0x01,0x03,0xFF,0xFF,0xFF,0xFF,0xFF);
    smpm=false;
   }
   else if(bvr==true){
    j.j1850vpw_send(11,0x6c,0x30,0xf1,0x2A,0x01,0x04,0xFF,0xFF,0xFF,0xFF,0xFF);
    bvr=false;
   }
   else if(torq==true){
    j.j1850vpw_send(11,0x6c,0x30,0xf1,0x2A,0x01,0x01,0xFF,0xFF,0xFF,0xFF,0xFF);
    torq=false;
   }
   else if(ivc==true){
    j.j1850vpw_send(11,0x6c,0x30,0xf1,0x2A,0x01,0x05,0xFF,0xFF,0xFF,0xFF,0xFF);
    ivc=false;
   }
   else if(send_speed==true){
    j.j1850vpw_send(6,0x88,0x29,0x10,0x02,speed,0x7F*remainder);
    send_speed=false;
   }
   else if(ignitionOff){
   j.j1850vpw_send(4,0x28,0xff,0x40,0x06);
 
   j.j1850vpw_send(5,0x28,0xff,0x40,0x06,0x06);
   digitalWrite(Ignition,HIGH);
   ignitionOff=false;   
   }

 ////////////////////////////////////////////
  if(counter1==230){
    counter1=0;
 if(powerOn==true){
    j.j1850vpw_send(4,0x08,0xff,0x40,0x03);
    
    j.j1850vpw_send(4,0x08,0xff,0x60,0x03);
   
    j.j1850vpw_send(4,0x08,0xff,0x10,0x03);
   }
  } */

byte rx_buf[12];
void loop() {    
 if (j.accept(rx_buf)) { 
   if(rx_buf[0]==0xA9&&rx_buf[4]==0xE0){ 
         digitalWrite(12,1);
         delayMicroseconds(200);
         digitalWrite(12,0);
                                                                                
       }
      else if( rx_buf[0]==0xC9&&rx_buf[4]==0x37){ 
         digitalWrite(12,1);
         delayMicroseconds(350);
         digitalWrite(12,0);
       }
     else if( rx_buf[0]==0xC9&&rx_buf[4]==0x41){
         digitalWrite(12,1);
         delayMicroseconds(550);
         digitalWrite(12,0);
     }
     else if (rx_buf[0]==0x8A&&rx_buf[6]==0x64){
          digitalWrite(12,1);
         delayMicroseconds(750);
         digitalWrite(12,0);
         }
      else if( rx_buf[0]==0x49&&rx_buf[4]==0xc2){ 
         digitalWrite(12,1);
         delayMicroseconds(950);
         digitalWrite(12,0);
         } 
      else{
          receiveCommand();
         }
     
 }
}
void receiveCommand(){
  int i=0;
  inString="";
while(Serial.available()>0){
      inChar = Serial.read(); 
      if(inChar!='\n'&&inChar!=' '){
         inString+=(String)inChar;   
      }        
     }
      String temp1=inString;
      String temp2=inString;
      temp1.remove(6);
      temp2.remove(0,6);
      if(temp1=="speed:"){
      speed=temp2.toInt();
      remainder=speed%2;
      speed/=2;
      send_speed=true;    
      }
    if(send_speed==true){
    j.j1850vpw_send(6,0x88,0x29,0x10,0x02,speed,0x7F*remainder);
    send_speed=false;
   }
}
      
   
  
