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

typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;

char id = 'A';

class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {

	ACE_thread_t thread_names[1]; // [1] and we spawned another thared instead of utilizing that previous spawned write thread

public:
	MyServiceHandler() {
		// A separate thread that keeps writing.
		ACE_Thread::spawn((ACE_THR_FUNC)write);
	}

	int open(void*) {
		__peer = peer();
		Connection con(id++, &__peer, std::string("")); 
		ClientManager::addconnection(con);
		/*	Connection con(id++, &__peer, std::string(""));

		ClientManager::addconnection(con);
		*/
		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));

		activate(THR_NEW_LWP,
			1, // 1 thread for reading.
			1, //force active false, if already created don’t try again.
			ACE_DEFAULT_THREAD_PRIORITY,//Use default thread priority
			-1,
			this,//Which ACE_Task object to create? In this case this one.
			0,// don’t care about thread handles used
			0,// don’t care about where stacks are created
			0,//don’t care about stack sizes
			thread_names); // keep identifiers in thread_names
						   //keep the service handler registered with the acceptor.
		return 0;
	}

	void read(void) {

		char type[2] = { 0 };
		request r; //int _type=0;
		type[1] = '\0';
		int _type;
		//
		__peer.recv_n(type, 1);   // type= 1
		if (type[0] != '1' && type[0] != '2' && type[0] != '3') {
			__peer.send_n("Invalid Specifier", 17);

			return;
		}

		_type = std::stoi(type);
		if (_type == 1 || _type == 2) {
			puts("|Authenication Request|");

			// Register	
			r.buffer = new char[20];
			//	r.buffer[21] = '\0';
			__peer.recv_n(r.buffer, 20); // username01password01
#pragma region parsingUsernamePassword
			char username[10];
			char password[10];
			int bIndex = 0, vIndex = 0;
			username[vIndex++] = r.buffer[bIndex++];// 0-1
			username[vIndex++] = r.buffer[bIndex++];// 1-2
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];
			username[vIndex++] = r.buffer[bIndex++];// 9-10
													//	username[vIndex++] = '\0';
			vIndex = 0;
			password[vIndex++] = r.buffer[bIndex++];// 0-11
			password[vIndex++] = r.buffer[bIndex++];// 1-12
			password[vIndex++] = r.buffer[bIndex++];// 2- 13
			password[vIndex++] = r.buffer[bIndex++];
			password[vIndex++] = r.buffer[bIndex++];
			password[vIndex++] = r.buffer[bIndex++];
			password[vIndex++] = r.buffer[bIndex++];
			password[vIndex++] = r.buffer[bIndex++];
			password[vIndex++] = r.buffer[bIndex++];
			password[vIndex++] = r.buffer[bIndex++]; // 9- 20
													 //	password[vIndex++] = '\0';
													 /*r.buffer = new char[21];
													 r.buffer[21] = '\0';*/

			char u[11];
			char p[11];
			bIndex = 0, vIndex = 0;
			u[vIndex++] = username[bIndex++];// 0-11
			u[vIndex++] = username[bIndex++];// 1-12
			u[vIndex++] = username[bIndex++];// 2- 13
			u[vIndex++] = username[bIndex++];
			u[vIndex++] = username[bIndex++];
			u[vIndex++] = username[bIndex++];
			u[vIndex++] = username[bIndex++];
			u[vIndex++] = username[bIndex++];
			u[vIndex++] = username[bIndex++];
			u[vIndex++] = username[bIndex++]; // 9- 20
			u[vIndex++] = '\0';

			vIndex = 0; bIndex = 0;
			p[vIndex++] = password[bIndex++];// 0-1
			p[vIndex++] = password[bIndex++];// 1-2
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];
			p[vIndex++] = password[bIndex++];// 9-10
			p[vIndex++] = '\0';
			vIndex = 0;

#pragma endregion

			userauthenticationstatus accountSts;
			response res;

			if (_type == 1) {
				accountSts = UserManager::registerUser(std::string(u), std::string(p));

				if (accountSts != userauthenticationstatus::OK) {
					// Something went wrong.
					// Return invalid response
					__peer.send_n("Cannot create account", sizeof("Cannot create account"));
				}
			}

			// Login
			accountSts = UserManager::authenticateUser(std::string(u), std::string(p));
			if (accountSts == userauthenticationstatus::OK) {
				Connection con(id, &this->peer_, UserManager::getsenderId(u));
				ClientManager::addconnection(con);

				if (id % 2 == 0) {
					for (auto g : GroupManager::groups) {
						if (g.name == "g0even") {
							g.connections.push_back(con);
							break;
						}
					}
				}
				else {
					for (auto g : GroupManager::groups) {
						if (g.name == "g00odd") {
							g.connections.push_back(con);
							break;
						}
					}
				}

				id++;
				// Logged in
				res.type = 1;
				std::string userId = UserManager::getsenderId(u);
				res.buffer = (char*)userId.c_str();
				res.length = "0006";

				std::string buff = responsehelper::parseresponse(res);
				__peer.send_n(buff.c_str(), buff.length());
			}
			else if (accountSts == userauthenticationstatus::UsernamePasswordMismatch ||
				accountSts == userauthenticationstatus::UserNotfound) {
				// User not found or password mismatch
				res.type = 1;
				res.buffer = "Cannot login";
				res.length = "0012";

				std::string buff = responsehelper::parseresponse(res);
				__peer.send_n(buff.c_str(), buff.length());
			}

		}
		else if (_type == 3) {
			char input;
			__peer.recv_n(&input, 1);
			if (input == 'A' || input == 'B' || input == 'C' || input == 'D' || input == 'E') {
				for (auto con : ClientManager::connections) {
					if (con.id == (int)input) {
						char buff[5] = { 0 };
						__peer.recv_n(buff, 5);

						response forward;
						forward.type = 10;
						forward.length = "0005";
						forward.buffer = new char[5];
						forward.buffer = buff;
						forward.socket = con.socket;
						QueueManager::responses.push(forward);

						response resp;
						resp.type = 5;
						resp.length = "0004";
						resp.buffer = "Okay";
						resp.socket = &__peer;
						QueueManager::responses.push(resp);
						printf("QueueManager has %d responses.\n", QueueManager::responses.size());
						break;
					}
				}
			}
			else if (input == 'g') {
				// Send to a group
				auto members = GroupManager::getuserlist("g0food");
			}
			else if (input == 'u') {
				// Send to a user

			}
			else {
				response res;
				res.length = "0016";
				res.buffer = "Client offline ";
				res.socket = &__peer;
				res.type = 5;
				QueueManager::responses.push(res);
			}
		}
	}

	// Write function follows static semantics; can be used in the thread spawning.
	static void write(void) {
		while (1) {
			// Continue to loop
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

			//Pause

		}
	}

	int svc(void) {
		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
		if (ACE_Thread::self() == thread_names[0]) {
			while (1) {
				read();
			}
		}
			//while (1) {
			//	/*write();
			//	ACE_OS::sleep(10);*/
			//}
		return 0; // keep the compiler happy.
	}

	void printpeer(ACE_SOCK_Stream& p) {
		ACE_TCHAR peer_name[MAXHOSTNAMELEN];
		ACE_INET_Addr peer_addr;
		p.get_remote_addr(peer_addr);
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN);

		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread "));
		std::cout << "Accepting input from socket " << peer_name << std::endl;
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
	// wait for something to happen.
	while (1)
		Reactor::instance()->handle_events();
	return 0;
}