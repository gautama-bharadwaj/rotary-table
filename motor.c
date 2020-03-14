/*
Program to control the rotation of the motor
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#define MOTOR_PIN_DIR		17
#define MOTOR_PIN_STEP		27
#define STR_CLOCKWISE		"clockwise"
#define STR_ANTICLOCKWISE	"anticlockwise"
#define DIR_CLOCKWISE		1
#define DIR_ANTICLOCKWISE   0


//Function to setup the I/O ports of the Raspberry Pi
int motor_gpio_init(void)
{
    if (wiringPiSetupGpio() < 0) {
	printf("error in gpio setup\n");
	return -1;
	}
   
    pinMode(MOTOR_PIN_DIR, OUTPUT);
    pinMode(MOTOR_PIN_STEP, OUTPUT);
    digitalWrite(MOTOR_PIN_STEP, LOW);
    printf("Gpio setup is done\n");
    return 0;
}

//Function to rotate the motor
void motor_rotate(int dir, int steps, int delay)
{
    int i;

    digitalWrite(MOTOR_PIN_DIR, dir);
	for (i = 0; i <= steps; i++) {
		digitalWrite(MOTOR_PIN_STEP, HIGH);
		delayMicroseconds(delay);
		digitalWrite(MOTOR_PIN_STEP, LOW);
		delayMicroseconds(delay);
	    }
}

//This accepts 3 arguments from the command promt. Dir(0 or 1), Steps, Delay(minimum 2500)
int main(int argc, char *argv[])
{   
    int dir;
    int steps;
    int delay;
    if(argc<4) {
        printf("No arguments entered\n");
        return 0;
    }
    if(argc>=4) {
        dir = atoi(argv[1]);
        steps = atoi(argv[2]);
        delay = atoi(argv[3]);

    }
    if (motor_gpio_init())
	    return 1;
    motor_rotate(dir, steps, delay);
	return 0;
}
