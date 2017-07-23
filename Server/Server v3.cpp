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
class MyServiceHandler; //forward declaration

typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;

char id = 'A';

class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
	// The two thread names are kept here
	ACE_thread_t thread_names[2];

public:
	int open(void*) {
		__peer = peer();
	/*	Connection con(id++, &__peer, std::string(""));

		ClientManager::addconnection(con);
	*/	
		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));

		activate(THR_NEW_LWP,
			2, //2 new threads
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
		__peer.recv_n(type, 1);
		//if (type != int('1') && type != '2' && type != '3')
		if (type[0] != '1' && type[0] != '2' && type[0] != '3') {
			__peer.send_n("Invalid Cpecifier", 17);

			return;
		}
		_type = std::stoi(type);
		if (_type == 1) {
			printf("|Register Request|");
			r.type = MessageType::Register;
		}
		else if (_type == 2)
		{
			printf("|Login Request|");
			r.type = MessageType::Login;
		}
		if (_type == 1 || _type == 2) {
			// Register	
			r.buffer = new char[21];
			r.buffer[21] = '\0';
			__peer.recv_n(r.buffer, 20); // u12345
			char username[11];
			char password[11];
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
			username[vIndex++] = '\0';
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
			password[vIndex++] = '\0';
			userauthenticationstatus accountSts;
			response res;

			if (_type == 1) { // register

				accountSts = UserManager::registerUser(std::string(username), std::string(password));
				if (accountSts == userauthenticationstatus::OK) {
					// Login
					accountSts = UserManager::authenticateUser(std::string(username), std::string(password));
					if (accountSts == userauthenticationstatus::OK) {
						Connection con(id, &this->peer_, UserManager::getsenderId(username));
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
						// Send OK response
						res.type = 1;
						std::string userId = UserManager::getsenderId(username);
						res.buffer = (char*)userId.c_str();
						res.length = "0006";
						std::string responseStr = responsehelper::parseresponse(res);
						
						readytowrite = true;
						buffer = responseStr;
						//this->peer_.send_n(responseStr.c_str(), responseStr.size());
						//_write(1, responseStr.c_str(), responseStr.size());
						// Send info to the client
						if (ClientManager::connections.size() > 0) {
							std::stringstream clientBuffer;
							clientBuffer << "Active clients: ";
							for (auto con : ClientManager::connections) {
								if (con.userid != res.buffer)
									clientBuffer << con.userid << "|";
							}
							std::string _buf = clientBuffer.str();
							std::stringstream bufferStream;

							bufferStream << 9 << std::setw(4) << std::setfill('0') << _buf.size() << _buf;

							readytowrite = true;
							buffer = responseStr;
							// this->peer_.send_n(bufferStream.str().c_str(), bufferStream.str().size());
							//_write(1, bufferStream.str().c_str(), bufferStream.str().size());
						}
					}
				}
				else {
					// Send cannot login response
					res.type = 2;
					std::string userId = UserManager::getsenderId(username);
					res.buffer = "Cannot login";
					res.length = "0012";

					std::string responseStr = responsehelper::parseresponse(res);
				

					readytowrite = true;
					buffer = responseStr;
				//	this->peer_.send_n(responseStr.c_str(), responseStr.size());// lets suppose 20 - 1 type+ 4 length+ rest message buffer
				//	_write(1, responseStr.c_str(), responseStr.size());
				//
				}
			}
			else if (_type == 2) { // login
				accountSts = UserManager::authenticateUser(std::string(username), std::string(password));
				if (accountSts == userauthenticationstatus::OK) {
					Connection con(id, &this->peer_, UserManager::getsenderId(username));
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
					std::string userId = UserManager::getsenderId(username);
					res.buffer = (char*)userId.c_str();
					res.length = "0006";

					std::string responseStr = responsehelper::parseresponse(res);
					this->peer_.send_n(responseStr.c_str(), responseStr.size());
					_write(1, responseStr.c_str(), responseStr.size());
					// Send info to the client
					if (ClientManager::connections.size() > 0) {
						std::stringstream clientBuffer;
						clientBuffer << "Active clients: ";
						for (auto con : ClientManager::connections) {
							clientBuffer << con.userid << "|";
						}
						std::string _buf = clientBuffer.str();
						std::stringstream bufferStream;

						bufferStream << 9 << std::setw(4) << std::setfill('0') << _buf.size() << _buf;
					

						readytowrite = true;
						buffer = bufferStream.str();
						// this->peer_.send_n(bufferStream.str().c_str(), bufferStream.str().size());
						//_write(1, bufferStream.str().c_str(), bufferStream.str().size());
					}
				}
				else if (accountSts == userauthenticationstatus::UsernamePasswordMismatch ||
					accountSts == userauthenticationstatus::UserNotfound) {
					// User not found or password mismatch
					res.type = 1;
					res.buffer = "Cannot login";
					res.length = "0012";

					std::string responseStr = responsehelper::parseresponse(res);
					this->peer_.send_n(responseStr.c_str(), responseStr.size());
					_write(1, responseStr.c_str(), responseStr.size());
				}
			}
		}
		else if (_type == 3) {
			char* initials = new char[17];
			initials[17] = '\0';
			__peer.recv_n(initials, 16);
			requesthelper::parseheader(initials, r);
			if (r.length == 0) {
				// There was something wrong.

				this->peer_.send_n("Invalid Length", 15); // 00t6
				_write(1, "Invalid Length", 15);
			}
			else {
				r.buffer = new char[r.length];
				r.buffer[r.length++];
				//r.buffer = { 0 };
				//int l = r.length;
				__peer.recv_n(r.buffer, r.length);
				RequestStatus rStatus = requesthelper::validate_request(r);
				// Send the message
				std::string rec = std::string(r.recipient);
				rec = rec.substr(0, 6);
				if (rec == "BRDCST") {
					// BRDCST
					for (auto con : ClientManager::connections) {
						// We need to send it to all, go insane! :D 
						con.socket->send_n(r.buffer, r.length);
						_write(1, r.buffer, r.length);
					}

					// Send Okay, even if not one was there... We don't need to know who got the message, there was no list. 
					response resp;
					resp.type = _type;
					resp.length = "0004";
					resp.buffer = "Okay";

					std::string strRes = responsehelper::parseresponse(resp);
					this->peer_.send_n(strRes.c_str(), strRes.size());
					_write(1, strRes.c_str(), strRes.size());
				}
				else if (rec[0] == 'g') {
					// Group message
					// Send Okay, even if not one was there... We don't need to know who got the message, there was no list. 
					response resp;
					resp.type = _type;
					resp.length = "0015";
					resp.buffer = "Group not found";

					for (auto g : GroupManager::groups) {
						if (g.name == rec) {
							for (auto con : g.connections) {
								con.socket->send_n(r.buffer, r.length);
								_write(1, r.buffer, r.length);
							}

							// Send Okay
							resp.type = _type;
							resp.length = "0004";
							resp.buffer = "Okay";
							break;
						}
					}

					std::string strRes = responsehelper::parseresponse(resp);


					readytowrite = true;
					buffer = strRes;
			/*		this->peer_.send_n(strRes.c_str(), strRes.size());
					_write(1, strRes.c_str(), strRes.size());
			*/	}
				else {

					response resp;
					resp.type = _type;
					resp.length = "0015";
					resp.buffer = "Client Offline";

					for (auto con : ClientManager::connections) {
						if (con.userid == rec) {
							con.socket->send_n(r.buffer, r.length);
							_write(1, r.buffer, r.length);
							// Send Okay
							response resp;
							resp.type = _type;
							resp.length = "0004";
							resp.buffer = "Okay";

							break;
						}
					}
					std::string strRes = responsehelper::parseresponse(resp);
					this->peer_.send_n(strRes.c_str(), strRes.size());
					_write(1, strRes.c_str(), strRes.size());
				}

				char input;
				__peer.recv_n(&input, 1);
				printpeer(__peer);

				if (input == 'A' || input == 'B' || input == 'C') {
					for (auto con : ClientManager::connections) {
						if (con.id == (int)input) {
							con.socket->send_n("Hello from other side ", sizeof("Hello from other side "));
							break;
						}
					}
				}
				else {
					__peer.send_n("Client offline ", sizeof("Client offline "));
				}
			}
		}
	}

	int write(void) {
		if (readytowrite) {
			// WRite here
			__peer.send_n(buffer.c_str(), buffer.length());
			
			buffer.clear();
			readytowrite = false;
		}
		return 0;
	}

	int svc(void) {
		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
		if (ACE_Thread::self() == thread_names[0])
			while (1) read();
		else
			while (1) write();
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