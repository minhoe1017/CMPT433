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
    printf("printing string of length %d\n", length);
    if (sendto(s, buffer, length, 0, (struct sockaddr*) &si_other, slen) == -1) {
        perror("UDP sending error\n");
        exit(4);
    }
}

 
//int main(void) {
/*    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFFER_LENGTH];
     
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
     
    //keep listening for data
    while(1) {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buffer, BUFFER_LENGTH, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            perror("UDP receiving error\n");
            exit(3);
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buffer);
         
        //now reply the client with the same data
        if (sendto(s, buffer, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) {
            perror("UDP sending error\n");
            exit(4);
        }
    }
*/
//  UDP_initialize();
//  int size;
//  char* test = UDP_read(&size);
//  UDP_write(test);
//  UDP_close();
//}