// MessageProtocolDLL.h
#pragma once
#include "ace/SOCK_Acceptor.h"

namespace MessageProtocol {

	enum  class  _declspec(dllexport) MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
	enum  class _declspec(dllexport) ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK };
	enum  class  _declspec(dllexport) ResponseMessage {
		CantLogin = 1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK, LoginOK,
		GroupCreated, AddedInGroup, AlreadyMember, PrivateGroup, MemberList, GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers,
		InvalidSpecifier, InvalidToorFrom, Unauthorized, AlreadyLoggedIn, Logout, Custom = 255
	};

	
	  class _declspec(dllexport) request {
	public:
		MessageProtocol::MessageType type;
		char sender[6];
		char recipient[6];
		int length;
		char* buffer;
	};

	class  _declspec(dllexport) requesthelper {
	public:
		static int request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len);

	};

	
	class _declspec(dllexport) response {
	public:
		ACE_SOCK_Stream* socket;
		char type;
		char *length;
		char *buffer;
	};

	class  _declspec(dllexport) responsehelper {
	public:
		static void getlength(char ** buffer, int length);
		static std::string parseresponse(response &resp, std::string payload, bool string = true);
	};
}