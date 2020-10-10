// sorter.h
// Module to spawn a separate thread to sort random arrays 
// (permutations) on a background thread. It provides access to the
// contents of the current (potentially partially sorted) array, 
// and to the count of the total number of arrays sorted.
#ifndef _SORTER_H_
#define _SORTER_H_


struct arrayStruct {
  _Bool isSorting;
  int arraySize;
  int nextArraySize;
  long long arraysSorted;
  int* arrayData;
} arraystruct;


// Local function to allocate and permute array.
static void Sorter_initializeArray (void);

// Bubble sort loops through the array over and
// over. With each loop it moves from from the 
// first index to the second-to-last, each time
// swapping with the adjacent integer if they
// are in the wrong order. With each loop, we
// make one less comparison, as per wikipedia.
static void Sorter_bubbleSort (void);

// Initializes array, permutes it then sorts it
// until isSorting is set to false.
static void Sorter_loopSort (void);

// Begin/end the background thread which sorts random permutations.
void Sorter_startSorting (void);
void Sorter_stopSorting (void);
_Bool Sorter_isSorting (void);

// Get the size of the array currently being sorted.
// Set the size the next array to sort (don’t change current array)
void Sorter_setArraySize (int newSize);
int Sorter_getArrayLength (void);

// Get a copy of the current (potentially partially sorted) array.
// Returns a newly allocated array and sets 'length' to be the 
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
int* Sorter_getArrayData (int *length);

// Get the number of arrays which have finished being sorted.
long long Sorter_getNumberArraysSorted (void);
#endif