#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
int speed;
int time=0;
int count=0;
int count_timer=0;
int count2=1;
void Timer1_PhaseCorrectPWM_init(){
	TCNT1=0;
	DDRD|=(1<<PD5);//setting OC1A as a output pin(PD5)
	TCCR1A|=(1<<WGM10)|(1<<COM1A1);//PWM phase correct 8-bit
	TCCR1B|=(1<<CS10);//no pre-scaler
}

void IO_PINS(){
	DDRD|=(1<<PD0)|(1<<PD1);//fan output pin
	DDRC=0xFF;//7-segment output pins
	DDRB&=~(1<<PB7);//speed 1 push button input pin
	DDRB&=~(1<<PB6);//speed 2 push button input pin
	DDRB&=~(1<<PB5);//speed 3 push button input pin
	DDRB&=~(1<<PB4);//timer push button input pin
	DDRB&=~(1<<PB3);//start/stop push button input pin
	PORTD|=(1<<PD0);//clockwise rotation
	PORTD&=~(1<<PD1);//clockwise rotation
}
void Timer0_CTC_init(){
	TCNT0=0;
	TCCR0|=(1<<FOC0)|(1<<WGM01)|(1<<CS02)|(1<<CS00);//CTC mode with 1024 pre-scaler
	OCR0=250;//compare value
	TIMSK=(1<<OCIE0);
}
void timer_count(){
	if(count==14400&&time==1){
		OCR1A=0;
		speed=0;
	}else if(count==28800&&time==2){
		OCR1A=0;
		speed=0;
	}else if(count==57600&&time==3){
		OCR1A=0;
		speed=0;
	}
}
ISR(TIMER0_COMP_vect){
	count_timer++;
	timer_count();
}
void speed_disp(){
	if(PINB&(1<<PB7)){
			speed=1;
			OCR1A=64;
		}else if(PINB&(1<<PB6)){
			speed=2;
			OCR1A=128;
		}else if(PINB&(1<<PB5)){
			speed=3;
			OCR1A=255;
		}
	}

void display(){
	PORTC|=(1<<PC4);
	PORTC=(PORTC&0xF0)|(speed&0x0F);
	_delay_ms(2);
	PORTC&=~(1<<PC4);

	PORTC|=(1<<PC5);
	PORTC=(PORTC&0xF0)|(time&0x0F);
	_delay_ms(2);
	PORTC&=~(1<<PC5);
}


void timer_disp(){
	if(PINB&(1<<PB4)){
		while(PINB&(1<<PB4)){
			if(count==0){
				time=0;
				display();
			}else if(count==1){
				time=1;
				display();
			}else if(count==2){
				time=2;
				display();
			}else if(count==3){
				time=3;
				display();
			}else if(count==4){
				count=0;
				display();
	}
	}
	count++;
	}
	}

void start_stop(){//press button to start, hold button to turn off
	if(PINB&(1<<PB3)) {

	if(count2%2==0) {
		speed=1;
		OCR1A=64;
		display();
		while (count2%2==0){
			if(PINB&(1<<PB3)){
				count2++;
			}
			speed_disp();
			timer_disp();
			display();
		}
	}
	count2++;
	speed=0;
	OCR1A=0;
	display();

	}
}
int main(){
	SREG|=(1<<7);
	IO_PINS();
	Timer1_PhaseCorrectPWM_init();

	while(1){
		start_stop();
		//timer_disp();


	}
}



