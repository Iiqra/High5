#pragma once
#include <queue>
#include "ace/Thread_Mutex.h" //3u00002u000030009hdfnsdgjb // 2- u00002- mesage sent  3- u0003 hello
#include <mutex>   // 3
//#include "msgprotocol.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\15August\unamanaged\myprotocol.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\15August\unamanaged\myprotocol.cpp"


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
