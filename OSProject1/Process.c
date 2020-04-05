#include "Process.h"

void ListInit(List* plist) {
	plist->numCycle = 0;
}

int LIsEmpty(List* plist) {
	return !(plist->numCycle);
}

void LPush(List* plist, Data data) {
	plist->cycleArr[plist->numCycle] = data;
	(plist->numCycle)++;
}

Data LPop(List* plist) {
	Data tmp = plist->cycleArr[plist->numCycle];
	(plist->numCycle)--;
	return tmp;
}

void PReceive(Process* pproc) {
	ListInit(pproc->IO);
	ListInit(pproc->CPU);
}

