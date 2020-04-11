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
	FILE* fp = fopen("input.txt", "r");

	
	Queue ioQueue;
	Queue rQueue0;
	Queue rQueue1;
	Heap rQueue2;
	Queue rQueue3;
	Queue inputQueue;
	Queue fQueue;
	QInit(&rQueue0);
	QInit(&rQueue1);
	HeapInit(&rQueue2);
	QInit(&rQueue3);
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

	/*for (int i = 0; i < numProcess; i++) {
		fgets(buffer, BUFFER_LEN, fp);
		Process* tmp = PInit(buffer);
		HPush(&rQueue2, tmp);
	}

	for (int i = 0; i < numProcess; i++) {
		showProcessInfo(HPop(&rQueue2));
		
	}*/

	



	fclose(fp);

	if (!QIsEmpty(&inputQueue)) {
		if (timeStamp == ProcessArrivalTime(QSearch(&inputQueue))) { // timestamp �� arrivaltime�� ��ġ�ϴ��� üũ
			Process* tmpProc = QPop(&inputQueue);
			if (ProcessPriority(tmpProc) == 0) {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				QPush(&rQueue0, tmpProc);
			}
			else if (ProcessPriority(tmpProc) == 1) {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				QPush(&rQueue1, tmpProc);
			}
			else if (ProcessPriority(tmpProc) == 2) {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				HPush(&rQueue2, tmpProc);
			}
			else {
				printf("P%d is scheduled in time %d to priority %d\n", tmpProc->pid, timeStamp, tmpProc->Queue);
				QPush(&rQueue3, tmpProc);
			}
		}
	}

	while (fQueue.numofdata != numProcess) {
		
		if (!QIsEmpty(&rQueue0)) { // roundrobin, time quantum 2
			timeQuantum = 2;
			for (int i = 0; i < 2; i++) {
				CheckArrivalTimeandPut(&inputQueue, &rQueue0, &rQueue1, &rQueue2, &rQueue3, timeStamp);
				printf("time %d -> time %d ", timeStamp, timeStamp + 1);
				RoundRobin(&rQueue0);
				printf("P%d CPU\n", QSearch(&rQueue0)->pid);
				timeQuantum--;

				timeStamp++;

				if (!QIsEmpty(&ioQueue)) { // ioqueue�� queue�� ������
					IO(&ioQueue); // ioqueue�� ��� ioburst �� 1�� ���δ�.
					if (LPeek(&(QSearch(&ioQueue)->IO)) <= 0) { // IOqueue�� �ִ� elem�� 0�� ��
						LPop(&(QSearch(&ioQueue)->IO));
						Process* tmpProc = QPop(&ioQueue); // �������´�
						printf("P%d io out. ", tmpProc->pid);
						
						if (ProcessPriority(tmpProc) == 0) { // �켱������ 0�̾������
							printf("P%d is upgraded to priority %d\n", tmpProc->pid, tmpProc->Queue);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 1) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 2) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue1, tmpProc);
						}
						else {
							Upgrade(tmpProc, 1);
							HPush(&rQueue2, tmpProc);
						}
					}
				}

				if (LPeek(&(QSearch(&rQueue0)->CPU)) == 0) { // cputime == 0 
					timeQuantum = 0;
					LPop(&(QSearch(&rQueue0)->CPU)); // cputime ����.
					
					Process* tmp = QPop(&rQueue0);
					printf("P%d cpu burst time ended, ", tmp->pid );
					if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
						tmp->turnaroundTime = timeStamp - (tmp->arrivalTime);
						tmp->waitingTime = (tmp->turnaroundTime) - (tmp->burstTime);
						printf("P%d total turnaround Time : %d. ", tmp->pid, tmp->turnaroundTime);
						printf("go to final Queue\n");
						QPush(&fQueue, tmp); // final queue �� ����.
					}
					else { // �Ⱥ�� io�� ���� ���.
						printf("go to io Queue\n");
						QPush(&ioQueue, tmp); // io queue �� ����.
					}

					break;

				}
				else if (timeQuantum == 0) {
					if (LPeek(&(QSearch(&rQueue0)->CPU)) == 0) { // cputime == 0 
						LPop(&(QSearch(&rQueue0)->CPU)); // cputime ����.
						Process* tmp = QPop(&rQueue0);
						printf("P%d cpu burst time ended, ", tmp->pid);
						if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
							tmp->turnaroundTime = timeStamp - (tmp->arrivalTime);
							tmp->waitingTime = (tmp->turnaroundTime) - (tmp->burstTime);
							printf("P%d total turnaround Time : %d. ", tmp->pid, tmp->turnaroundTime);
							printf("go to final Queue\n");
							QPush(&fQueue, tmp); // final queue �� ����.
						}
						else { // �Ⱥ�� io�� ���� ���.
							printf("go to io Queue\n");
							QPush(&ioQueue, tmp); // io queue �� ����.
						}

					}
					else { // cputime�� 0�� �ƴѵ� timequantum�� ���� ��.
						Process* tmp = QPop(&rQueue0);
						Demote(tmp,1);
						QPush(&rQueue1, tmp);
					}

					break;

				}
			}
			
		}
		else if (!QIsEmpty(&rQueue1)) {
			timeQuantum = 6;
			for (int i = 0; i < 6; i++) {
				CheckArrivalTimeandPut(&inputQueue, &rQueue0, &rQueue1, &rQueue2, &rQueue3, timeStamp);
				printf("time %d -> time %d ", timeStamp, timeStamp + 1);
				RoundRobin(&rQueue1);
				printf("P%d CPU\n", QSearch(&rQueue1)->pid);
				timeQuantum--;

				timeStamp++;

				if (!QIsEmpty(&ioQueue)) { // ioqueue�� queue�� ������
					IO(&ioQueue); // ioqueue�� ��� ioburst �� 1�� ���δ�.
					if (LPeek(&(QSearch(&ioQueue)->IO)) <= 0) { // IOqueue�� �ִ� elem�� 0�� ��
						LPop(&(QSearch(&ioQueue)->IO));
						Process* tmpProc = QPop(&ioQueue); // �������´�
						printf("P%d io out. ", tmpProc->pid);
						
						if (ProcessPriority(tmpProc) == 0) { // �켱������ 0�̾������
							printf("P%d is upgraded to priority %d\n", tmpProc->pid, tmpProc->Queue);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 1) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 2) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue1, tmpProc);
						}
						else {
							Upgrade(tmpProc, 1);
							HPush(&rQueue2, tmpProc);
						}
					}
				}

				if (LPeek(&(QSearch(&rQueue1)->CPU)) == 0) { // cputime == 0 
					timeQuantum = 0;
					LPop(&(QSearch(&rQueue1)->CPU)); // cputime ����.
					Process* tmp = QPop(&rQueue1);
					printf("P%d cpu burst time ended, ", tmp->pid);

					if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
						tmp->turnaroundTime = timeStamp - (tmp->arrivalTime);
						tmp->waitingTime = (tmp->turnaroundTime) - (tmp->burstTime);
						printf("P%d total turnaround Time : %d. ", tmp->pid, tmp->turnaroundTime);
						printf("go to final Queue\n");
						QPush(&fQueue, tmp); // final queue �� ����.
					}
					else { // �Ⱥ�� io�� ���� ���.
						printf("go to io Queue\n");
						QPush(&ioQueue, tmp); // io queue �� ����.
					}

					break;

				}
				else if (timeQuantum == 0) {
					if (LPeek(&(QSearch(&rQueue1)->CPU)) == 0) { // cputime == 0 
						LPop(&(QSearch(&rQueue1)->CPU)); // cputime ����.
						Process* tmp = QPop(&rQueue1);
						printf("P%d cpu burst time ended, ", tmp->pid);
						if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
							tmp->turnaroundTime = timeStamp - (tmp->arrivalTime);
							tmp->waitingTime = (tmp->turnaroundTime) - (tmp->burstTime);
							printf("P%d total turnaround Time : %d. ", tmp->pid, tmp->turnaroundTime);
							printf("go to final Queue\n");
							QPush(&fQueue, tmp); // final queue �� ����.
						}
						else { // �Ⱥ�� io�� ���� ���.
							printf("go to io Queue\n");
							QPush(&ioQueue, tmp); // io queue �� ����.
						}

					}
					else { // cputime�� 0�� �ƴѵ� timequantum�� ���� ��.
						Process* tmp = QPop(&rQueue1);
						Demote(tmp,1);
						HPush(&rQueue2,tmp);
					}
					break;
				}

			}
			
		}else if (!HIsEmpty(&rQueue2)) {
			while (!LIsZero(&(HSearch(&rQueue2)->CPU))) {
				CheckArrivalTimeandPut(&inputQueue, &rQueue0, &rQueue1, &rQueue2, &rQueue3, timeStamp);
				printf("time %d -> time %d ", timeStamp, timeStamp + 1);
				SRTN(&rQueue2);
				printf("P%d CPU\n", HSearch(&rQueue2)->pid);
				timeStamp++;

				if (!QIsEmpty(&ioQueue)) { // ioqueue�� queue�� ������
					IO(&ioQueue); // ioqueue�� ��� ioburst �� 1�� ���δ�.
					if (LPeek(&(QSearch(&ioQueue)->IO)) <= 0) { // IOqueue�� �ִ� elem�� 0�� ��
						LPop(&(QSearch(&ioQueue)->IO));
						Process* tmpProc = QPop(&ioQueue); // �������´�
						printf("P%d io out. ", tmpProc->pid);
						if (ProcessPriority(tmpProc) == 0) { // �켱������ 0�̾������
							printf("P%d is upgraded to priority %d\n", tmpProc->pid, tmpProc->Queue);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 1) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 2) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue1, tmpProc);
						}
						else {
							Upgrade(tmpProc, 2);
							HPush(&rQueue2, tmpProc);
						}
					}
				}
			}
			LPop(&(HSearch(&rQueue2)->CPU));
			Process* tmp = HPop(&rQueue2);
			printf("P%d cpu burst time ended, ", tmp->pid);

			if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
				tmp->turnaroundTime = timeStamp - (tmp->arrivalTime);
				tmp->waitingTime = (tmp->turnaroundTime) - (tmp->burstTime);
				printf("P%d total turnaround Time : %d. ", tmp->pid, tmp->turnaroundTime);
				printf("go to final Queue\n");
				QPush(&fQueue, tmp); // final queue �� ����.
			}
			else { // �Ⱥ�� io�� ���� ���.
				printf("go to io Queue\n");
				QPush(&ioQueue, tmp); // io queue �� ����.
			}

		}else if (!QIsEmpty(&rQueue3)){
			
			while (!LIsZero(&(QSearch(&rQueue3)->CPU))) {
				CheckArrivalTimeandPut(&inputQueue, &rQueue0, &rQueue1, &rQueue2, &rQueue3, timeStamp);
				printf("time %d -> time %d ", timeStamp, timeStamp + 1);    
				FCFS(&rQueue3);
				printf("P%d CPU\n", QSearch(&rQueue3)->pid);
				timeStamp++;

				if (!QIsEmpty(&ioQueue)) { // ioqueue�� queue�� ������
					IO(&ioQueue); // ioqueue�� ��� ioburst �� 1�� ���δ�.
					if (LPeek(&(QSearch(&ioQueue)->IO)) <= 0) { // IOqueue�� �ִ� elem�� 0�� ��
						LPop(&(QSearch(&ioQueue)->IO));
						Process* tmpProc = QPop(&ioQueue); // �������´�
						printf("P%d io out. ", tmpProc->pid);
						if (ProcessPriority(tmpProc) == 0) { // �켱������ 0�̾������
							printf("P%d is upgraded to priority %d\n", tmpProc->pid, tmpProc->Queue);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 1) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue0, tmpProc);
						}
						else if (ProcessPriority(tmpProc) == 2) {
							Upgrade(tmpProc, 1);
							QPush(&rQueue1, tmpProc);
						}
						else {
							Upgrade(tmpProc, 2);
							HPush(&rQueue2, tmpProc);
						}
					}
				}
			}
			
			LPop(&(QSearch(&rQueue3)->CPU)); // cputime ����.
			Process* tmp = QPop(&rQueue3);
			printf("P%d cpu burst time ended, ", tmp->pid);

			if (LIsEmpty(&(tmp->CPU))) { // cputime ��� ���
				tmp->turnaroundTime = timeStamp - (tmp->arrivalTime);
				tmp->waitingTime = (tmp->turnaroundTime) - (tmp->burstTime);
				printf("P%d total turnaround Time : %d. ", tmp->pid, tmp->turnaroundTime);
				printf("go to final Queue\n");
				QPush(&fQueue, tmp); // final queue �� ����.
			}
			else { // �Ⱥ�� io�� ���� ���.
				printf("go to io Queue\n");
				QPush(&ioQueue, tmp); // io queue �� ����.
			}
		}

		if (!QIsEmpty(&ioQueue)) { // ioqueue�� queue�� ������
			IO(&ioQueue); // ioqueue�� ��� ioburst �� 1�� ���δ�.
			if (LPeek(&(QSearch(&ioQueue)->IO)) <= 0) { // IOqueue�� �ִ� elem�� 0�� ��
				LPop(&(QSearch(&ioQueue)->IO));
				Process* tmpProc = QPop(&ioQueue); // �������´�
				printf("P%d io out. ", tmpProc->pid);
				
				if (ProcessPriority(tmpProc) == 0) { // �켱������ 0�̾������
					printf("P%d is upgraded to priority %d\n", tmpProc->pid, tmpProc->Queue);
					QPush(&rQueue0, tmpProc);
				}
				else if (ProcessPriority(tmpProc) == 1) {
					Upgrade(tmpProc, 1);
					QPush(&rQueue0, tmpProc);
				}
				else if (ProcessPriority(tmpProc) == 2) {
					Upgrade(tmpProc, 1);
					QPush(&rQueue1, tmpProc);
				}
				else {
					Upgrade(tmpProc, 2);
					HPush(&rQueue2, tmpProc);
				}
			}
		}

	}
	int ttmean=0;
	int wtmean=0;

	for (int i = 0; i < numProcess; i++) {
		Process* tmp = QPop(&fQueue);
		showResultInfo(tmp);
		ttmean+=(tmp->turnaroundTime);
		wtmean += (tmp->waitingTime);
	}

	printf("Average Turnaround Time: %d\n", ttmean / numProcess);
	printf("Average Waiting Time: %d\n", wtmean / numProcess);

	
	
	
	
	


}