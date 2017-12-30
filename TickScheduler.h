#ifndef TickScheduler_h
#define TickScheduler_h

#include <ESP8266WiFi.h>

class TickScheduler {
	private:
		static const int nbTaskMax;
		static unsigned long tickTime;
		static int taskId;
		static unsigned long taskTime[];
		static String taskName[];
	public:
		static void beginTick();
		static void beginTask(String);
		static void endTask(String);
		static void endTaskAndBeginTask(String, String);
		static void endTick();
};

#endif