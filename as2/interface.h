// interface.h
// module to take commands from netcat through UDP
// and return the appropriate information
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//#include "udp.h"
//#include "potentiometer.h"
//#include "sorter.h"

// Local function to allocate and permute array.
static void Interface_help (void);
static void Interface_count (void);
static void Interface_getLength (void);
static void Interface_getArray (void);	//WRITE THIS!!!!!!!!
static void Interface_get (int index);	//WRITE THIS!!!!!!!!
static void Interface_stop (void);
static void Interface_error (void);
// Read the command coming in through UDP, which
// will respond with the appropriate message.
void Interface_run (void);

extern void  UDP_close(void);
extern char* UDP_read(int* size);
extern void  UDP_write(char* c);

extern void Sorter_stopSorting (void);
extern void Sorter_setArraySize (int newSize);
extern int Sorter_getArrayLength (void);
extern int* Sorter_getArrayData (int *length);
extern long long Sorter_getNumberArraysSorted (void);

#endif