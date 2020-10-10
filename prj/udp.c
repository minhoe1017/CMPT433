#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "udp.h"

#define BUFFER_LENGTH 512
#define PORT 12345

static struct sockaddr_in si_me, si_other;
static int s, recv_len;
static unsigned int slen = sizeof(si_other);

void  UDP_initialize(void) {
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket could not be created\n");
        exit(1);
    }
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
        perror("socket could not be bound\n");
        exit(2);
    }
}

void  UDP_close(void) {
    close(s);
}
// Returns a null-terminated string of up to 512 characters. It is
// up to the calling function to call free.
char* UDP_read(int* size) {
//    while(!received) {
        printf("Waiting for data...");
        fflush(stdout);
        char buffer[BUFFER_LENGTH];
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buffer, BUFFER_LENGTH, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            perror("UDP receiving error\n");
            exit(3);
        }
        *size = recv_len;
        char* bufferCopy = malloc(BUFFER_LENGTH * sizeof(char));
        strcpy(bufferCopy, buffer);
        buffer[*size-1] = '\0';
        printf("%s\n", buffer);
        return bufferCopy; 
        //printf("Data: %s\n" , buffer);
//  }
}
void  UDP_write(char* buffer) {
    int length = strlen(buffer)+1;
    //printf("sending \"%s\" of length %d\n",buffer, length);
    if (sendto(s, buffer, length, 0, (struct sockaddr*) &si_other, slen) == -1) {
        perror("UDP sending error\n");
        exit(4);
    }
}

 



/*
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>         // for strncmp()
#include <unistd.h>         // for close()
#include <pthread.h>
#include <stdbool.h>

#define UDP_PORT 12345
#define MAX_RECEIVE_MESSAGE_LENGTH 1024
#define REPLY_BUFFER_SIZE (10*1024)
#define PRIMES_PER_LINE 4

#define COMMAND_HELP  "help"
#define COMMAND_COUNT "count"
#define COMMAND_GET   "get"
#define COMMAND_LAST  "last"
#define COMMAND_FIRST "first"
#define COMMAND_STOP  "stop"

static char replyBuffer[REPLY_BUFFER_SIZE];

// Header
void udpListeningThread(void);
*/
// void UdpListener_cleanup(void) {
//     stopping = true;
//     pthread_join(s_threadId, NULL);
// }

/*
 *  On the host:
 *      > netcat -u 192.168.0.171 12345
 *      (Change the IP address to your board)
 *
 *  On the host, type in a command and press enter:
 *      help<Enter>
 *
 */
/*


void udpListeningThread(void) {
    // Buffer to hold packet data:
    char message[MAX_RECEIVE_MESSAGE_LENGTH];

    // Address
    struct sockaddr_in sin;
    unsigned int sin_len;                       // May change.
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
    sin.sin_port = htons(UDP_PORT);             // Host to Network short

    // Create the socket for UDP
    int socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // Bind the socket to the port that we specify
    bind(socket_descriptor, (struct sockaddr*) &sin, sizeof(sin));

    while (1) {
        // Get the data (blocking)
        // Will change sin (the address) to be the address of the client.
        sin_len = sizeof(sin);
        int bytesRx = recvfrom(socket_descriptor, message, MAX_RECEIVE_MESSAGE_LENGTH, 0,
                (struct sockaddr *) &sin, &sin_len);

        // Make it null terminated (so string functions work):
        message[bytesRx] = 0;
        //printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, message);


        if (strncmp(message, "start", strlen("start")) == 0) {
            printf("WE GOT A MESSAGE!!!\n");
            replyBuffer[0] = 0;
            while (1) {
                sprintf(replyBuffer, "room1_pref_temp: %d\n", Devices_potentiometerRead());
                sin_len = sizeof(sin);
                sendto( socket_descriptor,
                    replyBuffer, strnlen(replyBuffer, REPLY_BUFFER_SIZE),
                    0,
                    (struct sockaddr *) &sin, sin_len);
                sprintf(replyBuffer, "room1_act_temp: %d\n", 25);
                //sin_len = sizeof(sin);
                sendto( socket_descriptor,
                    replyBuffer, strnlen(replyBuffer, REPLY_BUFFER_SIZE),
                    0,
                    (struct sockaddr *) &sin, sin_len);
                sleep(1);
            }
        }
    }

    // Close (signals to main that we are ending)
    close(socket_descriptor);
}
*/

