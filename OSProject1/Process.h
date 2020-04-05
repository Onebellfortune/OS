#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>

typedef struct {
	int pid;
	int arrivalTime;
	int numCycles;
	List CPU;
	List IO;
}Process;

typedef int Data;

typedef struct {
	Data cycleArr[20];
	int numCycle;
}List;
#endif