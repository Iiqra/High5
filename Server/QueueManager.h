#pragma once
#include <queue>
#include "ace/Thread_Mutex.h"
#include <mutex>
#include "Header.h"

//typedef ACE_Singleton<ACE_Mutex, ACE_Null_Mutex> MyMutex;

class QueueManager {
public:
	static std::mutex _pushMutex;
	static std::queue<request> requests;   // <- Everything read.
	static std::queue<response> responses; // <- Everything to write.

public:
	static int getresponse(response& r) {
		if (responses.empty()) { return 0; }
		r = responses.front();
		int size = responses.size();
		//if (!responses.empty())
		responses.pop();
		return 1;
	}

	static void addresponse(response& r) {
		_pushMutex.lock();
		responses.push(r);
		int size = responses.size();
		_pushMutex.unlock();
	}
};

std::queue<request> QueueManager::requests = std::queue<request>();
std::queue<response> QueueManager::responses = std::queue<response>();
std::mutex QueueManager::_pushMutex;
