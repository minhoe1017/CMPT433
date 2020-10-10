#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "audioMixer.h"
#include "beatGen.h"
#include "joystick.h"

wavedata_t basedrum;
wavedata_t hihat;
wavedata_t snare;
pthread_t beatThreadId;

static int tempo = DEFAULT_BPM;
//static int beat = 2;

//char beatArr[3];

long halfBeat(int bpm) {
	return (long) ((60 / bpm / 2) * 100000000);
}

void beatInit(){
	AudioMixer_readWaveFileIntoMemory(BASEDRUM, &basedrum);
	AudioMixer_readWaveFileIntoMemory(SNARE, &snare);
	AudioMixer_readWaveFileIntoMemory(HIHAT, &hihat);
	//pthread_create(&beatThreadId, NULL, beatThread, NULL);
	//beatArr[0] = basedrum;
	//beatArr[1] = snare;
	//beatArr[2] = hihat;
}

//beat turned off
void noDrumBeat(){
	tempo = AudioMixer_getBPM();

	for (int i = 0; i < 8; i++)
		nsleep(0, halfBeat(tempo));
}

void standardRockDrumBeat(){
	tempo = AudioMixer_getBPM();

	for (int i = 0; i < 8; i++){
		if (i % 2 == 0){
			if (i == 0 || i == 4)
				AudioMixer_queueSound(&basedrum);
			else if (i == 2 || i == 6)
				AudioMixer_queueSound(&snare);
		}
		else
			AudioMixer_queueSound(&hihat);
		nsleep(0, halfBeat(tempo));
	}
}

//“Four on the Floor” from https://takelessons.com/blog/easy-drum-beats-z07
void otherDrumBeat(){
	tempo = AudioMixer_getBPM();

	for (int i = 0; i < 8; i++){
		if (i == 0 || i == 1)
			AudioMixer_queueSound(&snare);
		else{
			AudioMixer_queueSound(&hihat);
			AudioMixer_queueSound(&basedrum);
		}
		nsleep(0, halfBeat(tempo));
	}
}

void* beatThread(void* arg){
	beatInit();

	while(1) {
		if (AudioMixer_getState() == 0){
			noDrumBeat();
		}else if (AudioMixer_getState() == 1){
			standardRockDrumBeat();
		}else if (AudioMixer_getState() == 2){
			otherDrumBeat();
		}
	}
	pthread_exit(0);
}

void beatFree() {
	AudioMixer_freeWaveFileData(&snare);
	AudioMixer_freeWaveFileData(&hihat);
	AudioMixer_freeWaveFileData(&basedrum);
	pthread_join(beatThreadId, NULL);
}