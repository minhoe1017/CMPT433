#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>

#include <stdbool.h>
#include <ctype.h>
#include <sys/uio.h>

#include "audioMixer.h"
#include "beatGen.h"
#include "joystick.h"
#include "udp.h"

char* beatNames[3] = {"none", "std", "other" }; 

char quit = ' ';

void die(char *msg){
	perror(msg);
	exit(1);
}

char* checkCmd(char* buff, int s, struct sockaddr_in serverAddr){
	char* msgRes = (char*)malloc(sizeof(char)*MAX_SIZE);

	char buffer[MAX_SIZE];
	char temp[MAX_SIZE];
	int offset = 5;
	int getCount = -1;

	sscanf(buff, "%s %d", getNum, &getCount);


	if(strcmp(buff, "help\n") == 0){
		memset(msgRes, 0, MAX_SIZE);
			strcat(msgRes, "Accepted command examples:\n\
	next beat\t-- Switch to the next beat.\n\
	prev beat\t-- Switch to the previous beat.\n\
	volU\t-- Increase the volume.\n\
	volD\t-- Decrease the volume.\n\
	tempoU\t-- Increase the tempo.\n\
	tempoD\t-- Decrease the tempo.\n\
	trigger beat1\t-- Trigger the playback of each of the sounds.\n\
	stop\t-- cause the server program to end.\n");
			return msgRes;
		}else if(strcmp(buff, "nextbeat\n") == 0){
			memset(msgRes, 0, MAX_SIZE);

			AudioMixer_nextState();
			int currBeat = AudioMixer_getBeat();

			strcat(msgRes, "Switching to the next beat: ");
			sprintf(temp, beatNames[currBeat]);
			strcat(msgRes, temp);
			return msgRes;
		}else if(strcmp(buff, "prevbeat\n") == 0){
			memset(msgRes, 0, MAX_SIZE);

			AudioMixer_prevState();
			int currBeat = AudioMixer_getBeat();

			strcat(msgRes, "Switching to the previous beat: ");
			sprintf(temp, beatNames[currBeat]);
			strcat(msgRes, temp);
			return msgRes;
		}else if(strcmp(buff, "volU\n") == 0){
			memset(msgRes, 0, MAX_SIZE);
			AudioMixer_setVolume(AudioMixer_getVolume() + offset);
			strcat(msgRes, "Current Volume: ");
			sprintf(temp, "%d\n", AudioMixer_getVolume());
			strcat(msgRes, temp);
			return msgRes;
		}else if(strcmp(buff, "volD\n") == 0){
			memset(msgRes, 0, MAX_SIZE);
			AudioMixer_setVolume(AudioMixer_getVolume() - offset);
			strcat(msgRes, "Current Volume: ");
			sprintf(temp, "%d\n", AudioMixer_getVolume());
			strcat(msgRes, temp);
			return msgRes;
		}else if(strcmp(buff, "tempoU\n") == 0){
			memset(msgRes, 0, MAX_SIZE);
			AudioMixer_setBPM(AudioMixer_getBPM() + offset);
			strcat(msgRes, "Current Tempo: ");
			sprintf(temp, "%d\n", AudioMixer_getBPM());
			strcat(msgRes, temp);
			return msgRes;
		}else if(strcmp(buff, "tempoD\n") == 0){
			memset(msgRes, 0, MAX_SIZE);
			AudioMixer_setBPM(AudioMixer_getBPM() - offset);
			strcat(msgRes, "Current Tempo: ");
			sprintf(temp, "%d\n", AudioMixer_getBPM());
			strcat(msgRes, temp);
			return msgRes
		}else if(strcmp(getNum, "trigger beat") == 0){
			memset(msgRes, 0, MAX_SIZE);

			nsleep(0, 100000000);
			AudioMixer_queueSound(&beatArr[getCount]);
			nsleep(0, 100000000);

			strcat(msgRes, "Playing beat ");
			sprintf(temp, "%d\n", getCount);
			strcat(msgRes, temp);
			return msgRes;
			else{
				memset(msgRes, 0, MAX_SIZE);
				strcat(msgRes, "Value ");
				sprintf(temp, "%d = %d\n", getCount, globalArr[getCount-1]);
				strcat(msgRes, temp);
				return msgRes;
			}
		}else if(strcmp(buff, "trigger beat1\n") == 0){
			int currBeat = AudioMixer_getBeat();

			nsleep(0, 100000000);
			AudioMixer_queueSound(AudioMixer_getBeat());
			nsleep(0, 100000000);

			memset(msgRes, 0, MAX_SIZE);

			strcat(msgRes, "Current  array length = ");
			sprintf(temp, "%d\n", arrLength);
			strcat(msgRes, temp);
			return msgRes;
		}else if(strcmp(buff, "stop\n") == 0){
			memset(msgRes, 0, MAX_SIZE);
			strcat(msgRes, "Program Terminating\n");
			initDisplay();
			return msgRes;
		}else{
			memset(msgRes, 0, MAX_SIZE);
			sprintf(msgRes, "ERROR: Invalid command. Try help.\n");
			return msgRes;
		}
}

void* netcat(){ 
	//struct sockaddr_storage serverAddr;
	char myMsg[1024];

	struct sockaddr_in serverAddr;
	
	char buff[MAX_SIZE];

	memset((char*) &serverAddr, 0, sizeof(serverAddr));

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(SERVER_PORT);
	socklen_t serverSize;
	serverSize = sizeof(serverAddr);

	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if (bind(s, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))
		die("ERROR: binding failed");

  	while(1){
		int i = recvfrom(s, buff, MAX_SIZE, 0, (struct sockaddr *)&serverAddr, &serverSize);
		//(strcmp(buff, "stop") != 0))
		buff[i] = '\0';

		char* msgRes = checkCmd(buff, s, serverAddr); 

		if(strcmp(msgRes, "Program Terminating\n") == 0){
			quit = 'Q';
			sendto(s, msgRes, MAX_SIZE, 0, (struct sockaddr *) &serverAddr, serverSize);
			free(msgRes);
			break;
		}else if (strcmp(msgRes, " /n") == 0){
			free(msgRes);
		}else{
			sendto(s, msgRes, MAX_SIZE, 0, (struct sockaddr *) &serverAddr, serverSize);
			free(msgRes);
		}
	}

	close(s);
	pthread_exit(0);
}