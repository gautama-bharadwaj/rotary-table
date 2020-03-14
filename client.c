/*
 Client program for TCP with Raspberry Pi.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define FALSE       0
#define TRUE        1
#define SIZE        500
//PORT is fixed as 8090
#define PORT        8090

int main(int argc, char *argv[])
{
    struct sockaddr_in obj, obj1;
    int sfd;
    ssize_t nr;
    char buf[SIZE];

    sfd = socket(AF_INET, SOCK_STREAM, FALSE);
    if (sfd ==  -1) {
	    perror("socket");
	fprintf(stderr, "Failed to open\n");
    }
    printf("socket is created\n");
    memset(&obj, '\0', sizeof(obj));
    obj.sin_family = AF_INET;
    obj.sin_port = htons(PORT);
    //The IP address of the Raspberry Pi should be entered below
    obj.sin_addr.s_addr = inet_addr("192.168.43.226");

    if (connect(sfd, (struct sockaddr *) &obj, sizeof(obj)) ==  -1) {
	    perror("connect");
	    return 0;
    }

    if (&obj == NULL)
	    fprintf(stderr, "Could not connect\n");
    else
	    printf("Connected\n");

    while (1) {
	    printf("Enter 1 for clockwise and 2 for anticlockwise rotation :\n");
	if (fgets(buf, SIZE, stdin) == NULL) {
	    printf("fgets failed\n");
	    return 0;
	    }
	if (write(sfd, buf, SIZE) ==  -1)
	    fprintf(stderr, "Failed to write\n");

	nr = read(sfd, buf, SIZE);
	if (nr ==  -1)
		    perror("read");
	    else
		    printf("The rotation is : %s\n", buf);
    }
	close(sfd);
	return 0;
}
