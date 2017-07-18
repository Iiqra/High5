#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include "MessageProtocol.h"
#include "User.h"
#include "ace/OS.h"

#define SIZE 5
typedef ACE_SOCK_Acceptor Acceptor;
class Accept_Handler;
class Service_Handler : public ACE_Event_Handler {
public:
	Service_Handler() {
		//ACE_DEBUG((LM_DEBUG, "Input Handler Constructor\n"));
	}
	// 1000000xxxxxx5555deadbeef
	int handle_input(ACE_HANDLE handle) override {
		char type;
		request r;
		 get_stream().recv_n(&type, 1);
		int _type = std::stoi(&type);
		if (_type == 1) printf("|Register Request|");
		else if (_type == 2) printf("|Login Request|");
		if (_type == 1|| _type==2) {
			// Register
			r.type = MessageType::Register;
			r.buffer = new char[21];
			get_stream().recv_n(r.buffer, 21); // u12345
			char username[10];
			char password[10];
			int bIndex = 1, vIndex = 0;
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

			userauthenticationstatus accountSts;
			if (_type == 1) {

				accountSts = UserManager::registerUser(std::string(username), std::string(password));
			}
			else if (_type == 2) {
				accountSts = UserManager::authenticateUser(std::string(username), std::string(password));
			}

		
			response res;
			if(accountSts == userauthenticationstatus::OK) {
				res.type = ResponseType::OK;
				res.buffer = (char*)UserManager::getsenderId(std::string(username)).c_str();

				std::string _responseBuffer = responsehelper::parseresponse(res);
				const char* responseBuffer = _responseBuffer.c_str();

				this->peer_.send_n(responseBuffer, _responseBuffer.size()); // send userId back to the client who has sent the register request whihc he will in the next header
				//-- call login method and load the home screen of that user
				return 0;
			} // 
			else if (accountSts != userauthenticationstatus::Exist)
			{
				ACE_DEBUG((LM_INFO, "-----> User Already Exist, Try Again\n"));
				// call register again 
			}

			else if (accountSts != userauthenticationstatus::UsernamePasswordMismatch)
			{
				ACE_DEBUG((LM_INFO, "-----> Eithe the password or the username you used is incorrect\n"));
				// call login again 
			}

			// Send a request to UserCreateResponse UserHelper::createuser();
		}
		
		else if (_type == 3) {
			// text 
			/*
			1- print request type 
			2- read 17 bytes, header, call parse header, validate header, response 
			3- send response 
			*/
			printf("|Message Request|");
			// resourse file in applicatin for having all type of messages 
			r.buffer = new char[17];
			get_stream().recv_n(r.buffer, 17);
			requesthelper::parseheader(r.buffer, r);
			r.buffer = new char[r.length];
			get_stream().recv_n(r.buffer, r.length);
			RequestStatus rStatus = requesthelper::validate_request(r);
			response resp;
			requesthelper::process_message(r, rStatus, resp);

		//	std::string responseStr = responsehelper::parseresponse(resp);



			/*char* responseBuffer = responseStr;
			get_stream().send_n(responseStr, responseStr.size());
*/
		}
		

		//// 1. Parse -- deserialize - convert bytes into object instance.
		//// 2. Validate -- length fine? headers? banned?
		//// 3. Process the response.
		//// 4. Send the response.

		//	spec = std::stoi(&data[0]);
	//	if (spec== 1) {  // register or login request
	//		printf("call login helper here");
	//		// parse header -- validate header-- process request-- send response
	//		_write(1, data, 17);
	//	}
	//	else if (spec==2){
	//		printf("call message helper here ");
	//	_write(1, data, 17);   // ill read jusr the header here and then pass it to get parsed 
	//}
	//	// rceived string from client now. encapsulate it into object.

//		char* data_;
		//requesthelper::parseheader(requesthelper::parserequest(r), r);
		////
		//r.buffer = new char[r.length];
		//spec= get_stream().recv_n(r.buffer, r.length);

		//RequestStatus rStatus = requesthelper::validate_request(r);
		//response resp;
		//requesthelper::process_message(r, rStatus, resp);

		//std::string responseStr = responsehelper::parseresponse(resp);

		//// 1. Parse -- deserialize - convert bytes into object instance.
		//// 2. Validate -- length fine? headers? banned?
		//// 3. Process the response.
		//// 4. Send the response.

		

		//bytesend = get_stream().send_n(data, bytereceived);
		/*char* responseBuffer = responseStr;
		get_stream().send_n(responseStr, responseStr.size());*/
		//_write(1, r.buffer, 21);
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
	char header[17] = { 0 };
	char specifier[1] = { 0 };
	int bytesend = 0, bytereceived = 0;
	int spec = 0; 

};
class Accept_Handler : public ACE_Event_Handler {
public:
	//Constructor
	Accept_Handler()
	{
		ACE_DEBUG((LM_DEBUG, "Listening......|.....|...\n"));
		
	}
	explicit Accept_Handler(ACE_Addr &addr) {
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
	ACE_INET_Addr addr(50009);
	Accept_Handler *eh = new Accept_Handler(addr);
	ACE_Reactor::instance()->register_handler(eh,
		ACE_Event_Handler::ACCEPT_MASK);
	while (1)
		ACE_Reactor::instance()->run_reactor_event_loop();
}


