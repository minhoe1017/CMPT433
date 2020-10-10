
static void Main_configure(void);
static void Main_refreshArray(void);
//static void Main_runDisplay(void);


extern void  Sorter_startSorting (void);
extern void  Interface_run (void);
extern void  Devices_configure(void);
extern void  UDP_initialize(void);
extern void  UDP_write(char* message);
extern _Bool Sorter_isSorting(void);
extern int   Devices_potentiometerRead(void);
extern void  Sorter_setArraySize(int newSize);
extern void  writeNum(int i2cFileDesc, int number);
extern int   i2cInit(void);
extern long long Sorter_getNumberArraysSorted (void);
