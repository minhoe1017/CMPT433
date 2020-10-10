#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		//rand?
#include <pthread.h>
#include <stdbool.h>	//for bool
#include <sys/wait.h>	//for sleep?
#include <string.h>		//for memcpy
#include "sorter.h"



static struct arrayStruct arr;
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
pthread_t thread;

static void Sorter_initializeArray (void) {
	int pthread_mutex_lock(pthread_mutex_t *mutex);
	arr.arraySize = arr.nextArraySize;
//	printf("arraySize: %d; nextArraySize: %d\n", arr.arraySize, arr.nextArraySize);
	arr.arrayData = malloc(arr.arraySize * sizeof(int));
	if (arr.arrayData == NULL) {
		printf("malloc failed\n");
		return;
	}
	int i;
	for (i=1; i<=arr.arraySize; i++) {
		arr.arrayData[i-1] = i;
	}
	int pthread_mutex_unlock(pthread_mutex_t *mutex);
	for (i=0; i<arr.arraySize; i++) {
		int random = rand() % arr.arraySize;
//		printf("random: %d; arr.size: %d ", random, arr.arraySize);
		int temp = arr.arrayData[i];
		int pthread_mutex_lock(pthread_mutex_t *mutex);
		arr.arrayData[i] = arr.arrayData[random];
		arr.arrayData[random] = temp;
		int pthread_mutex_unlock(pthread_mutex_t *mutex);
	}
//	printf("stop");
}

static void Sorter_bubbleSort (void) {
	int last = arr.arraySize-2;
	_Bool swapped;
	while (last >= 0) {
		swapped = false;
		int i;
		for (i=0; i<=last; i++) {
			if (arr.arrayData[i] > arr.arrayData[i+1]) {
				int temp = arr.arrayData[i];
				int pthread_mutex_lock(pthread_mutex_t *mutex);
				arr.arrayData[i] = arr.arrayData[i+1];
				arr.arrayData[i+1] = temp;
				int pthread_mutex_unlock(pthread_mutex_t *mutex);
				swapped = true;
			}
		}
		if (swapped == false) {
			return;
		}
		last--;
	}
}

static void Sorter_loopSort (void) {
	while(arr.isSorting) {
		Sorter_initializeArray();
		if (arr.arrayData != NULL) {
			Sorter_bubbleSort();
			arr.arraysSorted++;
			//free(arr.arrayData);
		}
	}
	free(arr.arrayData);
}

// Begin/end the background thread which sorts random permutations.
void Sorter_startSorting (void){
	printf("starting sorting\n");
	arr.arraySize = 100;
	printf("array size: %d\n", arr.arraySize);
	arr.nextArraySize = 100;
	arr.arraysSorted = 0;
	arr.isSorting = true;


	int iret1;
	iret1 = pthread_create(&thread, NULL, (void*) Sorter_loopSort, NULL);
	if (iret1 != 0) {
		printf("Thread initialization failed!\n");
	}
}
void Sorter_stopSorting (void) {
	arr.isSorting = false;
	pthread_join(thread, NULL);
	int pthread_mutex_destroy(pthread_mutex_t *mutex);
}
_Bool Sorter_isSorting (void) {
	return arr.isSorting;
}

// Set the size the next array to sort (don’t change current array)
void Sorter_setArraySize (int newSize) {
	arr.nextArraySize = newSize;
}

// Get the size of the array currently being sorted.
int Sorter_getArrayLength (void) {
	return arr.arraySize;
}

// Get a copy of the current (potentially partially sorted) array.
// Returns a newly allocated array and sets 'length' to be the 
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
int* Sorter_getArrayData (int *length) {
	int pthread_mutex_lock(pthread_mutex_t *mutex);
	*length = arr.arraySize;
	int* temp = malloc(*length * sizeof(int));
	for (int i=0; i<*length; i++) {
		temp[i] = arr.arrayData[i];
	}
	int pthread_mutex_unlock(pthread_mutex_t *mutex);
	return temp;
}

// Get the number of arrays which have finished being sorted.
long long Sorter_getNumberArraysSorted (void) {
	return arr.arraysSorted;
}
/*
int main () {
	Sorter_startSorting();
	printf("Get array length: %d\n", Sorter_getArrayLength());
	printf("Set array length: 50\n");
	Sorter_setArraySize(30000);
	printf("Get array length: %d\n", Sorter_getArrayLength());
	sleep(1);
//	printf("slept\n");
	printf("Get array length: %d\n", Sorter_getArrayLength());
	Sorter_stopSorting();

//	printf("already slept and stopped sorting\n");
	printf("Number of arrays sorted: %lld\n", Sorter_getNumberArraysSorted());
	printf("Get array length: %d\n", Sorter_getArrayLength());
}*/
