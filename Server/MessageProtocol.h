#pragma once
#include<string>
#include<vector>
#include <ctime>
#include <string>
#include <sstream>
#include "User.h"
#include "ClientManager.h"
#include "ace/SOCK_Acceptor.h"


class request;
class response;
enum class MessageType     {Register = 1, Login, Message, Group, Broadcast, Image, File };
enum class RequestStatus   {InvalidPacketLength, InvalidSenderId, InvalidReceiverId, InvalidRequestSpecifier, BlockedSender, BlockedRecipient, Unauthorized, OK };
enum class ResponseType    {Invalid, HeaderError, Blocked, Unauthorized, OK };
enum class ResponseMessage {CantLogin = 1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK, LoginOK,
							GroupCreated, AddedInGroup, AlreadyMember,PrivateGroup, MemberList, GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers,
							InvalidSpecifier, Unauthorized,AlreadyLoggedIn, Logout, Custom = 255};

class ProtocolManager {
	enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
public:
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

	//const char *recipient; // either define this or like above - in this case you can assign value as calling spot.  
	int length;
	char* buffer;
	// read this for issues with chars array

	~request() {
		delete[] buffer;
	}
};

class requesthelper {
public:
	static int request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len);

};

class response {
public:
	ACE_SOCK_Stream* socket;
	char type;
	char *length;
	char *buffer;
	//dont initialize char pointer with nullptr. instead try to use new keyword before using it 

};

class responsehelper {
public:
	static void getlength(char ** buffer, int length);
	static std::string parseresponse(response &resp, std::string payload, bool string = true);
};