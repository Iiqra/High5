#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
class MyServiceHandler; //forward declaration
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;
class MyServiceHandler :
	public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
	// The two thread names are kept here
	ACE_thread_t thread_names[1];
public:
	int open(void*) {
		ACE_DEBUG((LM_DEBUG, "Acceptor: received new connection \n"));
		//Register with the reactor to remember this handler..
		Reactor::instance()
			->register_handler(this, ACE_Event_Handler::READ_MASK);
		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID:(%t) open\n"));
		//Create two new threads to create and send messages to the
		//remote machine.
		activate(THR_NEW_LWP,
			1, //2 new threads
			0, //force active false, if already created don’t try again.
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
	void send_message1(void) {
		peer().send_n("Message1", 10);
	} //end send_message1
	
	int svc(void) {
		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
		if (ACE_Thread::self() == thread_names[0])
			while (1) send_message1(); //send message 1s forever
		
		return 0; // keep the compiler happy.
	}
	int handle_input(ACE_HANDLE) {
		ACE_DEBUG((LM_DEBUG, "(%t) handle_input::"));
		char* data = new char[13];
		//Check if peer aborted the connection
		int br = peer().recv_n(data, 12);
		if (br == 0) {
			printf("Peer probably aborted connection");
			return -1; //de-register from the Reactor.
			_write(1, data, br);

		}
		//Show what you got..
		ACE_OS::printf(" << %s\n", data);
		//keep yourself registered
		return 0;
	}
};
int main(int argc, char* argv[]) {
	ACE_INET_Addr addr(50009);
	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main"));
	//Prepare to accept connections
	Acceptor myacceptor(addr, Reactor::instance());
	// wait for something to happen.
	while (1)
		Reactor::instance()->handle_events();
	return 0;
}