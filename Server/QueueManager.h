#pragma once
#include <queue>
#include "MessageProtocol.h"

typedef ACE_Singleton<ACE_Mutex, ACE_Null_Mutex> MyMutex;

class QueueManager {
public:
	static MyMutex _pushMutex;
	static std::queue<request> requests;   // <- Everything read.
	static std::queue<response> responses; // <- Everything to write.

public:
	static void getresponse(response& r) {
		r = responses.front();
		responses.pop();
	}

	static void addresponse(response& r) {
		while (_pushMutex.instance->tryacquire() == -1) { /* wait(10); */ }
		// _pushMutex.acquire();
		responses.push(r);
		_pushMutex.instance->release();
	}
};

std::queue<request> QueueManager::requests = std::queue<request>();
std::queue<response> QueueManager::responses = std::queue<response>();
MyMutex QueueManager::_pushMutex = MyMutex();
