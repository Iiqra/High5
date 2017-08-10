#pragma once

#include "ace/SOCK_Acceptor.h"
#include  <string>
enum  ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK };
enum  ResponseMessage {
	CantLogin = 1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK, LoginOK,
	GroupCreated, AddedInGroup, AlreadyMember, PrivateGroup, MemberList, GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers,
	InvalidSpecifier, InvalidToorFrom, Unauthorized, AlreadyLoggedIn, Logout, Custom = 255
};
class response {
public:
	ACE_SOCK_Stream* socket;
	char type;
	char *length;
	char *buffer;
};

class responsehelper {
public:
	static void getlength(char ** buffer, int length);
	static std::string parseresponse(response &resp, std::string payload, bool string = true);
};
