#ifndef J1850
#define J1850
#include <Arduino.h>
#define WAIT_100us 100
#define TX_SHORT 64
#define TX_LONG 128
#define TX_SOF 200
#define TX_EOF 380
#define RX_SHORT_MIN 34
#define RX_SHORT_MAX 96
#define RX_LONG_MIN 96
#define RX_LONG_MAX 163
#define RX_SOF_MIN 163
#define RX_SOF_MAX 239
#define RX_EOD_MIN 163
#define RX_IFS_MIN 280

#define MESSAGE_SEND_OK 1
#define MESSAGE_ACCEPT_OK 2
#define ERROR_MESSAGE_TOO_LONG 3
#define ERROR_NO_RESPONDS_WITHIN_100US 4
#define ERROR_ON_SOF_TIMEOUT 5
#define ERROR_SYMBOL_WAS_NOT_SOF 6
#define ERROR_SYMBOL_WAS_TOO_SHORT 7
#define ERROR_ACCEPT_CRC 8
#define RETURN_CODE_BUS_ERROR 9
class j1850vpw {

private:
unsigned long timer0=0;
void monitor(void);
void sendToUART(const char *header, int rx_nbyte, byte *msg_buf);

bool recv_msg(uint8_t *msg_buf );
byte *rx_msg_buf;
byte *tx_msg_buf;
int message;
int rx_nbyte = 0;
int tx_nbyte = 0;
unsigned int read_timer(void);
bool is_active(void);
void start_timer(void);
unsigned long timer_begin =0;
void Cardone_j1850vpw_ouput(int output_pin, bool state);
bool send(byte *msg_buf, int nbytes);
byte crc(byte *msg_buf, int nbytes);
bool send_msg(byte *, int);
void active(void);
void passive(void);
int in_pin = 0;
int out_pin = 0;
bool if_init = false;
void  wait_idle(void);
uint16_t delay;

bool _display=false;
bool j1850vpw::is_active_RX(void);
public:
bool accept(byte *msg_buf);
void init(int, int,bool );
bool j1850vpw_send(int size, ...);
};
#endif
