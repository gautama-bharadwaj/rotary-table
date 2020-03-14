/* 
Server program that controls rotation of stepper motor depending on instructions by client program
*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<strings.h>
#include	<sys/socket.h>
#include	<sys/types.h>
#include	<netinet/in.h>
#include    <wiringPi.h>
#define MOTOR_PIN_DIR       17
#define MOTOR_PIN_STEP      27
#define STR_CLOCKWISE       "clockwise"
#define STR_ANTICLOCKWISE   "anticlockwise"
#define DIR_CLOCKWISE       1
#define DIR_ANTICLOCKWISE   0
#define MOTOR_DEFAULT_SPEED 10  
//PORT is fixed at 8090
#define PORT                8090

//Function to setup the I/O ports of Raspberry Pi
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
void motor_rotate(int dir, int steps)
{
    int i;

    digitalWrite(MOTOR_PIN_DIR, dir);
    for (i = 0; i < steps; i++) {
        digitalWrite(MOTOR_PIN_STEP, HIGH);
        //delay can be modified here. The minimum delay should be 2500
        delayMicroseconds(2500);
        digitalWrite(MOTOR_PIN_STEP, LOW);
        delayMicroseconds(2500);
    }
}

int main(int argc, char *argv[])
{
    int sockfd, newsock, portno;
    struct sockaddr_in sa, ca;
    socklen_t casize;
    int n;
    char buffer[256];
    char clock[20] = "Clockwise";
    char anticlock[20] = "Anticlockwise";
    char error[20] = "Enter 1 or 2";
    if (motor_gpio_init())
        return 1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
	   perror("Error building socket\n");
	   return 1;
    }
    bzero(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
	   perror("Bind failure\n");
	   return 1;
    }

    if (listen(sockfd, 5)) {
	   perror("Error in listen\n");
	   return 1;
    }
    casize = sizeof(ca);
    newsock = accept(sockfd, (struct sockaddr *)&ca, &casize);

    if (newsock < 0) {
	   perror("Accept failed\n");
	   return 1;
    }

    while (1) {
    	n = read(newsock, buffer, sizeof(buffer)-1);
    	if (n < 0) {
    	    perror("Read error\n");
    	    close(newsock);
    	}
        int comp = atoi(buffer);
        if(comp==1)
        {
            printf("Rotating clockwise\n");
            write(newsock, clock, n);
            //The number of steps to be rotated can be modified by changing the 2nd argument in the motor_rotate() function call below.
            motor_rotate(0, 25);
        }
        else if(comp==2)
        {
            printf("Rotating anticlockwise\n");
            write(newsock, anticlock, n);
            //The number of steps to be rotated can be modified by changing the 2nd argument in the motor_rotate() function call below.
            motor_rotate(1, 25);
        }
    	else
        {
            write(newsock,error,n);
        }
        bzero(buffer, sizeof(buffer));
	}  
    return 0;
}
