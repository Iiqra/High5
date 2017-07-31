//#include "ace/Reactor.h"
//#include <iostream>
//#include "ace/Svc_Handler.h"
//#include "ace/Acceptor.h"
//#include "ace/Synch.h"
//#include "ace/SOCK_Acceptor.h"
//#include <string>
//#include <cctype>
//#include "ace/OS.h"
//#include "ClientManager.h"
//#include "MessageProtocol.h"
//#include "QueueManager.h"
//
//class MyServiceHandler; //forward declaration
//
//char id = 'A'; 
//std::string ids = "A";
//
//typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
//typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;
//
//class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
//ACE_thread_t thread_names[1];
//
//public:
//	MyServiceHandler() {
//		// A separate thread that keeps writing. launches only when server is started.
//		// ACE_Thread::spawn((ACE_THR_FUNC)write);
//		// Load the groups at the time server gets started.
//	}
//
//	int open(void*) {
//		__peer = peer();
//		Connection con(id, &__peer, std::string(&id));
//		ClientManager::addconnection(con);
//
//		if (id % 2 == 0) {
//			GroupManager::addconnection("gtecht", con);
//		}
//		else {
//			GroupManager::addconnection("gfubar", con);
//		}
//		id++;
//		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
//		activate(THR_NEW_LWP,
//			1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this, 0, 0, 0, thread_names);
//		return 0;
//	}
//
//	void read(void) {
//		char type[2] = { 0 };
//		request r; 
//		type[1] = '\0';
//		int _type;
//		__peer.recv_n(type, 1);   
//		if (type[0] != '1' && type[0] != '2' && type[0] != '3' && type[0] != '4') {
//			__peer.send_n("Invalid Specifier", 17);
//			return;
//		}
//		_type = std::stoi(type);
//		if (_type == 1 || _type == 2) {
//			// readBuffer(__peer, buf, 20);
//			char t;
//			__peer.recv_n(&t, 1);
//			std::vector<Connection> conVector;
//			if (t == 't') {
//				GroupManager::getconnections("gtecht", conVector);
//			}
//			else {
//				GroupManager::getconnections("gfubar", conVector);
//			}
//			__peer.send_n("Enter message to send to group: \n", 34);
//
//			char* msg = new char[5];
//			std::string  __response;
//			msg[5] = '\0';
//			__peer.recv_n(msg, 5);
//			for (auto _ : conVector) {
//			response res;
//			res.type = 5;
//			res.socket = _.socket;
//			res.buffer = msg;
//			res.length = "0005";
//				
//			__response =	responsehelper::parseresponse(res);
//			res.socket->send_n(__response.c_str(),sizeof(__response));
//				//QueueManager::addresponse(res);
//			}
//			return;
//		} else if (_type == 4) {
//			//give required user list to the requested client
//			//	std::string groupName;
//			char groupName[7];
//			groupName[6] = '\0';
//			__peer.recv_n(groupName, 6);
//
//			// readBuffer(__peer, groupName, 6);
//
//			std::string members = GroupManager::getuserlist(groupName);
//			response res;
//			res.type = 0;
//			res.buffer = (char*)members.c_str();
//			std::stringstream length;
//			length << std::setw(4) << std::setfill('0') << members.length();
//			res.length = (char*)length.str().c_str();
//
//			std::string respParsed = responsehelper::parseresponse(res, "", true);
//			__peer.send_n(respParsed.c_str(), respParsed.length());
//
//			// sendBuffer(__peer, respParsed.c_str(), respParsed.length());
//			_write(1, members.c_str(), sizeof(members));
//		} // type 4
//	}
//
//	static void write(void) {
//		while (1) {
//			// Continue to loop
//			response res;
//			
//			if (QueueManager::getresponse(res) == 1) {
//				res.type= (int)ResponseMessage::GroupMessage;
//				std::string responseBuffer = responsehelper::parseresponse(res);
//				res.socket->send_n(responseBuffer.c_str(), responseBuffer.length());
//				ACE_DEBUG((LM_DEBUG, "(%t) QueueThread \n"));
//			}
//			// ACE_OS::sleep(25); Questionable now.
//		}
//	}
//
//	int svc(void) {
//		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
//		if (ACE_Thread::self() == thread_names[0]) {
//			while (1) {
//				read();
//			}
//		}
//	
//		return 0; // keep the compiler happy.
//	}
//
//
//private:
//	ACE_SOCK_Stream __peer;
//	bool readytowrite = false;
//	std::string buffer;
//};
//
//int main(int argc, char* argv[]) {
//	ACE_INET_Addr addr(50009);
//	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main \n"));
//	//Prepare to accept connections
//	Acceptor myacceptor(addr, Reactor::instance());
//	ACE_Thread::spawn((ACE_THR_FUNC)MyServiceHandler::write);
//	GroupManager::addgroups();
//
//	// wait for something to happen.
//	while (1)
//		Reactor::instance()->handle_events();
//	return 0;
//}