// Initializes a UDP connection at port 12345 and closes when done.
// This module will only send read and writes of strings.
#ifndef _UDP_H_
#define _UDP_H_

void  UDP_initialize(void);
void  UDP_close(void);
// Returns a null-terminated string of up to 512 characters. It is
// up to the calling function to call free.
char* UDP_read(int* size);
void  UDP_write(char* c);


//int Devices_potentiometerRead(void);

#endif 