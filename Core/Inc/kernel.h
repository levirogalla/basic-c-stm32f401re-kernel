
#ifndef KERNEL_H    // Include guard to prevent multiple inclusions
#define KERNEL_H
#include <stdint.h>
#include <stdbool.h>
#define SHPR2 *(uint32_t*)0xE000ED1C
#define SHPR3 *(uint32_t*)0xE000ED20
#define _ICSR *(uint32_t*)0xE000ED04

typedef struct k_thread {
	uint32_t* sp;
	uint32_t usedMemory; // doesn't use this for now
	uint32_t runtime;
	uint32_t timeslice;
//	void (*thread_function)(void*); // don't need this, it gets saved to the stack
} KernelThread;

typedef struct k_state {
	uint32_t* prevStackInitLocation;
	uint8_t numThreads;
	KernelThread* currentThread;
	uint32_t runtime; // max value of 2^32 then restarts
} KernelState;


bool osCreateThread(void (*thread_function)(void*), void* arg);
bool osCreateThreadWithDeadline(void (*thread_function)(void*), void* arg, uint32_t timeout);
void osKernelInit(void);
void osKernelStart(void);
void osYield();
uint32_t osGetTime();

void Init_System(void);



#endif
