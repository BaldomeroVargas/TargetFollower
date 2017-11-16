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
	previous = MotorInit();
	
	/*initializing queue*/
	unsigned char queueSize = 30;
	moves = QueueInit(queueSize);
	
	//Task Scheduler
	unsigned char k = 0;
	tasksNum = 4;
	task tsks[4];
	tasks = tsks;
	
	tasks[k].state = -1;
	tasks[k].period = 20;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &RecieveData;
	
	++k;
	
	tasks[k].state = -1;
	tasks[k].period = 20;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &EnQueue_SM;
	
	++k;
	
	tasks[k].state = -1;
	tasks[k].period = 20;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &MotorLogic_SM;
	
	++k;
	
	tasks[k].state = -1;
	tasks[k].period = 2;
	tasks[k].elapsedTime = tasks[k].period;
	tasks[k].TickFct = &MotorMove_SM;
	
	
	TimerSet(2);
	TimerOn();
	
	while(1){}
}



