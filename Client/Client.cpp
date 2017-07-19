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
	char buff[SIZE] = { 0 }; int send;
	while (true)
	{
		printf("Hello, Would you like to log in or register?" "\n"
			"[1] Login" "\n"
			"[2] Register" "\n"
			"[3] Exit");
		//ACE_OS::printf("Enter 1 for login\r\n Enter 2 for Text Message\r\n");
		char choice;
		std::cin >>choice;	
		request r; //char buffer[20];
		char *data;
		switch (choice) {
			case '1': // register and login
				r.type = MessageType::Register;
				data = new char;
				data= "1un12345678pw12345678";
				// 10020usernameepassword
		      //peer.send_n(r.buffer, (unsigned)strlen(r.buffer));
			bs=	peer.send_n(data, 21 );

			  
			  // peer.send_n("0123456789", 10);
			  break;
			case '2': // message 
				r.type = MessageType::Message;
				//strcpy_s(r.sender, "sndrid");
				//strcpy_s(r.recipient, "recvid");
				////r.recipient = "recepient";
				//r.length = 10;
		//		r.buffer = "355555566666600100123456789";
				data = new char;
			data = "355555566666600100123456789";
		//hicallme10
			//	r.buffer = "2sndridrecvid0010heyyouten!";
				peer.send_n(data, 27);
				break;
		}
		//ACE_OS::read(ACE_STDIN, buff, 5);
		//	bs=peer.send_n(buff, 5);
		//	// to rcv from server 
		//	br = peer.recv_n(buff, bs);
		//	_write(1, buff, br);
	}
	peer.close();
	return (0);
}
