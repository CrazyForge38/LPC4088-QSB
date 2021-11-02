#include"mbed.h" 

DigitalOut clk(p27);
DigitalOut rst(p28);
DigitalOut data(p30); 

DigitalIn row1(p12);
DigitalIn row2(p11);
DigitalIn row3(p10);
DigitalIn row4(p9);

DigitalOut col1(p8);
DigitalOut col2(p7);
DigitalOut col3(p6);
DigitalOut col4(p5);

int arr[17][8] = {  {1,1,0,0,0,0,0,0}, //0 
               {1,1,1,1,1,0,0,1}, //1 
               {1,0,1,0,0,1,0,0}, //2 
               {1,0,1,1,0,0,0,0}, //3 
               {1,0,0,1,1,0,0,1}, //4 
               {1,0,0,1,0,0,1,0}, //5 
               {1,0,0,0,0,0,1,0}, //6 
               {1,1,1,1,1,0,0,0}, //7 
               {1,0,0,0,0,0,0,0}, //8 
               {1,0,0,1,1,0,0,0}, //9 
               {1,0,0,0,1,0,0,0}, //A
               {1,0,0,0,0,0,1,1}, //B
               {1,1,0,0,0,1,1,0}, //C
               {1,0,1,0,0,0,0,1}, //d
               {1,0,0,0,0,1,1,0}, //E
               {1,0,0,0,1,1,1,0}, //F
							 {1,1,1,1,1,1,1,1}}; //' '
								

int readPad(int row1, int row2, int row3, int row4, int col1, int col2, int col3, int col4 )
{
	if (!row1)
	{
		if			(!col1)	return 1;
    else if (!col2) return 2;
    else if (!col3) return 3;
    else if (!col4) return 10;
	}
	else if (!row2)
	{
		if			(!col1)	return 4;
		else if (!col2) return 5;
		else if (!col3) return 6;
		else if (!col4) return 11;
	}
	else if (!row3)
	{
		if			(!col1)	return 7;
    else if (!col2) return 8;
    else if (!col3) return 9;
    else if (!col4) return 12;
  }
  else if (!row4)
	{
		if			(!col1)	return 0;
    else if (!col2) return 15;
    else if (!col3) return 14;
    else if (!col4) return 13;
	}
  else return 88;
}

void EdgeClock(){ 
wait(0.000001); 
clk = 1; 
wait(0.000001); 
clk=0; 

} 

void displayVal(int val1){
   for (int j = 0; j < 8 ; j++) {
       data = arr[val1][j]; 
       EdgeClock(); 
   } 
   wait(0.2);
}

void clearScreen(){
	for (int i = 0; i < 3; i++)
	{
		displayVal(16);
	}
}

void Calculate(int topL, int topR, int Operation, int botL, int botR){
	int output = 0;
	
	switch(Operation)
	{
	  case 10: output = ((topL*10) + (topR)) + ((botL*10) + (botR));
		case 11: output = ((topL*10) + (topR)) - ((botL*10) + (botR));
	  case 12: output = ((topL*10) + (topR)) && ((botL*10) + (botR));
		case 13: output = ((topL*10) + (topR)) || ((botL*10) + (botR));
		default: break;
	}
	
	displayVal(output % 10);
	wait(0.0001);
	displayVal(output % 100);
	
}

void CalculatorInput(int num){ //this is going to take in 1-2 inputs, wait for a function thats not e, wait for another 1-2 inputs and wait for e to output the math
	//this could be the main as well and the other is search for input
	static int first_Operand[2] = {};
	static int second_Operand[2] = {};
	static int Operation = 0;
	static int Equal_Operator = 0;
		
	static int index_Operand1 = 0;
	static int index_Operand2 = 0;
		
	static int state_of_Operation = 1;
	
	if(num == 15)
	{
		clearScreen();
	}
		
  while (true)
	{ 
		switch(state_of_Operation)
		{
			case 1:
				first_Operand[index_Operand1] = num;
			  index_Operand1++;
			  displayVal(num); 
			  state_of_Operation++;
				break;
			case 2:
				first_Operand[index_Operand1] = num;
			  displayVal(num);
			  state_of_Operation++;
				break;
			case 3:
				Operation = num;
			  clearScreen();
				state_of_Operation++;
				break;
			case 4:
				second_Operand[index_Operand2] = num;
			  index_Operand2++;
			  displayVal(num);
			  state_of_Operation++;
				break;
			case 5: 
				second_Operand[index_Operand2] = num;
			  displayVal(num);
			  state_of_Operation++;
			case 6: 
				clearScreen();
			  Calculate(first_Operand[0], first_Operand[1], Operation, second_Operand[0], second_Operand[1]);
			  state_of_Operation++;
				break;
			default: break;
		}
	}
}


int main(){ 
   rst = 0; 
   EdgeClock(); 
   rst = 1; 
   EdgeClock(); 
   displayVal(16);
   displayVal(16);
   displayVal(16);
   while(1){

       int val;
       col1=0;
       col2=1;
       col3=1;
       col4=1;
       val=readPad(row1, row2, row3, row4, col1, col2, col3, col4);
		 
        if (val!=88)
		{
			CalculatorInput(val);
    }
		wait(0.0001);

       col1=1;
       col2=0;
       col3=1;
       col4=1;
       val=readPad(row1, row2, row3, row4, col1, col2, col3, col4);

       if (val!=88){
           CalculatorInput(val);
       }
				wait(0.0001);

       col1=1;
       col2=1;
       col3=0;
       col4=1;
       val=readPad(row1, row2, row3, row4, col1, col2, col3, col4);

       if (val!=88){
           CalculatorInput(val);
       }
			 wait(0.0001);

       col1=1;
       col2=1;
       col3=1;
       col4=0;
       val=readPad(row1, row2, row3, row4, col1, col2, col3, col4);

       if (val!=88){
           CalculatorInput(val);
       }
		  wait(0.0001);


   }
}//end main
