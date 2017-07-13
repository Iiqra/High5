#include <iostream>
#include <string>
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Message_Block.h"
#include "ace/Message_Queue.h"
#include "ace/SOCK_Stream.h"
#include "ace/OS.h"
#include "ace/Task.h"
#include "ClientManager.h"

class Service_Handler : public ACE_Event_Handler{
	int handle_input(ACE_HANDLE handle);
	ACE_HANDLE get_handle(void);
	ACE_SOCK_Stream &peer_i();	
private:
	ACE_SOCK_Stream peer_;
	char data[10] = { 0 };
	int bytesend, bytereceived;

};
class Accept_Handler : public ACE_Event_Handler {
public:
	int open(ACE_Addr &addr);
	int handle_input(ACE_HANDLE ace_handle);
	ACE_HANDLE get_handle(void);
private:
	ACE_SOCK_Stream peer;
};