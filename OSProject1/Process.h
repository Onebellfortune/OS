#ifndef __PROCESS_H__
#define __PROCESS_H__



typedef int Data;

typedef struct _list {
	Data cycleArr[20];
	int numofdata;
}List;

typedef struct _process {
	int pid;
	int Queue;
	int arrivalTime;
	int numcycles;
	int burstTime;
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

void ListInit(List* plist);
int LIsEmpty(List* plist);
void LPush(List* plist, Data data);
Data LPop(List* plist);
int LIsZero(List* plist);
void LMod(List* plist, int var);
int LPeek(List* plist);

Process* PInit(char* buffer);
void showProcessInfo(Process* pp);
void Upgrade(Process* pp);
void Demote(Process* pp);

void QInit(Queue* pq);
void QPush(Queue* pq, Process* pp);
Process* QPop(Queue* pq);
Process* QSearch(Queue* pq);
int QIsEmpty(Queue* pq);

int ProcessArrivalTime(Process* pp);
int ProcessPriority(Process* pp);

void RoundRobin(Queue* srcQ);
void SRTN(Queue* srcQ);
void FCFS(Queue* srcQ);
void IO(Queue* ioQ);

#endif