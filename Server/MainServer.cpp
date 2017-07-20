#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include "MessageProtocol.h"
#include "User.h"
#include "ace/OS.h"
#include <cctype>

int id = (int) 'A'; // Starting from A, then id++ --> 'B'

typedef ACE_SOCK_Acceptor Acceptor;
class Accept_Handler;
class Service_Handler : public ACE_Event_Handler {
public:
		int handle_input(ACE_HANDLE handle) override {
	
			response res1;
			if (ClientManager::connections.size() > 0) {
				std::stringstream clientBuffer;
			//	clientBuffer << "Active clients: ";
				for (auto con : ClientManager::connections) {
					if (con.userid != res1.buffer)
						clientBuffer << con.userid << "|";
				}
				this->peer_.send_n(clientBuffer.str().c_str(), clientBuffer.str().size());
				_write(1, clientBuffer.str().c_str(), clientBuffer.str().size());
			}

		char type[2] = { 0 };
		request r; //int _type=0;
		type[1] = '\0'; int _type;
		get_stream().recv_n(type, 1);
		//if (type != int('1') && type != '2' && type != '3')
		 _type = std::stoi(type);
		if (_type != 1 && _type != 2 && _type != 3 )
		{
			get_stream().send_n("Invalid Specifier", 17);
			
		}
		if (_type == 1){
			printf("|Register Request|");
			r.type = MessageType::Register;
		}
		else if (_type == 2)
		{
			printf("|Login Request|");
			r.type = MessageType::Login;
		}
		if (_type==1 || _type==2) {
			// Register	
			r.buffer = new char[21];
			r.buffer[21] = '\0';
			bytereceived= get_stream().recv_n(r.buffer, 20); // u12345
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
						Connection con(id++, &this->peer_, UserManager::getsenderId(username));
						ClientManager::addconnection(con);
						// Send OK response
						res.type = 1;
						std::string userId = UserManager::getsenderId(username);
						res.buffer = (char*)userId.c_str();
						res.length = "0006";
						std::string responseStr = responsehelper::parseresponse(res);
						this->peer_.send_n(responseStr.c_str(), responseStr.size());

						// Send info to the client
						if (ClientManager::connections.size() > 0) {
							std::stringstream clientBuffer;
							clientBuffer << "Active clients: ";
							for (auto con : ClientManager::connections) {
								if (con.userid != res.buffer)
								clientBuffer << con.userid << "|";
							}
							this->peer_.send_n(clientBuffer.str().c_str(), clientBuffer.str().size());
							_write(1, clientBuffer.str().c_str(), clientBuffer.str().size());
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
					this->peer_.send_n(responseStr.c_str(), responseStr.size());
				}
			}
			else if (_type == 2) { // login
				accountSts = UserManager::authenticateUser(std::string(username), std::string(password));
				if (accountSts == userauthenticationstatus::OK) {
					Connection con(id++, &this->peer_, UserManager::getsenderId(username));
					ClientManager::addconnection(con);

					// Logged in
					res.type = 1;
					std::string userId = UserManager::getsenderId(username);
					res.buffer = (char*)userId.c_str();
					res.length = "0006";

					std::string responseStr = responsehelper::parseresponse(res);
					this->peer_.send_n(responseStr.c_str(), responseStr.size());

					// Send info to the client
					if (ClientManager::connections.size() > 0) {
						std::stringstream clientBuffer;
						clientBuffer << "Active clients: ";
						for (auto con : ClientManager::connections) {
							clientBuffer << con.userid << "|";
						}
						this->peer_.send_n(clientBuffer.str().c_str(), clientBuffer.str().size());
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
				}
			}
		}
		else if (_type == 3) {
			auto initials = new char[17];
			initials[17] = '\0';
			bytereceived = get_stream().recv_n(initials, 16);
			requesthelper::parseheader(initials, r);
			r.buffer = new char[r.length];
			r.buffer[r.length++];
			//r.buffer = { 0 };
			//int l = r.length;
			bytereceived = get_stream().recv_n(r.buffer, r.length);
			RequestStatus rStatus = requesthelper::validate_request(r);
			// Send the message
			std::string rec = std::string(r.recipient);
			for (auto con : ClientManager::connections) {
				if (con.userid == rec) {
					con.socket->send_n(r.buffer, r.length);
					_write(1,r.buffer, r.length);
					// Send Okay
					response resp;
					resp.type = _type;
					resp.length = "0004";
					resp.buffer = "Okay";

					std::string strRes = responsehelper::parseresponse(resp);
					this->peer_.send_n(strRes.c_str(), strRes.size());
					_write(1, strRes.c_str(), strRes.size());
				}
			}
			this->peer_.send_n("Client Offline", 14);
			_write(1, "Client Offline", 14);
			// requesthelper::process_message(r, rStatus, resp);
		}

		//delete[] r.buffer;
		//type[1] = 0;
		return 0;
	}
	ACE_HANDLE get_handle(void) const override
	{
		return peer_.get_handle();
	}
	ACE_SOCK_Stream &
	get_stream() {
		return this->peer_;
	}
private:
	ACE_SOCK_Stream peer_;
	
	int bytesent = 0, bytereceived = 0;
	
};
class Accept_Handler : public ACE_Event_Handler {
public:
	//Constructor
	Accept_Handler()
	{
		
	}
	explicit Accept_Handler(ACE_Addr &addr) {
		ACE_DEBUG((LM_DEBUG, "Listening......|.....|...\n"));

		this->open(addr);
	}
	int    open(ACE_Addr &addr) {
		peer_acceptor.open(addr);
		return 0;
	}
	int   handle_input(ACE_HANDLE ace_handle) override {
		Service_Handler *eh = new Service_Handler();
		ACE_TCHAR peer_name[MAXHOSTNAMELEN];
		ACE_INET_Addr peer_addr;
		
		if (this->peer_acceptor.accept(eh->get_stream(), // stream
			0, // remote address
			0, // timeout
			1) == -1) //restart if interrupted
		ACE_DEBUG((LM_ERROR, "Error in connection\n"));
		ACE_DEBUG((LM_DEBUG, "Connection established\n"));
		eh->get_stream().get_remote_addr(peer_addr);
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN);
		ACE_DEBUG((LM_DEBUG, "with %s \n", peer_name));
		ACE_Reactor::instance()->
			register_handler(eh, ACE_Event_Handler::READ_MASK);
		return 0;
	}
	//Used by the reactor to determine the underlying handle
	ACE_HANDLE
		get_handle(void) const override {
		return this->peer_acceptor.get_handle();
	}
private:
	Acceptor peer_acceptor;
};
int main(int argc, char * argv[]) {
	system("pause");
	ACE_INET_Addr addr(50009);
	Accept_Handler *eh = new Accept_Handler(addr);
	ACE_Reactor::instance()->register_handler(eh,
		ACE_Event_Handler::ACCEPT_MASK);
	while (1)
		ACE_Reactor::instance()->run_reactor_event_loop();
}


