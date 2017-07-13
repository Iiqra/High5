#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include <iostream>
#include <string>
#define SIZE 30
using namespace std;
int ACE_TMAIN(int, ACE_TCHAR *[])
{
	ACE_INET_Addr srvr(50009, ACE_LOCALHOST);
	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;
	if (-1 == connector.connect(peer, srvr))
		ACE_ERROR_RETURN((LM_ERROR,
			ACE_TEXT("%p\r\n"),
			ACE_TEXT("connect")), 100);
	//char buff[BUFFER];
	int bc;
	char buff[SIZE] = { 0 };
	// to send mxg 
	//string cName;
	//ACE_OS::printf("\n Give a Client Name\n");
	//std::cin >> cName;
	while (true)
	{
		ACE_OS::printf("\r\nEnter data to send ");
		ACE_OS::read(ACE_STDIN, buff, sizeof(buff));

		peer.send_n(buff, sizeof(buff));
		// to rcv from server 
		bc = peer.recv(buff, sizeof(buff));
		_write(1, buff, bc);
	}

	peer.close();

	system("pause");
	return (0);
}
//   bc = peer.recv(buff, 6);

/*while (1){
if(buff != '\0')
strcpy_s(buff2, buff);
else break;
}*/

//_write(1, buff,6);


//#include "ace/INET_Addr.h"
//#include "ace/SOCK_Stream.h"
//#include "ace/SOCK_Connector.h"
//#include "ace/Log_Msg.h"
//
//int ACE_TMAIN(int, ACE_TCHAR *[])
//{
//	ACE_INET_Addr srvr(50000, ACE_LOCALHOST);
//	/*we need address to the server to whcih client has to establish a 
//	connection with the server*/
//	
//	ACE_SOCK_Connector connector;
//	/*When connection is established, its time to get your socket 
//	connected using that address*/
//	
//	ACE_SOCK_Stream peer;
//	/*ACE represents a connected TCP socket with the 
//	ACE_SOCK_Stream object*/
//
//	/*so named because a TCP connection represents a virtual connection, 
//	or "stream" of bytes, as opposed to the connectionless 
//	datagrams you get with UDP sockets. In order to actively 
//	connect an ACE_SOCK_Stream to a server, we use an ACE_SOCK_Connector 
//	and the ACE_INET_Addr */
//
//	if (-1 == connector.connect(peer, srvr))
//    /*The connect() method is provided with the stream object to connect 
//	and the address to which it should be connected.*/
//		ACE_ERROR_RETURN((LM_ERROR,
//			ACE_TEXT("%p\n"),
//			ACE_TEXT("connect")), 1);
//	/*The method then attempts to establish that relationship
//	If successful, the ACE_SOCK_Stream is placed into a connected state,
//	and we can use it to communicate with the server. At this point, 
//	we can begin communicating*/
//
//	int bc;
//	char buf[64];
//
//	peer.send_n("uptime\n", 7);
//	/*ACE_SOCK_Stream inherits from a number of classes that are part of ACE's 
//	design to properly abstract behavior away in layers. Although send_n() is a 
//	method defined on ACE_SOCK_Stream*/
//	bc = peer.recv(buf, sizeof(buf));
//	_write(1, buf, bc);
//	peer.close();
//
//	return (0);
//}
