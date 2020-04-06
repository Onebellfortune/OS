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

typedef Process pdata;

typedef struct _node {
	pdata data;
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

Process* PInit(char* buffer);
void showProcessInfo(Process* pp);

void QInit(Queue* pq);
void QPush(Queue* pq, pdata data);
pdata QPop(Queue* pq);
pdata QSearch(Queue* pq);
#endif