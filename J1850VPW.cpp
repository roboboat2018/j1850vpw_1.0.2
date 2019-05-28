#include"J1850VPW.h"
#include <Arduino.h>
unsigned long timer=0;
void j1850vpw:: wait_idle(void)    { 
  start_timer();
 while (read_timer() < RX_IFS_MIN/0.0625) {
    if (!is_active()){
        start_timer();  // wait if still receiving messages
    }
  }
  }
void j1850vpw::Cardone_j1850vpw_ouput(int output_pin, bool state){

if(state){
pinMode(out_pin,OUTPUT);
digitalWrite(output_pin,HIGH);

}
else{

digitalWrite(output_pin,LOW);//this order matter
pinMode(in_pin,INPUT);
}
}
void j1850vpw::init(int in_pin_, int out_pin_,bool display_) {
  out_pin = out_pin_;
  in_pin = in_pin_;
  _display=display_;
  
  pinMode(in_pin,INPUT);
  digitalWrite(in_pin,HIGH);
   
  TCCR1A=0; 
  TCCR1B=0;
  TCCR1B=1<<(CS10);
  passive();  
  if_init = true;
 Serial.begin(115200);
 Serial.println("starting j1850vpw....");
}
bool j1850vpw::is_active(void) {
  return digitalRead(in_pin);
}
void j1850vpw::start_timer() { 
  TCNT1=0;  

  }
ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
    } 
unsigned int j1850vpw::read_timer(void) { 
  return TCNT1; 
  }
void j1850vpw::active(void) {
  Cardone_j1850vpw_ouput(out_pin,HIGH);
}

