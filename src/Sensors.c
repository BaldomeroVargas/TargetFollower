/*
 * FinalProject.c
 *
 * Created: 11/11/2017 5:36:16 PM
 * Author : Baldo
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "usart_ATmega1284.h"
#include "scheduler.h"

/*grid looks like
	x	x	x	x  > upper half (PINA3-PINA0)
	x	x	x	x  > upper half (PINA7-PINA4)
	
	x	x	x	x  > bottom half (PINC3-PINC0)
	x	x	x	x  > bottom half (PINC7-PINC4)
*/
unsigned char topHalf = 0;
unsigned char bottomHalf = 0;

enum Input{polling};

int Input(int state)
{
	//transitions
	switch(state)
	{
		case -1:
		state = polling;
		break;
		
		case polling:
		state = polling;
		break;
		
		default:
		state = -1;
		break;
	}
	
	//actions
	switch(state)
	{

		case -1:
		break;
		
		case polling:
		
		//Grabbing sensor data
		
		topHalf = ~PINA; 	
		bottomHalf = ~PINC;
		break;
		
		default:
		break;
	}
	return state;
}

enum Send{dataSend};

int Send(int state)
{
	//transitions
	switch(state)
	{
		case -1:
		state = dataSend;
		break;
		
		case dataSend:
		state = dataSend;
		break;
		
		default:
		state = -1;
		break;
	}
	
	//actions
	switch(state)
	{

		case -1:
		break;
		
		case dataSend:
		
		//sending top half
		if (USART_IsSendReady(0) && topHalf != 0){USART_Send(topHalf, 0);}
		
		//sending bottom half
		if (USART_IsSendReady(1) && bottomHalf != 0){USART_Send(bottomHalf, 1);}		
			
		default:
		break;
	}
	return state;
}

int main(void)
{
	//inputs
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;

	initUSART(0);
	initUSART(1);
	
	//Task Scheduler
	unsigned char k = 0;
	tasksNum = 2;
	task tsks[2];
	tasks = tsks;
	
	tasks[k].state = -1;
	tasks[k].period = 50;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &Input;
	
	++k;
	
	tasks[k].state = -1;
	tasks[k].period = 60;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &Send;
	
	TimerSet(10);
	TimerOn();
	
	while(1){}
	
}



