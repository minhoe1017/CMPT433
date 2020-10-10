#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "led.h"
#include "joystick.h"

int randomLedOn(){
	time_t t;
	LED* ledToTurn = (LED *) malloc(sizeof(LED));

	LED* led0 = (LED *) malloc(sizeof(LED));
	LED* led3 = (LED *) malloc(sizeof(LED));
	led0 -> number = 0;
	led3 -> number = 3;
	LEDoff(led0);
	LEDoff(led3);

	srand((unsigned) time(&t));
	int randNum = rand() % 2;
	if (randNum == 0){
		ledToTurn -> number = 0;
	}else {
		ledToTurn -> number = 3;
	}

	LEDon(ledToTurn);
	int retVal = ledToTurn -> number;
	free(led0);
	free(led3);
	free(ledToTurn);
	if (retVal == 0) return up;
	else return down;
}

int waitInput(){
	while(true){
		if (gpioRead(up)){
			return up;
		}else if(gpioRead(down)){
			return down;
		}else if(gpioRead(left)){
			return left;
		}else if(gpioRead(right)){
			return right;
		}
	}
	return 0;
}


int main(){
	printf("Hello embedded world, from Min Kim!\n\n");

	printf("Press the Zen cape's Joystick in the direction of the LED.\n\
		UP for LED 0 (top)\n\
		DOWN for LED 3 (bottom)\n\
		LEFT/RIGHT for exit app.\n");
	joystick_init();
	int score = 0;
	int tries = 0;

	do{
		printf("Press joystick; current score(%d / %d)\n", score, tries);
		int ledOn = randomLedOn();

		int userInput = waitInput();
		sleep(1);

		if (userInput == left || userInput == right){
			break;
		}else if (ledOn == userInput){
			score++;
			tries++;
			printf("Correct!\n");
			
			LED* led = (LED *) malloc(sizeof(LED));
			for (int i = 0; i < 4; i++){
				led -> number = i;
				LEDflash(led, 1);
			}
		}else {
			tries++;
			printf("Incorrect! :(\n");

			LED* led = (LED *) malloc(sizeof(LED));
			for (int count = 0; count < 5; count++){		
				for (int i = 0; i < 4; i++){
					led -> number = i;
					LEDflash(led, 5);
				}
			}
			
		}

	} while(true);

	printf("Your final score was (%d / %d)\nThank you for playing!\n", score, tries);

	return 0;
}