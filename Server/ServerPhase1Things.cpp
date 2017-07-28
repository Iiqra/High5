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
//class MyServiceHandler; 
//ACE_Thread_Mutex writeMutex;
//char id = 'A'; std::string ids = "A";
//typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
//typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;
//
//class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
//	ACE_thread_t thread_names[1];
//public:
//	MyServiceHandler() {
//	}
//
//	int open(void*) {
//		__peer = peer();
//		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
//		activate(THR_NEW_LWP,1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this, 0, 0, 0, thread_names);
//		return 0;
//	}
//
//	void read(void) {
//		char type[2] = { 0 };
//		request r;
//		userauthenticationstatus accountSts;
//		response res;
//		type[1] = '\0';
//		int _type;
//		__peer.recv_n(type, 1);   // type= 1
//		if (type[0] != '1' && type[0] != '2' && type[0] != '3' && type[0] != '4') {
//			__peer.send_n("Invalid Specifier", 17);
//			return;}
//		_type = std::stoi(type);
//	
//		if (_type == 1) {
//
//			while(accountSts != userauthenticationstatus::OK) {
//				accountSts = UserManager::uregister(__peer, r, res);
//				if (accountSts != userauthenticationstatus::OK)
//			__peer.send_n("Alreadey Exist, Register again", sizeof("Alreadey Exist, Register again"));
//			}
//			__peer.send_n("Successful Login", sizeof("Successful Login"));
//	
//		} // type1 ends 
//
//		else if (_type == 2) {
//		accountSts = UserManager::ulogin(__peer, r , res);
//		else if (accountSts == userauthenticationstatus::OK) {
//			Connection con(id++, &__peer, UserManager::getsenderId(u));
//			ClientManager::addconnection(con);
//			// select groups
//			char addGroup[2] = { 0 };
//			addGroup[1] = '\0';
//			__peer.send_n("Currently we've 2 active groups.\n\rt- TechTalks \r\nf-Foodbar", sizeof("Currently we've 2 active groups\n 1- TechTalks 2-Foodbar"));
//			__peer.recv_n(addGroup, 1);
//			if (addGroup[0] == 't')
//			{
//				GroupManager::addconnection("g1tech", con);
//			}
//			else if (addGroup[0] == 'f')
//			{
//				GroupManager::addconnection("g2food", con);
//			}
//
//			//else  user dont want to be the part of any group
//			// call login directly with no group list added.
//
//			// Logged in
//			res.type = 1;
//			std::string userId = UserManager::getsenderId(u);
//			res.buffer = (char*)userId.c_str();
//			res.length = "0006";
//
//			std::string buff = responsehelper::parseresponse(res);
//			__peer.send_n(buff.c_str(), buff.length());
//		}
//	}
//	else if (accountSts == userauthenticationstatus::UsernamePasswordMismatch ||
//		accountSts == userauthenticationstatus::UserNotfound) {
//				// User not found or password mismatch
//				res.type = 1;
//				res.buffer = "Cannot login";
//				res.length = "0012";
//
//				std::string buff = responsehelper::parseresponse(res);
//				__peer.send_n(buff.c_str(), buff.length());
//			}
//		}
//		else if (_type == 3) {
//			//message
//			char input;
//			__peer.recv_n(&input, 1);
//			if (input == 'A' || input == 'B' || input == 'C' || input == 'D' || input == 'E') {
//				for (auto con : ClientManager::connections) {
//					if (con.id == (int)input) {
//						char buff[5] = { 0 };
//						__peer.recv_n(buff, 5);
//
//					response forward;
//					forward.type = 10;
//					forward.length = "0005";
//					forward.buffer = new char[5];
//					forward.buffer = buff;
//					forward.socket = con.socket;
//					QueueManager::addresponse(forward);
//					
//					response resp;
//					resp.type = 5;
//					resp.length = "0004";
//					resp.buffer = "Okay";
//					resp.socket = &__peer;
//					QueueManager::addresponse(resp);
//					printf("QueueManager has %d responses.\n", QueueManager::responses.size());
//					break;
//					}
//				}
//			}
//			else if (input == 'g') {
//				// Send to a group
//				//auto members = GroupManager::getuserlist("g0food");
//			}
//			else if (input == 'u') {
//				// Send to a user
//
//			}
//			else {
//				response res;
//				res.length = "0016";
//				res.buffer = "Client offline ";
//				res.socket = &__peer;
//				res.type = 5;
//				QueueManager::addresponse(res);
//			}
//
//		}
//		else if (_type == 4) {
//			char groupName[7];
//			groupName[6] = '\0';
//			__peer.recv_n(groupName, 6);
//
//			std::string members = GroupManager::getuserlist(groupName);
//			response res;
//			res.type = 0;
//			res.buffer = (char*)members.c_str();
//			std::stringstream length;
//			length << std::setw(4) << std::setfill('0') << members.length();
//			res.length = (char*)length.str().c_str();
//
//			std::string respParsed = responsehelper::parseresponse(res);
//			__peer.send_n(respParsed.c_str(), respParsed.length());
//			_write(1, members.c_str(), sizeof(members));
//		}
//	}
//
//	static void write(void) {
//		while (1) {
//			writeMutex.acquire();
//			bool isEmpty = QueueManager::responses.empty();
//			if (!isEmpty) {
//				// Take the first response
//				auto res = QueueManager::responses.front();
//				QueueManager::responses.pop(); // Remove it.
//				std::string responseBuffer = responsehelper::parseresponse(res);
//				// But this function creates another problem
//				// It sends all the data, synch.
//				res.socket->send_n(responseBuffer.c_str(), responseBuffer.length());
//
//				ACE_DEBUG((LM_DEBUG, "(%t) QueueThread \n"));
//			}
//
//			writeMutex.release();
//			//Pause
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