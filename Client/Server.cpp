//#include <iostream>
//#include <string>
//#include "ace/Auto_Ptr.h"
//#include "ace/Log_Msg.h"
//#include "ace/INET_Addr.h"
//#include "ace/SOCK_Acceptor.h"
//#include "ace/Reactor.h"
//#include "ace/Message_Block.h"
//#include "ace/Message_Queue.h"
//#include "ace/SOCK_Stream.h"
//#include "ace/OS.h"
//#include "ace/Task.h"
//#include "ConnectionManager.h"
//
//int id = 1;
//
//class ClientAcceptor : public ACE_Event_Handler
//{
//public:
//	//virtual ~ClientAcceptor();
//	int open(const ACE_INET_Addr &listen_addr);
//
//	// Get this handler's I/O handle.
//	virtual ACE_HANDLE get_handle(void) const
//	{
//		ACE_DEBUG((LM_DEBUG, "\nInside ClientAcceptor get_handle"));
//		return this->acceptor_.get_handle();
//	}
//	// Called when a connection is ready to accept.
//	virtual int handle_input(ACE_HANDLE);
//
//protected:
//	ACE_SOCK_Acceptor acceptor_;
//};
//
//int ClientAcceptor::open(const ACE_INET_Addr &listen_addr)
//{
//	ACE_DEBUG((LM_DEBUG, "\nInside ClientAcceptor Open"));
//	if (this->acceptor_.open(listen_addr, 1) == -1)
//		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"),
//			ACE_TEXT("acceptor.open")),
//			-1);
//	return this->reactor()->register_handler
//	(this, ACE_Event_Handler::ACCEPT_MASK);
//}
//
//class ClientService : public ACE_Event_Handler
//{
//public:
//	ACE_SOCK_Stream &peer(void) {
//		ACE_DEBUG((LM_DEBUG, "\nInside ClientService Peer"));
//		return this->sock_;
//	}
//
//	int open(void);
//
//	// Get this handler's I/O handle.
//	virtual ACE_HANDLE get_handle(void) const
//	{
//		ACE_DEBUG((LM_DEBUG, "\nInside ClientService gethandle"));
//		return this->sock_.get_handle();
//	}
//
//	// Called when input is available from the client.
//	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
//
//	// Called when output is possible.
//	virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);
//
//protected:
//	ACE_SOCK_Stream sock_;
//	ACE_Message_Queue<ACE_NULL_SYNCH> output_queue_;
//};
//
//int ClientAcceptor::handle_input(ACE_HANDLE)
//{
//	ACE_DEBUG((LM_DEBUG, "\nInside ClientAcceptor handle_Input"));
//	ClientService *client;
//	ACE_NEW_RETURN(client, ClientService, -1);
//
//	auto_ptr<ClientService> p(client);
//
//	if (this->acceptor_.accept(client->peer()) == -1)
//	
//		ACE_ERROR_RETURN((LM_ERROR,
//			ACE_TEXT("(%P|%t) %p\n"),
//			ACE_TEXT("Failed to accept ")
//			ACE_TEXT("client connection")),
//			-1);
//	p.release();
//	client->reactor(this->reactor());
//	if (client->open() == -1)
//		client->handle_close(ACE_INVALID_HANDLE, 0);
//	return 0;
//}
//int ClientService::open(void)
//{
//	ACE_DEBUG((LM_DEBUG, "\nInside ClientService open"));
//	ACE_TCHAR peer_name[MAXHOSTNAMELEN];
//	ACE_INET_Addr peer_addr;
//	if (this->sock_.get_remote_addr(peer_addr) == 0 &&
//		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN) == 0)
//		ACE_DEBUG((LM_DEBUG,
//			ACE_TEXT("(%P|%t) Connection from %s\n"),
//			peer_name));
//
//	return this->reactor()->register_handler
//	(this, ACE_Event_Handler::READ_MASK);
//}
//
//int ClientService::handle_input(ACE_HANDLE)
//{
//
//	ACE_DEBUG((LM_DEBUG, "\nInside ClientService handle_Input"));
//	const size_t INPUT_SIZE = 10; 
//	char buffer[INPUT_SIZE];
//	int recv_cnt;
//
//	if ((recv_cnt = this->sock_.recv(buffer, sizeof(buffer))) <= 0)
//	{
//		ACE_DEBUG((LM_DEBUG,
//			ACE_TEXT("(%P|%t) Connection has been closed hence no data received\n")));
//		return 0;
//	}
//	else 
//	{
//		ACE_OS::printf("\nWe have received this data from client\n");
//		_write(1, buffer, recv_cnt);
//	}
//
//	int id = std::stoi(buffer);
//
//	auto cons = ConnectionManager::connections;
//
//	for (auto con : cons) {
//	if (con.id == id) {
//	con.socket.send_n("1", 1);
//	}
//	}
//
//	// 1. Backend thread for database
//	// 2. ConnectionManager -- > List
//	// 3. List will be searched
//	// 4. If found, message will be forwarded.
//	return this->reactor()->register_handler
//	(this, ACE_Event_Handler::WRITE_MASK);
//
//	return 0;
//}
//
//int ClientService::handle_output(ACE_HANDLE)
//{
//	ACE_DEBUG((LM_DEBUG, "\nInside ClientService handle_output"));
//	const size_t INPUT_SIZE = 10;
//	char buffer[INPUT_SIZE];
//
//	ACE_OS::printf("\nWrite something to send to client\n");
//	ACE_OS::read(ACE_STDIN, buffer, sizeof(buffer));
//	sock_.send_n(buffer, sizeof(buffer));
//	return 0;
//}
//
//int ACE_TMAIN(int, ACE_TCHAR *[])
//{
//	ACE_INET_Addr port_to_listen(50009);
//	ClientAcceptor acceptor;
//	// = Accessors to set/get the various event demultiplexors.
//	/// Set the event demultiplexors
//	acceptor.reactor(ACE_Reactor::instance());
//	if (acceptor.open(port_to_listen) == -1)
//		return 1;
//	/*
//	After establishing the ACE_Reactor to use, we call the acceptor object's open()
//	method. Recall that it will begin listening and register with the
//	reactor for callbacks when new connections are ready to be accepted.
//	*/
//	// reactior is an event-dispatching class.
//	ACE_Reactor::instance()->run_reactor_event_loop();
//
//	return (0);
//}