//#include "ace/Reactor.h"
//#include "ace/SOCK_Acceptor.h"
//#include "ace/Log_Msg.h"
//#include "MessageProtocol.h"
//#include "User.h"
//#include "ace/OS.h"
//#include <cctype>
//#include "ace/Task.h"
//
//
//typedef ACE_SOCK_Acceptor Acceptor;
//class Accept_Handler;
//class RecvSend: public ACE_Task<ACE_MT_SYNCH> {
//	int virtual svc() {
//		ACE_DEBUG((LM_DEBUG, "\n\n[%t]Inside svc method\n\n"));
//
//		// send and receive calls for a perticular client
//		return 0;
//	}
//};
//class Service_Handler : public ACE_Event_Handler {
//
//public:
//	int handle_input(ACE_HANDLE handle) override {
//		ACE_DEBUG((LM_DEBUG, "\n\n[%t]Input handle method method\n\n"));
//		
//		RecvSend handler;
//		handler.activate();
//		handler.wait();
//		return 0;
//	}
//	
//	ACE_HANDLE get_handle(void) const override
//	{
//		return peer_.get_handle(); //
//	}
//	ACE_SOCK_Stream &
//		get_stream() {
//		return this->peer_;
//	}
//private:
//	ACE_SOCK_Stream peer_;
//
//	int bytesent = 0, bytereceived = 0;
//
//};
//class Accept_Handler : public ACE_Event_Handler {
//public:
//	//Constructor
//	Accept_Handler()
//	{
//
//	}
//	explicit Accept_Handler(ACE_Addr &addr) {
//		ACE_DEBUG((LM_DEBUG, "Listening......|.....|...\n"));
//
//		this->open(addr);
//	}
//	int    open(ACE_Addr &addr) {
//		peer_acceptor.open(addr);
//		return 0; // return zero on success
//	}
//	//Called when input events occur(e.g., connection or data).
//	int handle_input(ACE_HANDLE ace_handle) override {
//		Service_Handler *eh = new Service_Handler();
//		ACE_TCHAR peer_name[MAXHOSTNAMELEN];
//		ACE_INET_Addr peer_addr;
//
//		if (this->peer_acceptor.accept(eh->get_stream(), // stream
//			0, // remote address
//			0, // timeout
//			1) == -1) //restart if interrupted
//			ACE_DEBUG((LM_ERROR, "Error in connection\n"));
//		ACE_DEBUG((LM_DEBUG, "Connection established\n"));
//		eh->get_stream().get_remote_addr(peer_addr);
//		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN);
//		ACE_DEBUG((LM_DEBUG, "with %s \n", peer_name));
//		ACE_Reactor::instance()->
//			register_handler(eh, ACE_Event_Handler::READ_MASK);
//		return 0;
//	}
//	//Used by the reactor to determine the underlying handle
//	ACE_HANDLE
//		get_handle(void) const override {
//		return this->peer_acceptor.get_handle();
//	}
//private:
//	Acceptor peer_acceptor;
//};
//int main(int argc, char * argv[]) {
//	system("pause");
//	ACE_INET_Addr addr(50009);
//	Accept_Handler *eh = new Accept_Handler(addr);
//	ACE_Reactor::instance()->register_handler(eh,
//		ACE_Event_Handler::ACCEPT_MASK);
//	while (1)
//		ACE_Reactor::instance()->run_reactor_event_loop();
//
//	return 0;
//}
//
//
//
//
//
////class RecvSend1: public ACE_Task_Base {
////int open(void *args){
////peer = (ACE_SOCK_Stream * ) args; 
////return activate();
////}
////int virtual svc() {
////ACE_DEBUG((LM_DEBUG, "\n\n[%t]Inside svc method\n\n"));
////// send and receive calls for a perticular client
////return 0;
////}
////ACE_SOCK_Stream *peer;
////};
////int handle_input(ACE_HANDLE handle) override {
////ACE_DEBUG((LM_DEBUG, "\n\n[%t]Input handle method method\n\n"));
////
////RecvSend1 handler;
////handler.open(&peer);
////handler.wait();
////return 0;
////}
// coonectio acceptor, 