#include "Process.h"
#include <stdio.h>
#define BUFFER_LEN 100

void ListInit(List* plist) {
	plist->numofdata = 0;
}

int LIsEmpty(List* plist) {
	return !(plist->numofdata);
}

void LPush(List* plist, Data data) {
	plist->cycleArr[plist->numofdata]= data;
	(plist->numofdata)++;
}

Data LPop(List* plist) {
	Data tmp = plist->cycleArr[0];
	for (int i = 0; i < (plist->numofdata)-1; i++) {
		plist->cycleArr[i] = plist->cycleArr[i + 1];
	}
	
	(plist->numofdata)--;
	return tmp;
}

void LMod(List* plist, int var) {
	(plist->cycleArr[0]) -= var;
}

int LPeek(List* plist) {
	return plist->cycleArr[0];
}

int LIsZero(List* plist) {
	return !(plist->cycleArr[0]);
}

Process* PInit(char* buffer) {
	Process* pp = (Process*)malloc(sizeof(Process));
	ListInit(&(pp->CPU));
	ListInit(&(pp->IO));
	pp->burstTime = 0;
	
	char* ptr = strtok(buffer, " ");
	pp->pid = atoi(ptr);
	ptr = strtok(NULL, " ");
	pp->Queue = atoi(ptr);
	ptr = strtok(NULL, " ");
	pp->arrivalTime = atoi(ptr);
	ptr = strtok(NULL, " ");
	pp->numcycles = atoi(ptr);
	for (int i = 0; i < pp->numcycles - 1; i++) {
		ptr = strtok(NULL, " ");
		LPush(&(pp->CPU), atoi(ptr));
		(pp->burstTime) += atoi(ptr);
		ptr = strtok(NULL, " ");
		LPush(&(pp->IO), atoi(ptr));
		(pp->burstTime) += atoi(ptr);
	}
	ptr = strtok(NULL, " ");
	LPush(&(pp->CPU), atoi(ptr));
	(pp->burstTime) += atoi(ptr);
	return pp;
}

void showProcessInfo(Process* pp) {
	printf("pid: %d\n", pp->pid);
	printf("queue: %d\n", pp->Queue);
	printf("arrivalTime: %d\n", pp->arrivalTime);
	printf("number of cycle: %d\n", pp->numcycles);
	printf("CPU BURST TIME: ");
	for (int i = 0; i < pp->numcycles; i++) {
		printf("%d ", LPop(&(pp->CPU)));
	}
	printf("\n");
	printf("I/O BURST TIME: ");
	for (int i = 0; i < pp->numcycles - 1; i++) {
		printf("%d ", LPop(&(pp->IO)));
	}
	printf("\n");
}

void Upgrade(Process* pp) {
	(pp->Queue)--;
}

void Demote(Process* pp) {
	(pp->Queue)++;
}

void QInit(Queue* pq) {
	pq->front = NULL;
	pq->back = NULL;
	pq->numofdata = 0;
}

void QPush(Queue* pq, Process* pp) {
	
	Node* pNode = (Node*)malloc(sizeof(Node));

	pNode->data = pp;
	pNode->next = NULL;

	if (pq->numofdata==0) {
		pq->front = pNode;
		pq->back = pNode;
	}
	else {
		pq->front->next = pNode;
		pq->front = pNode;
	}

	(pq->numofdata)++;

}

Process* QPop(Queue* pq) {
	Node* rnode = pq->back;
	Process* rproc = rnode->data;
	if (pq->numofdata==1) {
		free(rnode);
		(pq->numofdata)--;
		return rproc;
	}
		
	else {
		pq->back = pq->back->next;
		free(rnode);
		(pq->numofdata)--;
		return rproc;
	}
}

Process* QSearch(Queue* pq) {
	return pq->back->data;
}



int QIsEmpty(Queue* pq) {
	return !(pq->numofdata);
}

int ProcessArrivalTime(Process* pp) {
	return pp->arrivalTime;
	
}

int ProcessPriority(Process* pp) {
	return pp->Queue;
}

void RoundRobin(Queue* srcQ) {
	Process* cpuProcess=QSearch(srcQ);
	/*int timeCount = 0;
	for (int i = 0; i < timeQuantum; i++) {
		LMod(&cpuProcess.CPU, 1);
		timeCount++;
		timeStamp++;
		if (LPeek(&cpuProcess) == 0)
			break;
	}
	return timeCount;*/
	LMod(&(cpuProcess->CPU), 1);


	
}

void IO(Queue* ioQ) {
	Node* tmp = ioQ->back;
	if(ioQ->numofdata==1){
		LMod(&(ioQ->back->data->IO), 1);
	}
	else if (ioQ->numofdata > 1) {
		while(ioQ->back!=ioQ->front){
			LMod(&(ioQ->back->data->IO), 1);
			ioQ->back = ioQ->back->next;
		}
	}
	ioQ->back = tmp;
	
}