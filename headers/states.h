#ifndef STATES_H
#define STATES_H

/*grid looks like
	x	x	x	x  > upper half (PINA3-PINA0)
	x	x	x	x  > upper half (PINA7-PINA4)
	
	x	x	x	x  > bottom half (PINC3-PINC0)
	x	x	x	x  > bottom half (PINC7-PINC4)
*/
/*Sensor Logic*/
unsigned char topHalf = 0; 
unsigned char bottomHalf = 0;
unsigned char prevTop = 0;
unsigned char prevBot = 0;
//used to grab bit X
unsigned char bit = 0;
/*Sensor Logic*/

/*Motor Logic*/
const unsigned char CCW[] = {0x06, 0x02, 0x0A, 0x08, 0x09, 0x01, 0x05, 0x04};
const unsigned char CW[] = {0x04, 0x05, 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06};
const unsigned short SensorDist = 1000; //find once board is created, just a place holder
	
/*queue size*/
Queue moves;
const unsigned char QSIZE = 10;
	
/*Motor Variables for motor location*/
Point MotorLocation;
Point MotorInit(){
	Point Initial;
	Initial.x = 0;
	Initial.y = 0;
	
	return Initial;
}
Point UpdateMotorLocation(signed char x, signed char y){
	Point MotorReturn;
	MotorReturn.x = x;
	MotorReturn.y = y;
	
	return MotorReturn;
}
Point DistanceToTravel(Point A, Point B){
	Point Distance;
	Distance.x = A.x - B.x;
	Distance.y = A.y - B.y;
	
	return Distance;
}

/*Motor Logic*/

/*USART Data gathering*/
enum RecieveData{polling};
int RecieveData(int state)
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
	
		//upper sensors
		if(USART_HasReceived(0)){
			topHalf = USART_Receive(0);
		}	
		
		//lower sensors
		if(USART_HasReceived(1)){
			bottomHalf = USART_Receive(1);
		}		
	
		break;	
		
		default:
		break;
	}
	return state;
}

/*LCD debugging state machine: will just load queue*/
enum LCD_SM{outputSensor};
int LCD_SM(int state)
{
	//transitions
	switch(state)
	{
		case -1:
		state = outputSensor;
		break;
		
		case outputSensor:
		state = outputSensor;
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
		
		case outputSensor:
		//output a sensor change
		
		bit = 0;
		
		if(topHalf != prevTop){
			prevTop = topHalf;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('A');
			QueueEnqueue(moves, 'A');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('B');
			QueueEnqueue(moves, 'B');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('C');
			QueueEnqueue(moves, 'C');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('D');
			QueueEnqueue(moves, 'D');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('E');
			QueueEnqueue(moves, 'E');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('F');
			QueueEnqueue(moves, 'F');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('G');
			QueueEnqueue(moves, 'G');}
			++bit;
			if(GetBit(topHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('H');
			QueueEnqueue(moves, 'H');}
		}
		
		bit = 0;		
		
		if(bottomHalf != prevBot){
			prevBot = bottomHalf;
			//check which sensor was triggered
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); LCD_WriteData(1); 
				QueueEnqueue(moves, '1');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('2');
				QueueEnqueue(moves, '2');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('3');
				QueueEnqueue(moves, '3');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('4');
				QueueEnqueue(moves, '4');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('5');
				QueueEnqueue(moves, '5');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('6');
				QueueEnqueue(moves, '6');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('7');
				QueueEnqueue(moves, '7');}
			++bit;
			if(GetBit(bottomHalf, bit)){//LCD_Cursor(1); //LCD_WriteData('8');
				QueueEnqueue(moves, '8');}
		}
		break;
		
		default:
		break;
	}
	return state;
}

enum PrintQueue_SM{print};
int PrintQueue_SM(int state)
{
	//transitions
	switch(state)
	{
		case -1:
		state = print;
		break;
		
		case print:
		state = print;
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
		
		case print:		
		//print contents of queue
		LCD_ClearScreen();
		for(int i = 0; i < QSIZE; ++i){
			if(!QueueIsEmpty(moves)){
				LCD_Cursor(i + 1);				
				LCD_WriteData(QueueDequeue(moves));
			}
		}
		
		break;
		
		default:
		break;
	}
	return state;
}

/*Prepares Point for motor to move*/
enum MotorLogic_SM{calculate};
int MotorLogic_SM(int state)
{
	//transitions
	switch(state)
	{
		case -1:
		break;
		
		case calculate:
		state = calculate;
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

		case calculate:
		if(!QueueIsEmpty(moves)){
			//convert queue to accept point
			
			//update location here
			//MotorLocation = UpdateMotorLocation(someX, someY);
		}

		default:
		break;
	}
	return state;
}

/*Executes Motor Movement*/
enum MotorMove_SM{wait, moveX, moveY};
int MotorMove_SM(int state)
{
	//transitions
	switch(state)
	{
		case -1:
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

		default:
		break;
	}
	return state;
}


#endif
