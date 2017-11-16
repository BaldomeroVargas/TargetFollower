#ifndef STATES_H
#define STATES_H

/*grid looks like
	x	x	x	x  > upper half (PINA3-PINA0)
	x	x	x	x  > upper half (PINA7-PINA4)
	
	x	x	x	x  > bottom half (PINC3-PINC0)
	x	x	x	x  > bottom half (PINC7-PINC4)
*/

/*LCD Output*/
const char * moveOut = "Mov: ";
const char * destOut = "Dest: ";
void lcdDestOut(Point P){
	LCD_ClearScreen();
	if(P.x < 0){
		LCD_DisplayString(1, moveOut);
		LCD_Cursor(6);
		LCD_WriteData('-');
		LCD_Cursor(7);
		LCD_WriteData(48 + (-1*P.x));
	}
	else{
		LCD_DisplayString(1, moveOut);
		LCD_Cursor(6);
		LCD_WriteData(48 + P.x);
	}
	if(P.y < 0){
		LCD_DisplayString(17, moveOut);
		LCD_Cursor(22);
		LCD_WriteData('-');
		LCD_Cursor(23);
		LCD_WriteData(48 + (-1*P.y));
	}
	else{
		LCD_DisplayString(17, moveOut);
		LCD_Cursor(22);
		LCD_WriteData(48 + P.y);
	}
	
	return;
}

void lcdLocOut(Point P){
	if(P.x < 0){
		LCD_DisplayString(9, destOut);
		LCD_Cursor(15);
		LCD_WriteData('-');
		LCD_Cursor(16);
		LCD_WriteData(48 + (-1*P.x));
	}
	else{
		LCD_DisplayString(9, destOut);
		LCD_Cursor(15);
		LCD_WriteData(48 + P.x);
	}
	if(P.y < 0){
		LCD_DisplayString(25, destOut);
		LCD_Cursor(31);
		LCD_WriteData('-');
		LCD_Cursor(32);
		LCD_WriteData(48 + (-1*P.y));
	}
	else{
		LCD_DisplayString(25, destOut);
		LCD_Cursor(31);
		LCD_WriteData(48 + P.y);
	}
	
	return;
}

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
const unsigned short SensorDist = 256; //find once board is created, just a place holder
signed short xMovement = 0;
signed short yMovement = 0;
unsigned short xTicks = 0;
unsigned short yTicks = 0;
unsigned char xRotation = 0;
unsigned char yRotation = 0;
/*Motor flags*/
unsigned char xFlag = 0;
unsigned char yFlag = 0;
unsigned char xComplete = 0;
unsigned char yComplete = 0;
/*Motor flags*/
	
/*queue size*/
Queue moves;
const unsigned char QSIZE = 30;
	
/*Motor Variables and functions for motor location*/
Point MotorLocation;
Point previous;
Point movement;
Point dest;
Point MotorInit(){
	Point Initial;
	Initial.x = 0;
	Initial.y = 0;
	
	return Initial;
}
Point UpdatePoint(signed char x, signed char y){
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
unsigned char isPointEqual(Point A, Point B){
	if(A.x != B.x){return 0;}
	if(A.y != B.y){return 0;}
	return 1;
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
enum EnQueue_SM{setqueue};
int EnQueue_SM(int state)
{
	//temp point variable
	Point temp;
		
	//transitions
	switch(state)
	{
		case -1:
		state = setqueue;
		break;
		
		case setqueue:
		state = setqueue;
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
		
		case setqueue:
		//output a sensor change
		bit = 0;
		
		if(topHalf != prevTop){		
			prevTop = topHalf;
			if(GetBit(topHalf, bit)){						
				temp = UpdatePoint(2,2);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(1,2);
				QueueEnqueue(moves, temp);				
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(0,2);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(-1,2);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(2,1);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(1,1);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(0,1);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(topHalf, bit)){
				temp = UpdatePoint(-1,1);
				QueueEnqueue(moves, temp);
			}
		}
		
		bit = 0;		
		
		if(bottomHalf != prevBot){
			prevBot = bottomHalf;
			//check which sensor was triggered
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(2,0);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(1,0);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(0,0);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(-1,0);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(2,-1);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(1,-1);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(0,-1);
				QueueEnqueue(moves, temp);
			}
			++bit;
			if(GetBit(bottomHalf, bit)){
				temp = UpdatePoint(-1,-1);
				QueueEnqueue(moves, temp);
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
		state = calculate;
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
			
			//waiting for flags to not be set anymore
			if(!xFlag && !yFlag){
				dest = QueueDequeue(moves);
				if(!isPointEqual(dest, previous)){
					previous = UpdatePoint(dest.x, dest.y);
					movement = DistanceToTravel(dest, MotorLocation);
					
					lcdDestOut(movement);
					
					if(movement.x != 0){
						xFlag = 1;
					}
					if(movement.y != 0){
						yFlag = 1;
					} 
				}				
			}
		}
		
		break;

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
		state = wait;
		break;
		
		case wait:
		if(xFlag){
			state = moveX; xComplete = 0; 
			xMovement = movement.x * SensorDist;
		}
		else if(yFlag){
			state = moveY; yComplete = 0; 
			yMovement = movement.y * SensorDist;
		}
		else{state = wait;}
		break;
		
		case moveX:
		if(xComplete){
			xFlag = 0; xComplete = 0; 
			if(yFlag){
				state = moveY; 
				yComplete = 0; 				
				yMovement = movement.y * SensorDist;
			}
			else{
				state = wait; 					
				//updating motor location	
				lcdLocOut(dest);
				MotorLocation = UpdatePoint(dest.x, dest.y);
			}
		}
		else{state = moveX;}
		break;
		
		case moveY:
		if(yComplete){
			yFlag = 0; yComplete = 0; 
			state = wait;
			//updating motor location
			lcdLocOut(dest);
			MotorLocation = UpdatePoint(dest.x, dest.y);	
		}
		else{state = moveY;}
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

		case wait:
		break;
		
		case moveX:
		//move in CW direction
		if(xMovement > 0){
			if(xTicks < xMovement){
				PORTA = CW[xRotation];
				++xRotation;
				if(xRotation >= 8){xRotation = 0;}
				++xTicks;
			}
			else{
				xRotation = 0;
				xTicks = 0;
				xFlag = 0;
				xComplete = 1;
				PORTA = 0;
			}
		}
		//move in CCW
		else{
			if(xTicks < (xMovement * -1)){
				PORTA = CCW[xRotation];
				++xRotation;
				if(xRotation >= 8){xRotation = 0;}
				++xTicks;
			}
			else{
				xRotation = 0;
				xTicks = 0;	
				xFlag = 0;
				xComplete = 1;
				PORTA = 0;
			}
		}
		break;
		
		case moveY:
		if(yMovement > 0){
			if(yTicks < yMovement){
				PORTB = CW[yRotation];
				++yRotation;
				if(yRotation >= 8){yRotation = 0;}
				++yTicks;
			}
			else{
				yRotation = 0;
				yTicks = 0;
				yFlag = 0;
				yComplete = 1;
				PORTB = 0;
			}
		}
		//move in CCW
		else{
			if(yTicks < (yMovement * -1)){
				PORTB = CCW[yRotation];
				++yRotation;
				if(yRotation >= 8){yRotation = 0;}
				++yTicks;
			}
			else{
				yRotation = 0;
				yTicks = 0;
				yFlag = 0;
				yComplete = 1;
				PORTB = 0;
			}
		}
		break;
		
		default:
		break;
	}
	return state;
}

#endif
