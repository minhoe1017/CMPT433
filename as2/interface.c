#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "interface.h"

static void Interface_help (void) {
	UDP_write("Accepted command examples:\ncount      -- display number arrays sorted.\nget length -- display length of array currently being sorted.\nget array  -- display the full array being sorted.\nget 10     -- display the tenth element of array currently being sorted.\nstop       -- cause the server program to end.\n");
}

static void Interface_count (void) {
	long long count  = Sorter_getNumberArraysSorted();
	char scount[50];
	snprintf(scount, 50 * sizeof(char), "%lld", count);
	char result[100] = "Number of arrays sorted = ";
	strcat(result, scount);
	strcat(result, ".\n");
	UDP_write(result);
}

static void Interface_getLength (void) {
	int length = Sorter_getArrayLength();
	char slength[50];
	snprintf(slength, 50 * sizeof(char), "%d", length);
	char result[100] = "Current array length = ";
	strcat(result, slength);
	strcat(result, ".\n");
	UDP_write(result);
}

static void Interface_getArray (void) {
	// HAVE TO BE CAREFUL ABOUT THE LENGTH! ONLY PRINT THE
	// MAX LENGTH OF STRING SHOULD BE 512 -1 FOR NULL TERMINATION
	// THEN YOU CAN CALL UDP_write() AGAIN FOR THE NEXT CHUNK
	int length;
	int* results = Sorter_getArrayData(&length);
	char message[512];
	message[0] = '\0';
	for (int i=0; i<length; i++) {
		char num[50];
		snprintf(num, 50 * sizeof(char), "%d", results[i]);
		if (strlen(message) + strlen(num) > 500) {
			perror("string too long to write!\n");
			exit(-3);
		}
		strcat(message, num);
		if (i==length-1) {
			strcat(message, "\n");
			UDP_write(message);
			message[0] = '\0';
			break;
		}
		if (i%10 == 9) {
			strcat(message, ",\n");
			UDP_write(message);
			message[0] = '\0';
		}
		else {
			strcat(message, ", ");
		}
	}

	results = malloc(500*sizeof(int));
	free(results);

}

static void Interface_get (int index) {
	int length;
	int* results = Sorter_getArrayData(&length);
	char message[200];
	char slength[50];
	char sindex[50];
	snprintf(slength, 50 * sizeof(char), "%d", length);
	snprintf(sindex, 50 * sizeof(char), "%d", index);
	if (index < 1 || index > length) {
		strcpy(message, "Index out of range. You can get a value from the current array index range, which is 1-");
		strcat(message, slength);
		strcat(message, "\n");
		UDP_write(message);
	}
	else {
		char svalue[50];
		snprintf(svalue, 50 * sizeof(char), "%d", results[index-1]);
		strcpy(message, "Value ");
		strcat(message, sindex);
		strcat(message, " = ");
		strcat(message, svalue);
		strcat(message, "\n");
		UDP_write(message);
	}
	free(results);
}

static void Interface_stop (void) {
	UDP_write("Program terminating\n");
	UDP_close();
	Sorter_stopSorting();
}

static void Interface_error (void) {
	UDP_write("Command not recognized. Please type \"help\" for a list of commands.\n");
}

static _Bool Interface_compare (char* str1, char* str2, int length) {
	for (int i=0; i<length; i++) {
		if (str1[i] != str2[i]) {
			return false;
		}
	}
	return true;
}


// Read the command coming in through UDP, which
// will respond with the appropriate message.
void Interface_run (void) {
	while (true) {
		int readSize = 0;
		char* buffer = UDP_read(&readSize);

		if (Interface_compare(buffer, "help", 4))
			Interface_help();
		else if (Interface_compare(buffer, "count", 5))
			Interface_count();
		else if (Interface_compare(buffer, "get length", 10))
			Interface_getLength();
		else if (Interface_compare(buffer, "get array", 9))
			Interface_getArray();
		else if (Interface_compare(buffer, "get ", 4))
			Interface_get(round(atoi(&buffer[4])));
		else if (Interface_compare(buffer, "stop", 4)) {
			Interface_stop();
			break;
		}
		else
			Interface_error();
		free(buffer);
	}
}
