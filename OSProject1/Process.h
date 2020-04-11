#ifndef __PROCESS_H__
#define __PROCESS_H__

#define ARR_LEN 20

typedef int Data;

typedef struct _list {
	Data cycleArr[ARR_LEN];
	int numofdata;
}List;

typedef struct _process {
	int pid;
	int Queue;
	int arrivalTime;
	int numcycles;
	int burstTime;
	int turnaroundTime;
	int waitingTime;
	List CPU;
	List IO;
}Process;



typedef struct _node {
	Process* data;
	struct _node* next;
}Node;

typedef struct _queue {
	Node * front;
	Node * back;
	int numofdata;
}Queue;

typedef struct _heap {
	Process* heapArr[ARR_LEN];
	int numofdata;
}Heap;

typedef Heap PQueue;

void ListInit(List* plist);
int LIsEmpty(List* plist);
void LPush(List* plist, Data data);
Data LPop(List* plist);
int LIsZero(List* plist);
void LMod(List* plist, int var);
int LPeek(List* plist);

Process* PInit(char* buffer);
void showProcessInfo(Process* pp);
void showResultInfo(Process* pp);
void CheckArrivalTimeandPut(Queue* pInputQ, Queue* rQ0, Queue* rQ1, Heap* rQ2, Queue* rQ3, int timeStamp);
void Upgrade(Process* pp, int var);
void Demote(Process* pp, int var);

void QInit(Queue* pq);
void QPush(Queue* pq, Process* pp);
Process* QPop(Queue* pq);
Process* QSearch(Queue* pq);
int QIsEmpty(Queue* pq);

int ProcessArrivalTime(Process* pp);
int ProcessPriority(Process* pp);

void RoundRobin(Queue* srcQ);
void SRTN(Heap* srcQ);
void FCFS(Queue* srcQ);
void IO(Queue* ioQ);

void HeapInit(Heap* ph);
int HIsEmpty(Heap* ph);
int HProcessPriority(Heap* ph, int idx);
void HPush(Heap* ph, Process* pp);
Process* HPop(Heap* ph);
Process* HSearch(Heap* ph);


#endif
