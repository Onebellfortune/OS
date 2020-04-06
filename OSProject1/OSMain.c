#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process.h"



#define BUFFER_LEN 100


int main() {

	int numProcess;
	char buffer[BUFFER_LEN];
	FILE* fp = fopen("input.txt", "r");

	Queue rqueue;
	Queue fqueue;
	Queue queue0;
	Queue queue1;
	Queue queue2;
	Queue queue3;
	QInit(&queue0);
	QInit(&queue1);
	QInit(&queue2);
	QInit(&queue3);
	QInit(&rqueue);
	QInit(&fqueue);

	int timeStamp;

	fgets(buffer, BUFFER_LEN, fp);
	numProcess = atoi(buffer);

	for (int i = 0; i < numProcess; i++) {
		fgets(buffer, BUFFER_LEN, fp);
		Process* tmp = PInit(buffer);
		QPush(&rqueue, *tmp);
	}

	while (fqueue.numofdata == numProcess) {
		
	}

	
	
	


}