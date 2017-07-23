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

void read_handler(ACE_SOCK_Stream& _peer) {
	while (1) {
		// Keep reading
		// On a new thread
	}
}

void register_handler(ACE_SOCK_Stream& _peer) {
	char buffer[10] = {0}; 	
	int br;
	printf("Let's break the 'Register Message'," "\n"
		"[1] Correct Message| 1username01password01" "\n"
		"[2] Correct Message for another Client| 1username02password02  " "\n"
		"[3] Duplicate user name| 1username01password02" "\n"
		"[4] Any of the field length isnt of 10 bytes | 1sername01password02" "\n\n"
		"[5] Custom data" "\n\n");
	char nestedChoice;
	std::cin >> nestedChoice;
	switch (nestedChoice) {
	case '1':
		_peer.send_n("1username01password01", 21); // Perfect:correct message.
		br = _peer.recv_n(buffer, 5);
		buffer[br++] = '\0';
		_write(1, buffer, br);
		break;
	case '2':
		_peer.send_n("1username02password02", 21); // Perfect: correct message for 2nd client
		break;
	case '3':
		_peer.send_n("1username01password02", 21); // Error: duplicate username | already exist 	
		break;
	case '4':
		_peer.send_n("1sername03password02", 20); // Error: either username or password is not of length 10| sever will read one letter from password and termed as username thast wrong
		break;
	case '5':
		std::string customInfo;
		cin >> customInfo;

		_peer.send_n(customInfo.c_str(), customInfo.size());
			}

	char type[2] = { 0 };
	type[1] = '\0';
	_peer.recv_n(type, 1); // get type 
	_write(1, type, 1);
	char length[5]; // 
	length[4] = '\0';

	_peer.recv_n(length, 4); // get length
	_write(1, length, 4); // print just fro debugging

	int len;
	len = std::stoi(length);

	char *mes;
	mes = new char[len + 1];
	mes[len] = '\0';
	_peer.recv_n(mes, len);

	_write(1, mes, len);
}

