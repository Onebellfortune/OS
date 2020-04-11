#include "Process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	plist->cycleArr[(plist->numofdata)-1] = 0;
	
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


//Process function Area
Process* PInit(char* buffer) { //Process에 관한 정보 모두 입력
	Process* pp = (Process*)malloc(sizeof(Process));
	ListInit(&(pp->CPU));
	ListInit(&(pp->IO));
	pp->burstTime = 0;
	pp->turnaroundTime = 0;
	pp->waitingTime = 0;
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

void showProcessInfo(Process* pp) { // 프로세스 정보 출력
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

void showResultInfo(Process* pp) { // 프로세스 모두 끝난 결과 출력
	printf("P%d total Turnaround Time: %d\n", pp->pid, pp->turnaroundTime);
	printf("P%d total Waiting Time: %d\n", pp->pid, pp->waitingTime);
}


// 프로세스가 시간이 지남에 따라 들어올 때마다 첫 스케줄링
void CheckArrivalTimeandPut(Queue* pInputQ, Queue* rQ0, Queue* rQ1, Heap* rQ2, Queue* rQ3, int timeStamp) {
	if (!QIsEmpty(pInputQ)) {
		if (timeStamp == ProcessArrivalTime(QSearch(pInputQ))) { // timestamp 가 arrivaltime과 일치하는지 체크
			Process* tmpProc = QPop(pInputQ);
			if (ProcessPriority(tmpProc) == 0) {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				QPush(rQ0, tmpProc);
			}
			else if (ProcessPriority(tmpProc) == 1) {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				QPush(rQ1, tmpProc);
			}
			else if (ProcessPriority(tmpProc) == 2) {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				HPush(rQ2, tmpProc);
			}
			else {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				QPush(rQ3, tmpProc);
			}
		}
	}
}
//우선순위 증가
void Upgrade(Process* pp,int var) {
	(pp->Queue)-=var;
	printf("P%d is upgraded to priority %d\n", pp->pid, pp->Queue);
}
//우선순위 감소
void Demote(Process* pp,int var) {
	(pp->Queue)+=var;
	printf("P%d is demoted to priority %d\n", pp->pid, pp->Queue);
}

//Queue Area

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
	pq->back = pq->back->next;
	if (pq->numofdata==1) {
		free(rnode);
		(pq->numofdata)--;
		return rproc;
	}
	else {
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
	LMod(&(cpuProcess->CPU), 1);
	
}

void SRTN(Heap* srcQ) {
	Process* cpuProcess = HSearch(srcQ);
	LMod(&(cpuProcess->CPU), 1);
	
}

void FCFS(Queue* srcQ) {
	Process* cpuProcess = QSearch(srcQ);
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

void HeapInit(Heap* ph) {
	ph->numofdata = 0;
}

int HIsEmpty(Heap* ph) {
	return !(ph->numofdata);
}

//완전이진트리 복습.

int HProcessPriority(Heap* ph , int idx) {
	if (idx * 2 > ph->numofdata)
		return 0;
	else if (idx * 2 == ph->numofdata)
		return idx * 2;
	else {
		if(ph->heapArr[idx*2]->CPU.cycleArr[0] < ph->heapArr[idx * 2 + 1]->CPU.cycleArr[0])
			return idx * 2;
		else
			return idx * 2 + 1;
	}
		
}

void HPush(Heap* ph, Process* pp) {
	int idx = (ph->numofdata)+1;
	
	while (idx != 1) {
		if (pp->CPU.cycleArr[0] < ph->heapArr[idx / 2]->CPU.cycleArr[0]) {
			ph->heapArr[idx] = ph->heapArr[idx / 2];
			idx = idx / 2;
		}
		else
			break;
	}

	ph->heapArr[idx] = pp;
	(ph->numofdata)++;

}

Process* HPop(Heap* ph) {
	
	Process* rproc = ph->heapArr[1]; // 뺄 노드
	
	Process* tmpProc = ph->heapArr[(ph->numofdata)]; //마지막에 달린 노드

	int parent = 1;
	int child;

	while (child = HProcessPriority(ph, parent)) // 빼는 노드 중에 왼쪽, 오른쪽 중에 우선순위 높은 (CPU Time 짧은 것) 선택
	{
		if (tmpProc->CPU.cycleArr[0] <= ph->heapArr[child]->CPU.cycleArr[0]) // 우선순위가 더 낮은 노드를 만날 때까지
			break;
		ph->heapArr[parent] = ph->heapArr[child]; // 계속 자리를 스위칭
		parent = child;
	}

	ph->heapArr[parent] = tmpProc; // 자리를 찾았기 때문에 바꿈
	(ph->numofdata)--;
	return rproc; // 뺄 노드 리턴
}

Process* HSearch(Heap* ph) {
	return ph->heapArr[1];
}


