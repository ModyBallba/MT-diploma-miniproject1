
#include <stdio.h>
#define WITH_ENGINE_TEMP_CONTROLLER 1


char start(void);
void displaystate(void);
void turnon(void);
void turnoff(void);


char VeichleSpeed;
float enginetemperature;
char enginecontrflag; // engine tempreature controller flag will be one if not in range of temp 100
float roomtemperature;
char acflag;  // flag which control AC on or off during changing in temp
char engineflag; //engine flag which view if engine is workin or no note : it will work if any operation done
int operationCounter=0; //count operation done on system if more than one it will display all info about system

int main() {
	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);
	char input;
	do{
		displaystate();
		input = start();
		if(input == 'a')
		{
			turnon();
		}
		else if(input == 'b')
		{
			turnoff();
		}
	}while(input != 'c');



	if(input == 'c')
	{
		return 0;
	}



	return 0;
}





char start(void)
{
	char Input;
	do{
			printf("welcome to engine system \n");
			printf("choose ('a' or 'b' or 'c') where \n");
			printf("a: Turn on the vehicle engine\n");
			printf("b: Turn off the vehicle engine\n");
			printf("c: Quit the system\n");
			scanf(" %c",&Input);
	}while(Input != 'a' && Input != 'b' && Input != 'c');

	return Input;
}




void turnon(void){
	char inputon;
	char color;
	printf("The vehicle engine is turned on\n");
	printf("");
	do{
			printf("Sensors set menu:\n");
			printf("choose ('a' or 'b' or 'c' or 'd' or 'e') according for your choice\n");
			printf("a. Turn off the engine\n");
			printf("b. Set the traffic light color\n");
			printf("c. Set the room temperature (temperature sensor)\n");
#if WITH_ENGINE_TEMP_CONTROLLER
			printf("d. Set the engine temperature (engine temperature sensor)\n");
#endif
			scanf(" %c",&inputon);


			if(inputon == 'b')
			{
				operationCounter++;
				do{
					printf("please enter the letter ('G' or 'O' or 'R') based on the traffic light color\n");
					scanf(" %c",&color);
				}while(color != 'G' && color != 'O' && color != 'R');
				if(color=='G')
				{
					VeichleSpeed=100;
				}
				if(color=='O')
				{
					VeichleSpeed=30;
				}
				if(color=='R')
				{
					VeichleSpeed=0;
				}
				displaystate();

			}
			else if(inputon == 'd')
			{
				operationCounter++;
				printf("please enter the engine temperature value\n");
				scanf("%f",&enginetemperature);
#if WITH_ENGINE_TEMP_CONTROLLER
				if(enginetemperature<100 || enginetemperature>150)
				{
					enginetemperature=125;
					enginecontrflag=1;
				}


				else
				{
					enginecontrflag=0;
				}
#endif
				displaystate();

			}
			else if(inputon == 'c')
			{
				operationCounter++;
				printf("please enter the room temperature value\n");
				scanf("%f",&roomtemperature);
				if(roomtemperature<10 || roomtemperature>30)
				{
					acflag=1;
					roomtemperature=20;
				}

				else
				{
					acflag=0;
				}

			}



			if(VeichleSpeed==30)
			{
				acflag=1;
				roomtemperature=roomtemperature*(5/4)+1;
#if WITH_ENGINE_TEMP_CONTROLLER
				enginecontrflag=1;
				enginetemperature=enginetemperature*(5/4)+1;
#endif
			}

			if(operationCounter>=3 && inputon != 'a')
			{
				engineflag=1;
				displaystate();
			}

	}while(inputon != 'a');

	if(inputon=='a')
	{
		turnoff();
		engineflag=0;
		displaystate();
		return;
	}

}


void displaystate(void)
{
	if(operationCounter>=1)
	{
		if(engineflag==1)
		{
		printf("i. engine state:ON\n");
		}
		else if(engineflag==0)
		{
		printf("i. engine state:OFF\n");
		}
		if(acflag==1)
		{
		printf("ii. AC:ON\n");
		}
		else if(acflag==0)
		{
		printf("ii. AC:OFF\n");
		}
		printf("iii. vehicle speed:%d km/hr\n",VeichleSpeed);
		printf("iv. room temperature:%f\n",roomtemperature);
#if WITH_ENGINE_TEMP_CONTROLLER
		if(enginecontrflag==1)
		{
		printf("v. engine temperature controller state:ON\n");
		}
		else if(enginecontrflag==0)
		{
		printf("v. engine temperature controller state:OFF\n");
		}
		printf("vi. engine temperature:%f\n",enginetemperature);
#endif
	}
	return;
}


void turnoff(void){
	printf("The vehicle engine is turned off\n");
	return;
}
