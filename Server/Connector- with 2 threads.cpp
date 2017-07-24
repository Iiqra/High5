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
//
//class MyServiceHandler; //forward declaration
//
//typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
//typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;
//
//char id = 'A';
//
//class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
//	// The two thread names are kept here
//	ACE_thread_t thread_names[2];
//
//
//public:
//	int open(void*) {
//		__peer = peer();
//		Connection con(id++, &__peer, std::string(""));
//
//		ClientManager::addconnection(con);
//		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
//
//		activate(THR_NEW_LWP,
//			2, //2 new threads
//			1, //force active false, if already created don’t try again.
//			ACE_DEFAULT_THREAD_PRIORITY,//Use default thread priority
//			-1,
//			this,//Which ACE_Task object to create? In this case this one.
//			0,// don’t care about thread handles used
//			0,// don’t care about where stacks are created
//			0,//don’t care about stack sizes
//			thread_names); // keep identifiers in thread_names
//						   
//		//keep the service handler registered with the acceptor.
//		return 0;
//	}
//
//	void read(void) {
//		
//		
//		char input;
//		__peer.recv_n(&input, 1);
//		printpeer(__peer);
//
//		if (input == 'A' || input == 'B' || input == 'C') {
//			for (auto con : ClientManager::connections) {
//				if (con.id == (int)input) {
//					con.socket->send_n("Hello from other side ", sizeof("Hello from other side "));
//					break;
//				}
//			}
//		}
//		else {
//			__peer.send_n("Client offline ", sizeof("Client offline "));
//		}
//	} 
//
//	int write(void) {
//		return 0;
//	}
//
//	int svc(void) {
//		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
//		if (ACE_Thread::self() == thread_names[0])
//			while (1) read();
//		else
//			while (1) write();
//		return 0; // keep the compiler happy.
//	}
//
//
//	void printpeer(ACE_SOCK_Stream& p) { 
//		ACE_TCHAR peer_name[MAXHOSTNAMELEN];
//		ACE_INET_Addr peer_addr;
//		p.get_remote_addr(peer_addr);
//		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN);
//
//		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread "));
//		std::cout << "Accepting input from socket " << peer_name << std::endl;
//	}
//
//private:
//	ACE_SOCK_Stream __peer;
//};
//
//int main(int argc, char* argv[]) {
//	ACE_INET_Addr addr(50009);
//	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main \n"));
//	//Prepare to accept connections
//	Acceptor myacceptor(addr, Reactor::instance());
//	// wait for something to happen.
//	while (1)
//		Reactor::instance()->handle_events();
//	return 0;
//}