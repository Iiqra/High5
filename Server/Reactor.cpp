#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include <iostream>
#include "ace/OS.h"
#include "Message.h"


class Connection {
public:
	int id;
	ACE_SOCK_Stream *socket;
	std::string userid;

	Connection(int i, ACE_SOCK_Stream *sock, std::string userid) :
		id(i), socket(sock), userid(userid) {}
};

class ClientManager {

public:
	static std::vector<Connection> connections;

	static void addconnection(Connection conn);

	static void removeconnection(int id);

	static void getpeerinfo(std::string userid, Connection &connection);
};

#define SIZE 10
int id = 1;
typedef ACE_SOCK_Acceptor Acceptor;
class Accept_Handler;
class Service_Handler : public ACE_Event_Handler {
public:  Service_Handler() {
		ACE_DEBUG((LM_DEBUG, "Input Handler Constructor\r\n"));
	}
	int handle_input(ACE_HANDLE handle) override
	{	
        bytereceived = get_stream().recv_n(data,SIZE);

		bytesend = get_stream().send_n(data, bytereceived);
		request r;

		if (r.type = 1) // login
		{
			Connection con(1, &get_stream(), "someone");
			ClientManager::addconnection(con); // con stored
		}
		auto response = MessageHelper::processmessage(r);


		// C:\First\Second\Third\folder.txt

	/*data[rec_cnt] = '\0';
	printf("\r\nClient said");
	std::cout <<data<<std::endl;*/
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
	char data[10] = {0};
	int bytesend, bytereceived;
};
class Accept_Handler : public ACE_Event_Handler {
public:
	// constructor
	 Accept_Handler(ACE_Addr &addr) {
		this->open(addr);
	}
	int open(ACE_Addr &addr) {
		peer.open(addr);
		return 0;
	}
	int   handle_input(ACE_HANDLE ace_handle) override {
		Service_Handler *eh = new Service_Handler();
		ACE_TCHAR peer_name[MAXHOSTNAMELEN];
		ACE_INET_Addr peer_adr;

		if (this->acceptor.accept(eh->peer_i(), // stream
			0, // remote address
			0, // timeout
			1) == -1) //restart if interrupted
			ACE_DEBUG((LM_ERROR, "Error in connection (%P|%t)\n"));
			ACE_DEBUG((LM_DEBUG, "Connection established from %s\n", peer_name));
		//<ip:500087>
  //<ip:50086>
		ACE_Reactor::instance()->
			register_handler(eh, ACE_Event_Handler::READ_MASK);
		return 0;
	}
	ACE_HANDLE
		get_handle(void) const override {
		return this->acceptor.get_handle();
	}
private:
	ACE_SOCK_Acceptor acceptor;
};
int main(int argc, char * argv[]) {
	ACE_INET_Addr addr(50009);
	Accept_Handler *eh = new Accept_Handler(addr);
	ACE_Reactor::instance()->register_handler(eh,
		ACE_Event_Handler::ACCEPT_MASK);
	while (1)
		ACE_Reactor::instance()->run_reactor_event_loop();
}