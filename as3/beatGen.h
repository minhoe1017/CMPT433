#ifndef BEATGEN_H
#define BEATGEN_H

#define BASEDRUM "beatbox-wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT "beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define DEFAULT_BPM 120

void beatInit();
long halfBeat(int bpm);

void noDrumBeat();
void standardRockDrumBeat();
void otherDrumBeat();

void* beatThread(void* arg);
void beatFree();

#endif