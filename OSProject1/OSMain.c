#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process.h"

int timeStamp = 0;

#define BUFFER_LEN 100


int main() {

	int numProcess;
	int timeQuantum = 0;
	char buffer[BUFFER_LEN];
	FILE* fp = fopen("test.txt", "r");

	Queue processor;
	Queue ioQueue;
	Queue rQueue0;
	Queue rQueue1;
	Queue rQueue2;
	Queue rQueue3;
	Queue inputQueue;
	Queue fQueue;
	QInit(&rQueue0);
	QInit(&rQueue1);
	QInit(&rQueue2);
	QInit(&rQueue3);
	QInit(&processor);
	QInit(&ioQueue);
	QInit(&inputQueue);
	QInit(&fQueue);



	fgets(buffer, BUFFER_LEN, fp);
	numProcess = atoi(buffer);

	for (int i = 0; i < numProcess; i++) {
		fgets(buffer, BUFFER_LEN, fp);
		Process* tmp = PInit(buffer);
		QPush(&inputQueue, tmp);
	}

	fclose(fp);

	while (fQueue.numofdata != numProcess) {
		printf("time : %d ", timeStamp);
		if (!QIsEmpty(&inputQueue)) {
			if (timeStamp == ProcessArrivalTime(QSearch(&inputQueue))) { // timestamp �� arrivaltime�� ��ġ�ϴ��� üũ
				Process* tmpProc = QPop(&inputQueue);
				if (ProcessPriority(tmpProc) == 0) {
					QPush(&rQueue0, tmpProc);
				}
				else if (ProcessPriority(tmpProc) == 1) {
					QPush(&rQueue1, tmpProc);
				}
				else if (ProcessPriority(tmpProc) == 2) {
					QPush(&rQueue2, tmpProc);
				}
				else {
					QPush(&rQueue3, tmpProc);
				}
			}
		}
		

		if (!QIsEmpty(&rQueue0)) { // roundrobin, time quantum 2
			if (timeQuantum == 0) { // timequantum ÷ �����ϴ°��
				timeQuantum = 2;
			}
			RoundRobin(&rQueue0);
			printf("P%d CPU\n", QSearch(&rQueue0)->pid);
			timeQuantum--;

			if (LPeek(&(QSearch(&rQueue0)->CPU))==0){ // cputime == 0 
				LPop(&(QSearch(&rQueue0)->CPU)); // cputime ����.
				Process* tmp = QPop(&rQueue0);
				if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
					QPush(&fQueue, tmp); // final queue �� ����.
				}
				else { // �Ⱥ�� io�� ���� ���.

					QPush(&ioQueue,tmp); // io queue �� ����.
				}
				
			}
			else if (timeQuantum == 0) {
				if (LPeek(&(QSearch(&rQueue0)->CPU)) == 0) { // cputime == 0 
					LPop(&(QSearch(&rQueue0)->CPU)); // cputime ����.
					Process* tmp = QPop(&rQueue0);
					if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
						QPush(&fQueue, tmp); // final queue �� ����.
					}
					else { // �Ⱥ�� io�� ���� ���.
						QPush(&ioQueue, tmp); // io queue �� ����.
					}

				}
				else { // cputime�� 0�� �ƴѵ� timequantum�� ���� ��.
					Process* tmp = QPop(&rQueue0);
					Demote(tmp);
					QPush(&rQueue1,tmp);
				}
			}

		}
		else if (!QIsEmpty(&rQueue1)) {
			if (timeQuantum == 0) { // timequantum ÷ �����ϴ°��
				timeQuantum = 6;
			}
			RoundRobin(&rQueue1);
			printf("P%d CPU\n", QSearch(&rQueue1)->pid);
			timeQuantum--;

			if (LPeek(&(QSearch(&rQueue1)->CPU)) == 0) { // cputime == 0 
				LPop(&(QSearch(&rQueue1)->CPU)); // cputime ����.
				Process* tmp = QPop(&rQueue1);
				if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
					QPush(&fQueue, tmp); // final queue �� ����.
				}
				else { // �Ⱥ�� io�� ���� ���.
					QPush(&ioQueue, tmp); // io queue �� ����.
				}

			}
			else if (timeQuantum == 0) {
				if (LPeek(&(QSearch(&rQueue1)->CPU)) == 0) { // cputime == 0 
					LPop(&(QSearch(&rQueue1)->CPU)); // cputime ����.
					Process* tmp = QPop(&rQueue1);
					if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
						QPush(&fQueue, tmp); // final queue �� ����.
					}
					else { // �Ⱥ�� io�� ���� ���.
						QPush(&ioQueue, tmp); // io queue �� ����.
					}

				}
				else { // cputime�� 0�� �ƴѵ� timequantum�� ���� ��.
					Process* tmp = QPop(&rQueue1);
					//Demote(tmp);
					//QPush(&rQueue2,tmp);
					QPush(&rQueue1, tmp);
				}
			}
		}
		/*else if (!QIsEmpty(&rQueue2)) {
			SRTN(&rQueue2);

		}else{
			FCFS(&rQueue3);
		}*/

		if (!QIsEmpty(&ioQueue)) { // ioqueue�� queue�� ������
			IO(&ioQueue); // ioqueue�� ��� ioburst �� 1�� ���δ�.
			if (LPeek(&(QSearch(&ioQueue)->IO))<=0) { // IOqueue�� �ִ� elem�� 0�� ��
				LPop(&(QSearch(&ioQueue)->IO));
				Process* tmpProc= QPop(&ioQueue); // �������´�
				Upgrade(tmpProc);
				if (ProcessPriority(tmpProc) == 0) { // �켱������ 0�̾������
					QPush(&rQueue0, tmpProc);
				}
				else if (ProcessPriority(tmpProc) == 1) {
					QPush(&rQueue0, tmpProc);
				}
				else if (ProcessPriority(tmpProc) == 2) {
					QPush(&rQueue1, tmpProc);
				}
				else {
					QPush(&rQueue1, tmpProc);
				}
			}
		}

		timeStamp++;

	}

	
	
	


}