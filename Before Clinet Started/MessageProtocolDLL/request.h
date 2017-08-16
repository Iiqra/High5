#pragma once
#include "ace/SOCK_Acceptor.h"
#include "MessageProtocol.h"

enum  RequestStatus { InvalidPacketLength, InvalidSenderId, InvalidReceiverId, InvalidRequestSpecifier, BlockedSender, BlockedRecipient, Unauthorized, OK }; enum  RequestStatus { InvalidPacketLength, InvalidSenderId, InvalidReceiverId, InvalidRequestSpecifier, BlockedSender, BlockedRecipient, Unauthorized, OK };
class request {
public:
	MessageProtocol::MessageType type;
	char sender[6];
	char recipient[6];
	int length;
	char* buffer;
};

class requesthelper {
public:
	static int request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len);

};
