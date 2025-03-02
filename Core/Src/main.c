/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "util.h"
#include "kernel.h"
#include <stdio.h>

typedef struct Counter {
	uint32_t current;
	uint32_t end;
	void (*done_function)(void);
} Counter;


void delay_counter(void* arg) {
	// counts to takes Timer struct then yields when timer is over
	while (true) {
		Counter* counter = (Counter*)arg;
		bool done = false;
		uint32_t systemStart = osGetTime();
		uint32_t systemTime = 0;
		while(!done) {
			systemTime = osGetTime();
			if ((systemTime - systemStart) % 1000 >= 0) {
				systemStart = osGetTime();
				counter->current++;
				done = (counter->current >= counter->end);
			}
			osYield();
		}
		counter->done_function();
		counter->current = 0;
		done = false;
	}
}



void display_counter(void* arg) {
	Counter* counter = (Counter*)arg;
	while(true) {
		printf("Count: %d/%d\r\n", counter->current, counter->end);
		for (int j=0; j<100000; j++) {};
	}
}
void print_th3(void*) {
	int i = 0;
	while(true) {
		for(int i = 0; i < 200002; i++){}
		printf("Thread 3!\r\n");
		i++;
//		osYield();
	}
}


const char* boolToString(bool value) {
    return value ? "true" : "false";
}

void print_done() {
	for (int i=0; i<1000; i++) printf("DONE!\r\n");
}

int main(void)
{
	int cool_num = 10;
	Init_System();

	Counter counter;
	counter.current = 0;
	counter.end = 20;
	counter.done_function = print_done;



//  lab instruction to comment this out
//  uint32_t control_val = 0x2;
//    __set_CONTROL(control_val);
    printf("\n\n\n\n\nSTARTING \r\n");
    osKernelInit();
    printf(boolToString(osCreateThreadWithDeadline(delay_counter, &counter, 10000)));
    printf("\r\n");
    printf(boolToString(osCreateThreadWithDeadline(display_counter, &counter, 500)));
    printf("\r\n");
    printf(boolToString(osCreateThread(print_th3, &cool_num))); // this shouldn't be created because MAX_THREADS is set to 2
    printf("\r\n");
    osKernelStart();
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
