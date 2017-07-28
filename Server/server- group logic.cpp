#include "ace/Reactor.h"
#include <iostream>
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include <string>
#include <cctype>
#include "ace/OS.h"
#include "ClientManager.h"
#include "MessageProtocol.h"
#include "QueueManager.h"

class MyServiceHandler; //forward declaration
ACE_Thread_Mutex writeMutex;

typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;

char id = 'A'; std::string ids= "A";

class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {

	ACE_thread_t thread_names[1];

public:
	MyServiceHandler() {
		// A separate thread that keeps writing. launches only when server is started.
		// ACE_Thread::spawn((ACE_THR_FUNC)write);
		// Load the groups at the time server gets started.
	}

	int open(void*) {
		__peer = peer();
		Connection con(id, &__peer, std::string(&id));
		ClientManager::addconnection(con);

		if (id % 2 == 0) {
			GroupManager::addconnection("g0even", con);
		}
		else {
			GroupManager::addconnection("g00odd", con);
		}

		id++;
		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
		activate(THR_NEW_LWP,
			1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this, 0, 0, 0, thread_names);
		return 0;
	}

	void read(void) {

		char type[2] = { 0 };
		request r; //int _type=0;
		type[1] = '\0';
		int _type;
		__peer.recv_n(type, 1);   // type= 1
		if (type[0] != '1' && type[0] != '2' && type[0] != '3' && type[0] != '4') {
			__peer.send_n("Invalid Specifier", 17);
			return;
		}
		_type = std::stoi(type);
		
	      if (_type == 4) {
			//give required user list to the requested client
			//	std::string groupName;
			char groupName[7];
			groupName[6] = '\0';
			__peer.recv_n(groupName, 6);

			std::string members = GroupManager::getuserlist(groupName);
			response res;
			res.type = 0;
			res.buffer = (char*)members.c_str();
			std::stringstream length;
			length << std::setw(4) << std::setfill('0') << members.length();
			res.length = (char*)length.str().c_str();

			std::string respParsed = responsehelper::parseresponse(res);
			__peer.send_n(respParsed.c_str(), respParsed.length());
			_write(1, members.c_str(), sizeof(members));
		}
	}

	// Write function follows static semantics; can be used in the thread spawning.
	static void write(void) {
		while (1) {
			// Continue to loop
			writeMutex.acquire();
			bool isEmpty = QueueManager::responses.empty();
			if (!isEmpty) {
				// Take the first response
				auto res = QueueManager::responses.front();
				QueueManager::responses.pop(); // Remove it.

				std::string responseBuffer = responsehelper::parseresponse(res);

				// But this function creates another problem
				// It sends all the data, synch.
				res.socket->send_n(responseBuffer.c_str(), responseBuffer.length());

				ACE_DEBUG((LM_DEBUG, "(%t) QueueThread \n"));
			}

			writeMutex.release();
			//Pause
			// ACE_OS::sleep(25); Questionable now.
		}
	}

	int svc(void) {
		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
		if (ACE_Thread::self() == thread_names[0]) {
			while (1) {
				read();
			}
		}
	
		return 0; // keep the compiler happy.
	}


private:
	ACE_SOCK_Stream __peer;
	bool readytowrite = false;
	std::string buffer;
};

int main(int argc, char* argv[]) {
	ACE_INET_Addr addr(50009);
	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main \n"));
	//Prepare to accept connections
	Acceptor myacceptor(addr, Reactor::instance());

	ACE_Thread::spawn((ACE_THR_FUNC)MyServiceHandler::write);
	GroupManager::addgroups();

	// wait for something to happen.
	while (1)
		Reactor::instance()->handle_events();
	return 0;
}