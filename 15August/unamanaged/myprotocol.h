#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
#include "ace/SOCK_Acceptor.h"

namespace mycode {
	class request;
	class response;
	enum class MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
	enum class RequestStatus { InvalidPacketLength, InvalidSenderId, InvalidReceiverId, InvalidRequestSpecifier, BlockedSender, BlockedRecipient, Unauthorized, OK };
	enum class ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK };
	enum class ResponseMessage {
		CantLogin = 1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK, LoginOK,
		GroupCreated, AddedInGroup, AlreadyMember, PrivateGroup, MemberList, GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers,
		InvalidSpecifier, InvalidToorFrom, Unauthorized, AlreadyLoggedIn, Logout, Custom = 255
	};

	class ProtocolManager {
	public:
		enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
		int Id;
		MessageType type;
		std::string sender;
		std::string recipient;
		std::time_t datetime;
		std::string filepath;
		std::string filename;
		std::string fileextension;
		int length;
		std::string actualcontent;

	};

	class request {
	public:
		MessageType type;
		char sender[6];
		char recipient[6];
		int length;
		char* buffer;
	};

	class requesthelper {
	public:
		static int request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len);

	};
	class response {
	public:
		ACE_SOCK_Stream* socket;
		int type;
		char *length;
		char *buffer;
	};

	class responsehelper {
	public:
		static void getlength(char ** buffer, int length);
		static std::string parseresponse(response &resp, std::string payload, bool string = true);
	};
}