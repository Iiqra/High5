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
//class MyServiceHandler; //forward declaration
//
//typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
//typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;
//
//char id = 'A';
//
//class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
//	
//	ACE_thread_t thread_names[2];
//
//public:
//	int open(void*) {
//
//		__peer = peer();
//     	Connection con(id++, &__peer, std::string(""));
//		ClientManager::addconnection(con);
//		
//		std::stringstream groupUsers;
//#pragma region  groupLogic
//		if (id % 2== 0) {
//		
//			for (auto g : GroupManager::groups) {
//				if (g.name == "g0even") {
//       				 g.connections.push_back(con);
//					// auto even = g.connections.data;
//					 break;
//				}
//			}
//		}
//		else {
//			for (auto g : GroupManager::groups) {
//				if (g.name == "g00odd") {
//					g.connections.push_back(con);
//					//break;
//				}
//				for (auto i = g.connections.begin(); i != g.connections.end(); i++)
//				{
//					std::cout << i->id << std::endl;
//				}
//			}
//		}
//		
//		//for (const auto i : GroupManager::groups)
//			//std::copy(groups[i].begin(), path.end(), std::ostream_iterator<char>(std::cout, " "));
//
//
//#pragma endregion 	
//		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
//		activate(THR_NEW_LWP,2, 1, ACE_DEFAULT_THREAD_PRIORITY,-1,this,0, 0,0,thread_names);
//		return 0;
//	}
//
//	void read(void) {
//
//		char type[2] = { 0 };
//		request r; //int _type=0;
//		type[1] = '\0';
//		int _type;
//		__peer.recv_n(type, 1);   // type= 1
//		if (type[0] != '1' && type[0] != '2' && type[0] != '3') {
//			__peer.send_n("Invalid Specifier", 17);
//			return;	}
//
//		_type = std::stoi(type);
//
//		if (_type == 3) {
//			char input;
//			__peer.recv_n(&input, 1);
//		if (input == 'A' || input == 'B' || input == 'C'|| input == 'D' || input == 'E') {
//				for (auto con : ClientManager::connections) {
//					if (con.id == (int)input) {
//						//con.socket->send_n("HELLO FROM CLIENT", sizeof("HELLO FROM CLIENT"));
//						char buff[5] = {0};
//						/*printf("enter:_\n");*/
//						//ACE_OS::read(ACE_STDIN, buff, sizeof(buff));
//						__peer.recv_n(buff ,5);
//						con.socket->send_n(buff, 5);
//						break;
//					}
//				}
//			}
//			else {
//				__peer.send_n("Client offline ", sizeof("Client offline "));
//			}
//		}		
//	}
//	int write(void) {
//		if (readytowrite) {
//			// WRite here
//			 //buffer.clear();
//			__peer.send_n(buffer.c_str(), buffer.length());
//			readytowrite = false;
//		}
//		return 0;
//	}
//
//	int svc(void) {
//		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
//		if (ACE_Thread::self() == thread_names[0])
//			while (1) { 
//				read(); 
//		    ACE_OS::sleep(10);
//		}
//		else
//			while (1) { 
//				write(); 
//		ACE_OS::sleep(10);
//		}
//		return 0; // keep the compiler happy.
//	}
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
//	bool readytowrite = false;
//	std::string buffer;
//};
//
//int main(int argc, char* argv[]) {
//	
//	//std::vector<char> path;
//	//for (int ch = 'a'; ch <= 'z'; ++ch)
//	//	path.push_back(ch);
//
//	///* Print path vector to console */
//	//std::copy(path.begin(), path.end(), std::ostream_iterator<char>(std::cout, " "));
//
//	//return 0;
//	ACE_INET_Addr addr(50009);
//	GroupManager::addgroups();
//	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main \n"));
//	//Prepare to accept connections
//	Acceptor myacceptor(addr, Reactor::instance());
//	// wait for something to happen.
//	while (1)
//		Reactor::instance()->handle_events();
//	return 0;
//}