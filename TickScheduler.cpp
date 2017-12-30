#include "TickScheduler.h"

const int TickScheduler::nbTaskMax = 20;
unsigned long TickScheduler::tickTime = 0;
int TickScheduler::taskId = 0;
unsigned long TickScheduler::taskTime[nbTaskMax];
String TickScheduler::taskName[nbTaskMax];

void TickScheduler::beginTick() {
	tickTime = millis();
	taskId = 0;
	for (int i = 0; i < nbTaskMax; i++) {
		taskTime[i] = 0;
		taskName[i] = "";
	}
}

void TickScheduler::beginTask(String task) {
	taskTime[taskId] = millis();
	taskName[taskId] = task;
	taskId++;
}

void TickScheduler::endTask(String task) {
	for (int i = 0; i < nbTaskMax; i++) {
		if (taskName[i].equals(task)) {
			taskTime[i] = millis() - taskTime[i];
			return;
		}
	}
}

void TickScheduler::endTaskAndBeginTask(String taskEnd, String taskBegin) {
	endTask(taskEnd);
	beginTask(taskBegin);
}

void TickScheduler::endTick() {
	tickTime = millis() - tickTime;
	if (tickTime >= 1250) {
		Serial.print("[WARN] Main thread had to hang for ");
		Serial.print(tickTime);
		Serial.println(" ms. Detailed tick time :");
		for (int i = 0; i < nbTaskMax; i++) {
			if (!taskName[i].equals("")) {
				Serial.print("[WARN] Task id ");
				Serial.print(i);
				Serial.print(" (");
				Serial.print(taskName[i]);
				Serial.print(") took ");
				Serial.print(taskTime[i]);
				Serial.println(" ms to execute");
			}
		}
	}
}