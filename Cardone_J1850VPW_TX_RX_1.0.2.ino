#include"Cardone_j1850vpw_TX_RX_1.0.2.h"
//Timer1 is being used for the J1850VPW library
//change powerOn=flase;
j1850vpw j;
const int PIN_OUT = 9; 
const int PIN_IN=8;
const int Power=4;
const int Ignition=5; 
unsigned long counter1=0;
unsigned long timer=0;
int counter2=0;
boolean begin=true;
boolean powerOn=true;
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
bool transmittingInprogress=false;
char inChar;
int speed;
int remainder;
char inData[12];
byte index=0;
bool currentState=false;
unsigned long pulseCommand=0;
unsigned long timer_temp=0;
void setup() {
pinMode(3, INPUT);
pinMode(PIN_OUT, OUTPUT);
pinMode(PIN_IN,INPUT);
pinMode(Power, OUTPUT);
pinMode(Ignition,OUTPUT);
digitalWrite(Power,HIGH);
digitalWrite(Ignition,HIGH); 
j.init(8, 9,1);
delay(100);
TCCR2B |=(1<<CS20);
TIMSK2 |= (1<<OCIE2A);//bit 1 set to call the interrupt on an OCR1A match
  OCR2A=10;
}
ISR(TIMER2_COMPA_vect){ //Interrupt Service Routine, Timer/Counter1 Compare Match A
  counter1++;
  TCNT2=0;
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
else if(engOn==true){
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
  else if(powerOn==true){
    if(counter1==210){
        j.j1850vpw_send(4,0x08,0xff,0x40,0x03);
    }
    else if(counter1==220){
      j.j1850vpw_send(4,0x08,0xff,0x60,0x03);
    }
    else if(counter1==230){
    j.j1850vpw_send(4,0x08,0xff,0x10,0x03);
    }

   }
  
  if(counter1==270){
    counter1=0;
  }

}

bool isRequest(){
  return digitalRead(3);
}
///////////////////////////////////////////////////////////////////
void loop() { 
if(isRequest()&&currentState==false){//read pin command
 start_timer();
 currentState=true;
 
}
else if(currentState==true&&!isRequest()){
 currentState=false;
 pulseCommand=read_timer()+100;
 start_timer();
 //Serial.println(pulseCommand);
 receiveCommand();
 
}
} 
/////////////////////////////////////////////////////                     
void receiveCommand(){
      if(pulseCommand>=50&&pulseCommand<250){
        a=1;
      }
      else if(pulseCommand>=250&&pulseCommand<450){
        b=1;
      }
       else if(pulseCommand>=450&&pulseCommand<650){
        c=1;
      }
       else if(pulseCommand>=650&&pulseCommand<850){
        d=1;
      }
       else if(pulseCommand>=850&&pulseCommand<1050){
       e=1;
      }
      else if(pulseCommand>=1050&&pulseCommand<1250){
        //digitalWrite(Power,LOW);
        powerOn=true;
        }
      else if(pulseCommand>=1250&&pulseCommand<1450){
        //digitalWrite(Power,HIGH);
        powerOn=false;
      }
      else if(pulseCommand>=1450&&pulseCommand<1650){
        //digitalWrite(Ignition,LOW); 
        ignitionOn=true;
        //begin=true;
      }
      else if(pulseCommand>=1650&&pulseCommand<1850){
        //digitalWrite(Ignition,HIGH); 
        begin=false;
        ignitionOff=true;
      }
      else if(pulseCommand>=1850&&pulseCommand<2050){
         engOn=true;
      }
       else if(pulseCommand>=2050&&pulseCommand<2250){
         engOff=true;
      }
      else if(pulseCommand>=2250&&pulseCommand<2450){
        dtc=true;
      }
       else if(pulseCommand>=2450&&pulseCommand<2650){
        h_dtc=true;
      }
      else if(pulseCommand>=2650&&pulseCommand<2850){
        c_dtc=true;
      }
      else if(pulseCommand>=2850&&pulseCommand<3050){
      rlc=true;
      }
      else if(pulseCommand>=3050&&pulseCommand<3250){
      admc=true;
      }
      else if(pulseCommand>=3250&&pulseCommand<3450){
      smpm=true;
      }
      else if(pulseCommand>=3450&&pulseCommand<3650){
      bvr=true;
      }
      else if(pulseCommand>3650&&pulseCommand<3850){
      torq=true;
      }
      else if(pulseCommand>3850&&pulseCommand<4050){
      ivc=true;
      }
    pulseCommand=0;
}
      
     /* String temp1=inString;
      String temp2=inString;
      temp1.remove(6);
      temp2.remove(0,6);
      if(temp1=="speed:"){
        speed=temp2.toInt();
        remainder=speed%2;
        speed/=2;
        send_speed=true;
      
      }
      */
    
 void start_timer(){ 
   timer=micros();
 }
 unsigned long read_timer(){
  return micros()-timer;
 }

