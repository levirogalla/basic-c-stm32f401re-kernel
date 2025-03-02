#include "kernel.h"
#include "util.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// sys calls
#define RUN_FIRST_THREAD 3
#define YIELD 4

// constants
#define MAX_THREADS 2
#define DEFAULT_TIMEOUT_MS 5


Queue threadsQ;
KernelState kState;

extern void runFirstThread();
void SVC_Handler_Main(unsigned int* svc_args);

// LOCAL STATIC FUNCTION DEFINITIONS
static uint32_t* _findNextStackLocation(bool* status);
static bool _osSelectNextThreadStack();

// STANDARD FUNCTION IMPLEMENTATIONS
int __io_putchar(int ch);


// INIT FUNCTIONS
void Init_System(void) {
	  HAL_Init();
	  SystemClock_Config();
	  MX_GPIO_Init();
	  MX_USART2_UART_Init();
}

void osKernelInit(void) {
	
	initQueue(&threadsQ, MAX_THREADS);
	uint32_t* MSP_INIT_VAL = *(uint32_t**)0x08000000;
	printf("MSP_INIT_VAL: %p\r\n", MSP_INIT_VAL);
	kState.prevStackInitLocation = MSP_INIT_VAL;
	kState.numThreads = 0;
	kState.runtime = 0;
	

	SHPR3 |= 0xFE << 16; //shift the constant 0xFE 16 bits to set PendSV priority
	SHPR2 |= 0xFDU << 24; //Set the priority of SVC higher than PendSV
}

// THREAD FUNCTIONS
bool osCreateThread(void (*thread_function)(void*), void* arg) {
	return osCreateThreadWithDeadline(thread_function, arg, DEFAULT_TIMEOUT_MS);
}


bool osCreateThreadWithDeadline(void (*thread_function)(void*), void* arg, uint32_t timeout) {
	bool success;
	uint32_t* sp = _findNextStackLocation(&success);
	if (!success) return 0;

	kState.prevStackInitLocation = sp;

	// initialize KernelThread
	KernelThread* newThread = (KernelThread*)malloc(sizeof(KernelThread));
	newThread->sp = sp;
	newThread->timeslice = timeout;

	*(--newThread->sp) = 1 << 24; // set xPSR
	*(--newThread->sp) = (uint32_t)thread_function; // set PC
	*(--newThread->sp) = 0xA; // set LR
	*(--newThread->sp) = 0xA; // set R12
	*(--newThread->sp) = 0xA; // set R3
	*(--newThread->sp) = 0xA; // set R2
	*(--newThread->sp) = 0xA; // set R1
	*(--newThread->sp) = (uint32_t)arg; // set R0, pass the args to the thread
	for (int i = 0; i<8; i++) {
	  	*(--newThread->sp) = 0xA; // set R11 through R4
	}

	enqueue(&threadsQ, newThread);
	return 1;
}



void osSched() {
	kState.currentThread->sp = (uint32_t*)(__get_PSP()-8*4);
	_osSelectNextThreadStack();
}

uint32_t osGetTime() {
	return kState.runtime;
}


// SYSTEM CALL API
void osYield() {
	__asm("SVC #4");
}

void osKernelStart(void) {
	__asm("SVC #3");
}

// SYSTEM CALL HANDLER
void SVC_Handler_Main(unsigned int* svc_args) {
	unsigned int svc_number;
	svc_number = ((char*)svc_args[6])[-2];
	switch(svc_number) {
		case RUN_FIRST_THREAD:
			_osSelectNextThreadStack();
			runFirstThread();

			break;
		case YIELD:
			_ICSR |= 1<<28;
			__asm("isb");
			break;
	}
}


// HELPER FUNCTIONS
static uint32_t* _findNextStackLocation(bool* status) {
// returns next stack location and sets status to 1 if valid location and 0 if not valid
	if (kState.numThreads >= MAX_THREADS) {
		*status = 0;
		return 0;
	}
	uint32_t* sp = kState.prevStackInitLocation - 0x400;
	kState.numThreads++;
	*status = 1;
	return sp;
}

static bool _osSelectNextThreadStack() {
	// Dequeues a stack, sets the current TCB reference, sets PSP, re queues TCB.
	if (isEmpty(&threadsQ)) return 0;
	KernelThread* threadToRun = (KernelThread*)dequeue(&threadsQ);
	kState.currentThread = threadToRun;
	kState.currentThread->runtime = kState.currentThread->timeslice;
	__set_PSP((uint32_t)threadToRun->sp);
	enqueue(&threadsQ, threadToRun);
	return 1;
}

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2,(uint8_t*)&ch,1,HAL_MAX_DELAY);
	return ch;
}