void j1850vpw::passive(void) {
  Cardone_j1850vpw_ouput(out_pin,LOW);
}
bool j1850vpw::j1850vpw_send(int size, ...) {
  if (size == 0)
    return false;
  byte *buffer = new byte[size];
  bool result = false;
  va_list ap;
  va_start(ap, size);
  for (int i = 0; i < size; i++)
    buffer[i] = va_arg(ap, int);
  va_end(ap);
  result = send(buffer, size);
  delete[] buffer;
  return result;
}
bool j1850vpw::send(byte *msg_buf, int nbytes) {
  if (!if_init)
    return false;
  msg_buf[nbytes] = crc(msg_buf, nbytes);
  nbytes++;
  bool f = send_msg(msg_buf, nbytes);
  if(f){
    monitor();
  }
  return f;
}
bool j1850vpw::send_msg(byte *msg_buf, int nbytes) {
  int nbits;
  byte temp_byte;
  tx_msg_buf = msg_buf;
  tx_nbyte = nbytes;
  if (nbytes > 12) {
    message = ERROR_MESSAGE_TOO_LONG;
    return false;
  }
wait_idle();
active();
while(is_active());//wait untill input signal go Low
start_timer();
while(read_timer()<TX_SOF/0.0625);//should substract to compensate the delay
do
  {
    temp_byte = *msg_buf; // store byte temporary//took 4 clock
    nbits = 8;// took 4 clock
    while (nbits--)   // send 8 bits
    { 
      if(nbits & 1) // start allways with passive symbol
      { start_timer();
        delay = (temp_byte & 0x80) ? TX_LONG : TX_SHORT;  // send correct pulse lenght 
        passive();  // set bus passive  
    
        while (read_timer() <= delay/0.0625-46/0.0625)  // wait
        {
          if(!is_active())  // check for bus error
          {  message=RETURN_CODE_BUS_ERROR;
             return false ; // error, bus collision!
          }
        }
      }
      else  // send active symbol
      {
        start_timer();
        delay = (temp_byte & 0x80) ? TX_SHORT : TX_LONG;  // send correct pulse lenght
        active(); // set bus active
        while(is_active()); //wait untill input signal go low
        while (read_timer() <= delay/0.0625-12/0.0625); // affect the high pulse width
        // no error check needed, ACTIVE dominates
      }
      temp_byte <<= 1;  // next bit
    }// end nbits while loop
    ++msg_buf;  // next byte from buffer
  } while(--nbytes);// end nbytes do loop
   
  passive();  // send EOF symbol
   
  start_timer();
  while (read_timer() <= TX_EOF/0.0625); // wait for EOF complete
  message=MESSAGE_SEND_OK;
  return true;  // no error
}
bool j1850vpw::recv_msg(byte *msg_buf) {
  start_timer();
  int nbits, nbytes;
  bool bit_state;
  rx_msg_buf = msg_buf;
  start_timer();
  while (is_active()) {
    if (read_timer() > WAIT_100us/0.0625) {
      message = ERROR_NO_RESPONDS_WITHIN_100US;
      //Serial.println(read_timer()*0.0625);
      return false;
    }
  }
  start_timer();
  while (!is_active()) { 
    if (read_timer() > RX_SOF_MAX/0.0625) {
      message = ERROR_ON_SOF_TIMEOUT;
      return false;
    }
  }
  if (read_timer() < RX_SOF_MIN/0.0625) {
   
    message = ERROR_SYMBOL_WAS_NOT_SOF;
    return false;
  }
 // Serial.println(read_timer()*0.0625);
  bit_state = !is_active();
  start_timer();
  for (nbytes = 0; nbytes < 12; ++nbytes) {
    nbits = 8;
    do {
      *msg_buf <<= 1;
      while (!is_active() == bit_state) {
          rx_nbyte = nbytes;
          message = MESSAGE_ACCEPT_OK;
        if (read_timer() > RX_EOD_MIN/0.0625) {
          return true;
        }
      }
      bit_state = !is_active();  
      long tcnt1_buf = read_timer();
      start_timer();
      if (tcnt1_buf < RX_SHORT_MIN/0.0625) {
        message = ERROR_SYMBOL_WAS_TOO_SHORT;
        return false;
      }
      if ((tcnt1_buf < RX_SHORT_MAX/0.0625) && is_active())
        *msg_buf |= 1;

      if ((tcnt1_buf > RX_LONG_MIN/0.0625) && (tcnt1_buf < RX_LONG_MAX/0.0625) && !is_active())
        *msg_buf |= 1;

    } while (--nbits);
    ++msg_buf;
  }

  rx_nbyte = nbytes;
  message = MESSAGE_ACCEPT_OK;
  return true;
}
bool j1850vpw::accept(byte *msg_buf) {
  if (!if_init)
    return false;
  bool f = recv_msg(msg_buf);

  if (f) {
    if (msg_buf[rx_nbyte - 1] != crc(msg_buf, rx_nbyte - 1)) {
      f = false;
      message = ERROR_ACCEPT_CRC;
      
    }else{
      message=MESSAGE_ACCEPT_OK;
      monitor();
    }
    
  }
  //Serial.println(message);
  return f;
}
void j1850vpw::monitor(void) {
 if(_display==true){
  if (MESSAGE_SEND_OK == message)
        sendToUART("TX: ", tx_nbyte, tx_msg_buf);
  if (MESSAGE_ACCEPT_OK == message)
        sendToUART("RX: ", rx_nbyte, rx_msg_buf);
}
}
void j1850vpw::sendToUART(const char *header, int rx_nbyte, byte *msg_buf) {
  Serial.print(header);
  for (int i = 0; i < rx_nbyte-1; i++) {
    if (msg_buf[i] < 0x10)
      Serial.print(0);

    Serial.print(msg_buf[i], HEX);

    if (i == (rx_nbyte - 2)) {
      Serial.println();
    } else {
      Serial.print(" ");
    }
  }
}

byte j1850vpw::crc(byte *msg_buf, int nbytes) {
  byte crc = 0xFF;
  while (nbytes--) {
    crc ^= *msg_buf++;
    for (int i = 0; i < 8; i++)
      crc = crc & 0x80 ? (crc << 1) ^ 0x1D : crc << 1;
  }
  crc ^= 0xFF;
  return crc;
}
