#pragma once
#include <queue>
#include "MessageProtocol.h"
#include "ace/Thread_Mutex.h"

//typedef ACE_Singleton<ACE_Mutex, ACE_Null_Mutex> MyMutex;

class QueueManager {
public:
	// MyMutex _pushMutex; 
	static ACE_Thread_Mutex _pushMutex;
	static std::queue<request> requests;   // <- Everything read.
	static std::queue<response> responses; // <- Everything to write.

public:

	//static void getresponse(response& r) 
	//{
	//	r = responses.front();
	//	responses.pop();
	//}
	/*static void addresponse(response& r) {


	}*/
	static void addresponse(response& r) {
		//while (_pushMutex.instance->tryacquire() == -1) { /* wait(10); */ }
		//// _pushMutex.acquire();
		_pushMutex.acquire();
		responses.push(r);

		_pushMutex.release();
		/*_pushMutex.instance->release();*/
	}
};

ACE_Thread_Mutex QueueManager::_pushMutex;

std::queue<request> QueueManager::requests = std::queue<request>();
std::queue<response> QueueManager::responses = std::queue<response>();
////MyMutex QueueManager::_pushMutex = MyMutex();
////ACE_Thread_Mutex  QueueManager::_mutex = ACE_Thread_Mutex();
//
//
///*ACE_Thread_Mutex& QueueManager::_mutex() {
////	static ACE_Thread_Mutex _mutexObject = ACE_Thread_Mutex();
////static ACE_Thread_Mutex _mutex();
////	return _mutexObject;
//}*/
//
////#include <queue>
////#include "MessageProtocol.h"
////#include "ace/Thread_Mutex.h"
////
////class QueueManager {
////public:
////	static ACE_Thread_Mutex _pushMutex;
////	static std::queue<request> requests;   // <- Everything read.
////	static std::queue<response> responses; // <- Everything to write.
////
////public:
////
////	static void getresponse(response& r) {
////
////	}
////	static void addresponse(response& r) {
////		_pushMutex.acquire();
////			responses.push(r);
////	   _pushMutex.release();
////	}
////};
////std::queue<request> QueueManager::requests = std::queue<request>();
////std::queue<response> QueueManager::responses = std::queue<response>();
////
//
//
//
//#pragma once
//#include <queue>
//#include "MessageProtocol.h"
//#include "ace/Thread_Mutex.h"
//class QueueManager {
//public:
//	static ACE_Thread_Mutex _pushMutex;
//	static std::queue<int> requests;   // <- Everything read.
//	static std::queue<int> responses; // <- Everything to write.
//	static void addresponse() {
//		int r;
//		_pushMutex.acquire();
//		responses.push(r);
//		_pushMutex.release();
//	}
//public:
//};
//ACE_Thread_Mutex QueueManager::_pushMutex{};
//std::queue<int> QueueManager::requests = std::queue<int>();
//std::queue<int> QueueManager::responses = std::queue<int>();
//
