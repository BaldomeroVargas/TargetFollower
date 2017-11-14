/* 
	Baldomero Vargas
	*CS 122A Custom Lab Project
	*Target Follower
*/

/*libs used*/
#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "scheduler.h"
#include "point.h"
#include "queue.h"
//////////////////////////
#include "shiftreg.h"
#include "lcd.h"
#include "bit.h"
//////////////////////////
#include "states.h"

int main(void)
{
	//motor outputs
	//x axis motor
	DDRA = 0xFF; PORTA = 0x00;
	//y axis motor
	DDRB = 0xFF; PORTB = 0x00;
	//lcd for testing purposes
	DDRC = 0xFF; PORTC = 0x00;

	/*usart inits*/
	LCD_init();
	initUSART(0);
	initUSART(1);
	
	/*motor inits*/
	MotorLocation = MotorInit();
	
	/*initializing queue*/
	unsigned char queueSize = 10;
	moves = QueueInit(queueSize);
	
	//Task Scheduler
	unsigned char k = 0;
	tasksNum = 3;
	task tsks[3];
	tasks = tsks;
	
	tasks[k].state = -1;
	tasks[k].period = 10;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &RecieveData;
	
	++k;
	
	tasks[k].state = -1;
	tasks[k].period = 10;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &LCD_SM;
	
	++k;
	
	tasks[k].state = -1;
	tasks[k].period = 2000;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &PrintQueue_SM;
	
	TimerSet(10);
	TimerOn();
	
	while(1){}
}



