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


void register_handler(ACE_SOCK_Stream _peer) {
	char buffer[100] = {0}; 	int br;
	while (true) {
	
		printf("before nested menu");

		printf("Let's break the 'Register Message'," "\n"
			"[1] Correct Message| 1username01password01" "\n"
			"[2] Correct Message for another Client| 1username02password02  " "\n"
			"[3] Duplicate user name| 1username01password02" "\n"
			"[4] Any of the field length isnt of 10 bytes | 1sername01password02" "\n");
		char nestedChoice;
		std::cin >> nestedChoice;
		switch (nestedChoice) {
		case '1':
			//_peer.send_n("1username01password01", 21); // Perfect:correct message.
			//br = _peer.recv_n(buffer, 100);
			//buffer[br++] = '\0';
			//_write(1, buffer, br);
			break;
		case '2':
			_peer.send_n("1username02password02", 21); // Perfect: correct message for 2nd client
			break;
		case '3':
			_peer.send_n("1username01password02", 21); // Error: duplicate username | already exist 	break;
		case '4':
			_peer.send_n("1sername03password02", 20); // Error: either username or password is not of length 10| sever will read one letter from password and termed as username thast wrong
			break;
		}
		printf("after nested menu");

	}	
}

void login_handler(ACE_SOCK_Stream _peer) {

	char buffer[100] = { 0 }; 	int br;
	while (true) {

		printf("before nested menu");
		printf("Let's break the 'Login Message'," "\n"
			"[1] Correct Message| 2username01password01" "\n"
			"[2] Username and password mismatch| 2username01password02  " "\n"
			"[3] Username doesnt exist| 2username02password02" "\n"
			);
		char nestedChoice;
		std::cin >> nestedChoice;
		switch (nestedChoice) {
		case '1':
			_peer.send_n("2username01password01", 21); // Perfect: correct match of username and password.
			br = _peer.recv_n(buffer, 100);
			buffer[br++] = '\0';
			_write(1, buffer, br);
			break;
		case '2':
			_peer.send_n("2username01password02", 21); // Error: username and password mismatch
			break;
		case '3':
			_peer.send_n("2username02password02", 21); // Error: username doesnt exist.

		}
		printf("after nested menu");

	}
}
void message_handler(ACE_SOCK_Stream _peer) {
	char buffer[100] = { 0 }; 	int br;
	while (true) {
		printf("before nested menu");
		printf("Let's break the 'Message request'," "\n"
			"[1] Correct Message| 3u00001u000020010HeyYouOkay" "\n"
			"[2] Recepient not registred| u00001u000050010HeyYouOkay  " "\n"
		);
		char nestedChoice;
		std::cin >> nestedChoice;
		switch (nestedChoice) {
		case '1':
			_peer.send_n("3u00001u000020010HeyYouOkay", 27); // Perfect: correct message.
			br = _peer.recv_n(buffer, 100);
			buffer[br++] = '\0';
			_write(1, buffer, br);
			break;
		case '2':
			_peer.send_n("3u00001u000050010HeyYouOkay", 27); // Error: Recipient not registered, invalid.
		}
		printf("after nested menu");

	}

}
 int ACE_TMAIN(int, ACE_TCHAR *[])
{
	 printf("Inside  main");
	ACE_INET_Addr srvr(50009, ACE_LOCALHOST);
	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;
	// Try connecting
	if (-1 == connector.connect(peer, srvr))
	        ACE_ERROR_RETURN((LM_ERROR,
			ACE_TEXT("%p\r\n"),
			ACE_TEXT("connect")), 100);

	while (true)
	{
		printf("before first menu");

		printf("Hello, Would you like to login, register or send message to active clients?" "\n"
			"[1] Login" "\n"
			"[2] Register" "\n"
			"[3] Message");
		char choice;
		std::cin >> choice;
		request r;
		char *data;
		switch (choice) {
		case '1':
			register_handler(peer);
		
			break;
		case '2':
		
			login_handler(peer);
			break;
		case '3':
			message_handler(peer);
			//peer.send_n("3u00001u000050010HeyYouOkay", 21);												//	peer.send_n("3username02password02", 21); // Error: username doesnt exist.
			break;
		}
		printf("after first menu");


	}
	peer.close();
	return (0);
 }

		//while (true)
		//{
		//	printf("Hello, Would you like to log in or register?" "\n"
		//		"[1] Login" "\n"
		//		"[2] Register" "\n"
		//		"[3] Exit");
		//	//ACE_OS::printf("Enter 1 for login\r\n Enter 2 for Text Message\r\n");
		//	char choice;
		//	std::cin >> choice;
		//	request r; //char buffer[20];
		//	char *data;
		//	switch (choice) {
		//	case '1': // register and login
		//		r.type = MessageType::Register;
		//		// data = new char[21 ];
		//		data = "1un12345678pw12345678";
		//		// 10020usernameepassword
		//		//peer.send_n(r.buffer, (unsigned)strlen(r.buffer));
		//		//intf("Enter something")

		//		peer.send_n("1un12345678pw12345678", 21);
		//		// peer.send_n("0123456789", 10);
		//		break;
		//	case '2': // message 
		//		r.type = MessageType::Message;
		//		//strcpy_s(r.sender, "sndrid");
		//		//strcpy_s(r.recipient, "recvid");
		//		////r.recipient = "recepient";
		//		//r.length = 10;
		//		//		r.buffer = "355555566666600100123456789";
		//		// data = new char[21];
		//		data = "355555566666600100123456789";
		//		//hicallme10
		//		//	r.buffer = "2sndridrecvid0010heyyouten!";
		//		peer.send_n(data, 27);
		//		break;
		//	}
			//ACE_OS::read(ACE_STDIN, buff, 5);
			//	bs=peer.send_n(buff, 5);
			//	// to rcv from server 
			//	br = peer.recv_n(buff, bs);
			//	_write(1, buff, br);
	//	}
	
