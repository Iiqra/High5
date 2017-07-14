#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include <iostream>
#include <string>
#include "User.h"
#include "MessageProtocol.h"

#define SIZE 5
using namespace std;
 //static enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
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
	int bs, br;
	char buff[SIZE] = {0};
	while (true)
	{
    ACE_OS::printf("Enter 1 for login\r\n Enter 2 for Text Message\r\n");
	char choice;
	std::cin >>choice;
	UserEntity *user = new UserEntity(); 
	request *r = new request();
	
	switch (choice) {
	case '1':
	{
		/*user->userName=  "someone";
		user->password = "Password";*/
		r->type = MessageType::Register;
    	//const int size= r->length = 20;
		char buffer[20];
		r->buffer = "usernameeepassworddd"; 
	}
		break;
	case '2':
		r->type = MessageType::Message;
		r->length = 10;
		r->buffer = "heyyouten!";
		break;
	}
	ACE_OS::read(ACE_STDIN, buff, 5);
		bs=peer.send_n(buff, 5);
		// to rcv from server 
		br = peer.recv_n(buff, bs);
		_write(1, buff, br);
	}
	peer.close();
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
