#ifndef UDP_H_
#define UDP_H_

#define SERVER_PORT 12345 //netcat -u 192.168.0.121 12345
#define MAX_SIZE 1024

//Changing the drum-beat mode directly (i.e., jumping from a standard rock beat to no beat)
//Changing the volume
//Changing the tempo
//Playing any one of the sounds your drum-beats use

char* displayResponse(char* respond_to_msg, char* prelimMsg);
char* displayError(char* respond_to_msg);

char* get_volume();
char* increase_volume();
char* decrease_volume();

char* get_tempo();
char* increase_tempo();
char* decrease_tempo();

char* get_beat();
char* next_beat();
char* previous_beat();
char* playBeat(int i);

void* playbackThread(void* arg);
 
char* verifyCommand(char* myMsg, int sock, struct sockaddr_storage serverAddr);
void* netcat();

#endif


