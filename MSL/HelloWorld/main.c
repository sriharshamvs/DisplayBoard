#include <MPCx58AP.h>
#include <string.h>

#define	LOW		0
#define HIGH	1
#define RXBUFFSIZE	60

void delay(unsigned int);
void TIMER_INIT(void);
void SERIAL_INIT(void);
void SERIAL_TRANSMIT(unsigned char tx_char);
void SERIAL_STRING_TX(unsigned char *tx_String);
unsigned char SERIAL_RX(void);

unsigned char temp_char=0;
unsigned char RECIEVEbuffer[RXBUFFSIZE+1];

static unsigned char rxBytes=0;

void main(void){
	unsigned int i=0;
	P0 = LOW;
	P1 = LOW;
	P2 = LOW;
	TIMER_INIT();
	SERIAL_INIT();
	SERIAL_STRING_TX("Serial Port Initialized");

	while(1){
		;;
	}
}

void delay(unsigned int delayValue){
	unsigned int i;
	for(i=0;i<=delayValue;i++);
}

void SERIAL_INIT(void){
	SCON=0x50;
	PCON |=0X80;
	TMOD&=0xCF; 
	TMOD|=0x20; 
	TH1=0xFF;   // setting buadrate to 115200 bps
	TL1=0x00;
	TR1=1;
	ES = 1;
	IE &= 0x90;  
} // SERIAL_INIT ends here

void TIMER_INIT(void){
	TMOD=0x00;
	IE=0x00;
	TR0=0;
	TF0=0;
	TMOD|=0x01;
	IE|=0x82;
	TH0=0xF8;   // setting 1ms timer
	TL0=0xCD;
	TR0=1;
} // TIMER_INIT ends here

void SERIAL_STRING_TX(unsigned char *tx_String){   // For sending a String
	unsigned int i,stringSize;
	
	stringSize = strlen(tx_String); // Calculate the String length
	for(i=0;i<stringSize;i++){
		temp_char = tx_String[i];  
		SERIAL_TRANSMIT(temp_char);  // Send each character to the SERIAL_TRANSMIT function
	}
}

void SERIAL_TRANSMIT(unsigned char tx_char){  // For sending each character
	SBUF=tx_char;  //Store the incoming character in SUBF
	while(TI==0);  // Wait until Tx bit is High
	TI=0;   // Make it 0 once it's high
}

unsigned char SERIAL_RX(void){
	while(RI==0);
	RI=0;
	return SBUF;
}

void SERIAL_RECIEVE(void) interrupt 4{
	unsigned value;
	P0_6 =~ P0_6;
	if(RI){
		value = SBUF;
		RECIEVEbuffer[rxBytes++]=value;
		SBUF = value;
		RI = 0;
	}
}