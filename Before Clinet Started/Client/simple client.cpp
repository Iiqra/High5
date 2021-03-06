#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include <iostream>
#include <string>
#include "User.h"
#include "MessageProtocol.h"
using namespace std;


int ACE_TMAIN(int, ACE_TCHAR *[])
{
	printf("Inside  main");
	ACE_INET_Addr srvr(50009, ACE_LOCALHOST);
	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;
	 //Try connecting
	if (-1 == connector.connect(peer, srvr))
		ACE_ERROR_RETURN((LM_ERROR,
			ACE_TEXT("%p\r\n"),
			ACE_TEXT("connect")), 100);

	while (true) {

		printf("\nEnter something to send\n");
		char buff[21];
		char buff1[21];

		// send
		ACE_OS::read(ACE_STDIN, buff, sizeof(buff));
		int bs= peer.send_n(buff, sizeof(buff));

		
		int br = peer.recv_n(buff1, sizeof(buff1));
		_write(1, buff1, bs);
	}
	peer.close();
	return (0);
}