void login_handler(ACE_SOCK_Stream& _peer) {

	char buffer[100] = { 0 }; 	int br;
	printf("before nested menu");
	printf("Let's break the 'Login Message'," "\n"
		"[1] Correct Message| 2username01password01" "\n"
		"[2] Correct Message| 2username02password02" "\n"
		"[3] Username and password mismatch| 2username01password02  " "\n"
		"[4] Username doesnt exist| 2username06password02" "\n\n"
	);
	char nestedChoice;
	std::cin >> nestedChoice;
	switch (nestedChoice) {
	case '1':
		_peer.send_n("2username01password01", 21); // Perfect: correct match of username and password.
		break;
	case '2':
		_peer.send_n("2username02password02", 21); // Perfect: correct match of username and password.
		break;
	case '3':
		_peer.send_n("2username01password02", 21); // Error: username and password mismatch
		break;
	case '4':
		_peer.send_n("2username06password02", 21); // Error: username doesnt exist.
	}

	char type[2] = { 0 };
	type[1] = '\0';
	_peer.recv_n(type, 1); // get type 
	_write(1, type, 1);
	char length[5]; // 
	length[4] = '\0';

	_peer.recv_n(length, 4); // get length
	_write(1, length, 4); // print just fro debugging

	int len;
	len = std::stoi(length);

	char *mes;
	mes = new char[len + 1];
	mes[len] = '\0';
	_peer.recv_n(mes, len);
	_write(1, mes, len);
}
void message_handler(ACE_SOCK_Stream& _peer) {
	char buffer[100] = { 0 }; 	int br;
	printf("before nested menu");
	printf("Let's break the 'Message request'," "\n"
		"[1] Correct Message| 3u00001u000020010HeyYouOkay" "\n"
		"[2] Recepient not registred| 3u00001u000050010HeyYouOkay  " "\n"
		"[3] Send message to odd group | 3u00001g00odd0010HeyYouOkay  " "\n"
		"[4] Send message to even group | 3u00001g0even0010HeyYouOkay  " "\n"
		"[5] Send message everyone | 3u00001BRDCST0010HeyYouOkay  " "\n"
	);
	char nestedChoice;
	std::cin >> nestedChoice;
	switch (nestedChoice) {
	case '1':
		_peer.send_n("3u00001u000020010HeyYouOkay", 27); // Perfect: correct message.
		/*br = _peer.recv_n(buffer, 100);
		buffer[br++] = '\0';
		_write(1, buffer, br);*/
		break;
	case '2':
		_peer.send_n("3u00001u000050010HeyYouOkay", 27); // Error: Recipient not registered, invalid.
		break;

	case '3':
		_peer.send_n("3u00001g00odd0010HeyYouOkay", 27); // Error: Recipient not registered, invalid.
		break;
	case '4':
		_peer.send_n("3u00001g0even0010HeyYouOkay", 27); // Error: Recipient not registered, invalid.
		break;
	case '5':
		_peer.send_n("3u00001BRDCST0010HeyYouOkay", 27); // Error: Recipient not registered, invalid.
		break;
	}


	char type[2] = { 0 };
	type[1] = '\0';
	_peer.recv_n(type, 1); // get type 
	_write(1, type,1); 
	char length[5]; // 
	length[4] = '\0';
	
	_peer.recv_n(length, 4); // get length
	_write(1, length, 4); // print just fro debugging

	int len;
	len = std::stoi(length);

	char *mes;
	mes = new char[len];
	mes[len--] = '/0';
	_peer.recv_n(mes, len); 
	_write(1, mes, len); 
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

	while (true) {
		printf("Hello, Would you like to login, register or send message to active clients?" "\n"
			"[1] Login" "\n"
			"[2] Register" "\n"
			"[3] Message");
		char choice;
		std::cin >> choice;
		request r;
		char *data;
		switch (choice) {
		case '2':
			register_handler(peer);
			break;
		case '1':
			login_handler(peer);
			break;
		case '3':
			message_handler(peer);
			break;
		}
	}
	peer.close();
	return (0);
 }


	//	while (true)
	//	{
	//		printf("Hello, Would you like to log in or register?" "\n"
	//			"[1] Login" "\n"
	//			"[2] Register" "\n"
	//			"[3] Exit");
	//		//ACE_OS::printf("Enter 1 for login\r\n Enter 2 for Text Message\r\n");
	//		char choice;
	//		std::cin >> choice;
	//		request r; //char buffer[20];
	//		char *data;
	//		switch (choice) {
	//		case '1': // register and login
	//			r.type = MessageType::Register;
	//			// data = new char[21 ];
	//			data = "1un12345678pw12345678";
	//			// 10020usernameepassword
	//			//peer.send_n(r.buffer, (unsigned)strlen(r.buffer));
	//			//intf("Enter something")

	//			peer.send_n("1un12345678pw12345678", 21);
	//			// peer.send_n("0123456789", 10);
	//			break;
	//		case '2': // message 
	//			r.type = MessageType::Message;
	//			//strcpy_s(r.sender, "sndrid");
	//			//strcpy_s(r.recipient, "recvid");
	//			////r.recipient = "recepient";
	//			//r.length = 10;
	//			//		r.buffer = "355555566666600100123456789";
	//			// data = new char[21];
	//			data = "355555566666600100123456789";
	//			//hicallme10
	//			//	r.buffer = "2sndridrecvid0010heyyouten!";
	//			peer.send_n(data, 27);
	//			break;
	//		}
	//		ACE_OS::read(ACE_STDIN, buff, 5);
	//			bs=peer.send_n(buff, 5);
	//			// to rcv from server 
	//			br = peer.recv_n(buff, bs);
	//			_write(1, buff, br);
	//	}
	//}
