#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include <iostream>
#define SIZE 10
typedef ACE_SOCK_Acceptor Acceptor;
class Accept_Handler;
class Service_Handler : public ACE_Event_Handler {
public:
	Service_Handler() {
		ACE_DEBUG((LM_DEBUG, "Input Handler Constructor\r\n"));
	}
	int handle_input(ACE_HANDLE handle) override
	
	{	
		int rec_cnt = peer_i().recv_n(data,SIZE);

		peer_i().send_n(data, rec_cnt);
	
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
		peer_i() {
		return this->peer_;
	}
private:
	ACE_SOCK_Stream peer_;
	char data[10] = {0};
};
class Accept_Handler : public ACE_Event_Handler {
public:
	//Constructor
	explicit Accept_Handler(ACE_Addr &addr) {
		this->open(addr);
	}
	int    open(ACE_Addr &addr) {
		peer_acceptor.open(addr);
		return 0;
	}
	int   handle_input(ACE_HANDLE ace_handle) override {
		Service_Handler *eh = new Service_Handler();
		if (this->peer_acceptor.accept(eh->peer_i(), // stream
			0, // remote address
			0, // timeout
			1) == -1) //restart if interrupted
			ACE_DEBUG((LM_ERROR, "Error in connection\n"));
		ACE_DEBUG((LM_DEBUG, "Connection established\n"));
		ACE_Reactor::instance()->
			register_handler(eh, ACE_Event_Handler::READ_MASK);
		return 0;
	}

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