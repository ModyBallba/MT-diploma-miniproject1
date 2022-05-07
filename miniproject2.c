/*
 * name : mahmoud abdellatif basiony
 * facebook : Mody Ballba
 * diploma 64
 * 	MT-Miniproject2
 *  stop watch
 *  finishing date : 5/5/2022
 */



/* **********************************includes******************************************* */
#include<avr/io.h>
#include"util/delay.h"
#include<avr/interrupt.h>
//some global variables for the INT

unsigned char SecCounter1=0;
unsigned char SecCounter2=0;
unsigned char MinCounter1=0;
unsigned char MinCounter2=0;
unsigned char HoursCounter1=0;
unsigned char HoursCounter2=0;

unsigned char TimerCounterFlag=0;
unsigned char Resume=0;
unsigned char Pause=0;

void timer1_init_CTC()
{
/*Starting value =0
top value =15625-1 giving 1 sec using prescaler 64
*/
	TCNT1=0;
	OCR1A=15624;
/*according to calculation to get 1 sec using 64 prescaler put OCR1A=15624
 but actually it gives more than one second compared to another real stop watch
	*/
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A) | (1<<FOC1B); // Non PWM
	TCCR1B|=(1<<WGM12)|(1 << CS11)|(1 << CS10);//ctc mode

}
/* here all interrupt lines are small code */

ISR(TIMER1_COMPA_vect)
{

	TimerCounterFlag=1;
}

ISR(INT0_vect){

//this interrupts  puts all variables with zero to reset these variables

SecCounter1=0;
SecCounter2=0;
MinCounter1=0;
MinCounter2=0;
HoursCounter1=0;
HoursCounter2=0;

}
// interrupts  1 for pause counting
ISR(INT1_vect){
	Pause=1;
	/*
	 this line for closing timer and opening it again down ,
	 comment this line if you dont need to turn off timer it will also work (a)
	  */
	TCCR1B &=(~(1 << CS11))& (~(1 << CS10));
}
// interrupts  2 for resume counting
ISR(INT2_vect){
	Resume=1;
}

void INT0_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	PORTD |= 0x04;                      //internal pull up resistor
	MCUCR |= (1<<ISC01);      			// Trigger INT0 with the falling edge
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

void INT1_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD3));               // Configure INT1/PD3 as input pin
	MCUCR |= (1<<ISC10)|(1<<ISC11);		// Trigger INT1 with the rising edge
	GICR  |= (1<<INT1);                 // Enable external interrupt pin INT1
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

void INT2_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRB  &= (~(1<<PB2));               // Configure INT2/PB2 as input pin
	PORTB |= 0x04; 						//internal pull up resistor
	MCUCSR &= (~(1<<ISC2));             // Trigger INT2 with the falling edge
	GICR  |= (1<<INT2);                 // Enable external interrupt pin INT2
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

int main(){
	DDRA |= 0x3F; //portA_output first 6 pins
	DDRC |= 0x0F;//portC_output  first 4 pins

	PORTA = 0x3F;    //control 7 segment with the 6 pins of port A
	PORTC&= 0xF0;	//Closing first four pins on port

	SREG|=(1<<7);

	INT0_Init();
	INT1_Init();
	INT2_Init();
	timer1_init_CTC();
	while(1){
if(TimerCounterFlag==1){
	SecCounter1++;
		if(SecCounter1==10){
			SecCounter1=0;
			SecCounter2++;
		}

		if(SecCounter2==6){
			SecCounter2=0;
			MinCounter1++;
		}

		if(MinCounter1==10){
			MinCounter1=0;
			MinCounter2++;

		}
		if(MinCounter2==6){
			MinCounter2=0;
			HoursCounter1++;
		}
		if(HoursCounter1==10){
			HoursCounter1=0;
			HoursCounter2++;

		}
		//reset if it gets its max 99:59:59
		if((HoursCounter2==9) &&( HoursCounter1=9) && (MinCounter2==5) &&( MinCounter1==9 )&&( SecCounter2==5 )&&( SecCounter1==10))
		{
			SecCounter1=0;
			SecCounter2=0;
			MinCounter1=0;
			MinCounter2=0;
			HoursCounter1=0;
			HoursCounter2=0;
		}
		TimerCounterFlag=0;
}
else{
			PORTA=0x01;
		    PORTC=SecCounter1;
		    _delay_us(100);
		    PORTA=0x02;
		    PORTC=SecCounter2;
		    _delay_us(100);
		    PORTA=0x04;
		    PORTC=MinCounter1;
		    _delay_us(100);
		   	PORTA=0x08;
		   	PORTC=MinCounter2;
		   	_delay_us(100);
		   	PORTA=0x10;
		   	PORTC=HoursCounter1;
		   	_delay_us(100);
		   	PORTA=0x20;
		   	PORTC=HoursCounter2;
		   	_delay_us(100);

		   	//while pause still put same values on 7 segments
		   	while(Pause==1){

		   		PORTA=0x01;
		   		PORTC=SecCounter1;
		   		_delay_us(100);
		   		PORTA=0x02;
		   		PORTC=SecCounter2;
		   		_delay_us(100);
		   		PORTA=0x04;
		   		PORTC=MinCounter1;
		   		_delay_us(100);
		   		PORTA=0x08;
		   		PORTC=MinCounter2;
		   		_delay_us(100);
		   		PORTA=0x10;
		   		PORTC=HoursCounter1;
		   		_delay_us(100);
		   		PORTA=0x20;
		   		PORTC=HoursCounter2;
		   		_delay_us(100);

		   		if(Resume==1){
		   			Pause=0;
		   			Resume=0;
		   			TCCR1B|=(1 << CS11)|(1 << CS10); //this enables timer again (b)
		   				   	}

		   	}



	}
}


}
