#pragma once
#include<string>
#include<vector>
#include <ctime>
#include <string>
#include <sstream>
#include "User.h"
#include "ClientManager.h"

class request;
class response;
 enum class MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
 enum class RequestStatus { InvalidPacketLength, InvalidSenderId, InvalidReceiverId, InvalidRequestSpecifier, BlockedSender, BlockedRecipient,Unauthorized, OK};
 enum class ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK};



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
 //enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
class request {
public:
	MessageType type;
	char sender[6];
	char recipient[6];

	//const char *recipient; // either define this or like above - in this case you can assign value as calling spot.  
	int length;
	char* buffer;
	// read this for issues with chars array
};


class requesthelper {
public:

     static RequestStatus validate_request(request& r) {
		    //if (r.type != MessageType::Login || r.type != MessageType::Register || r.type != MessageType::Message || r.type != MessageType::Broadcast || r.type != MessageType::Group)
			// return RequestStatus::InvalidRequestSpecifier;

		 if (r.length != (unsigned)strlen(r.buffer))
			 return RequestStatus::InvalidPacketLength;
		 
		if ((unsigned)strlen(r.sender) != 6 /* or sender wont be in DB #DBCALL*/)
			 return RequestStatus::InvalidSenderId;

		 //if (r.sender is in blocked list maintained at clientmanager side then this )
		 //return RequestStatus::BlockedSender;

		 ////if (r.recepient is in blocked list maintained at clientmanager side then this )
		 //return RequestStatus::BlockedRecipient;

		 //return RequestStatus::Unauthorized;
		return RequestStatus::OK;
	}

	static void process_message(const request& r, RequestStatus status, response &resp) {
		printf("\nInside process message");
		
		if (status == RequestStatus::OK) {
		}
				// UserHelper login
			//case Message:
			//	auto cons = ClientManager::connections;

			//	for (auto con : cons) {
			//		if (r.recipient == con.userid) {
			//			auto str = responsehelper::parseresponse(resp);
			//			con.socket.send_n(str, str.size());
			//		}
			//	}
			//	break;
			//case Group: 
			//	auto cons = ClientManager::connections;

			//	std::vector<User> users = DbHelper::groupclientinfo();
			//	for (auto us : users) {
			//		for (auto con : cons) {
			//			if (us.userid == con.userid) {
			//				auto str = responsehelper::parseresponse(resp);
			//				con.socket.send_n(str, str.size());
			//			}
			//		}
			//	}
			//	break;
			//case Broadcast:
			//	// 1. Get client sockets
			//	// 2. Tell DbHelper to log all msgs -- broadcasts go to all
			//	// 3. Send message to online people. 
			//	auto cons = ClientManager::connections;
			//	for (auto con : cons) {
			//		con.stream.send_n();
			//	}
			//	break;
			//	// Others		
			
	
		//else if (status == RequestStatus::BlockedSender) {
		//	//resp.type == ResponseType::Blocked;
		//}
		//else if (status == RequestStatus::Invalid || status == RequestStatus::HeaderError) {
		//	// LOg the information
		//}
		//else if (status == RequestStatus::Unauthorized) {
		//	// Log unauth access
		//}
	}

	/*static request* createrequest(MessageType type, std::string sender, std::string recipient) {
		// switch (type)
		{
		case Register:
			break;
		case Login:
			break;
		case Message:
			break;
		case Group:
			break;
		case Broadcast:
			break;
		case Image:
			break;
		case File:
			break;
		default:
			break;
		}
	}*/

//	static RequestStatus parseheader(std::string str, request &r) {
	static void parseheader(char str[16], request &r) {

		//senderID
		int hIndex = 0, vIndex = 0;
		r.sender[vIndex++] = str[hIndex++];
		r.sender[vIndex++] = str[hIndex++];
		r.sender[vIndex++] = str[hIndex++];
		r.sender[vIndex++] = str[hIndex++];
		r.sender[vIndex++] = str[hIndex++];
		r.sender[vIndex++] = str[hIndex++];
		//RecipientID
		vIndex = 0;
		r.recipient[vIndex++] = str[hIndex++];
		r.recipient[vIndex++] = str[hIndex++];
		r.recipient[vIndex++] = str[hIndex++];
		r.recipient[vIndex++] = str[hIndex++];
		r.recipient[vIndex++] = str[hIndex++];
		r.recipient[vIndex++] = str[hIndex++];

		//length
		auto length = new char[4];
		length[0] = str[hIndex++]; // 13 ,14,15,16
		length[1] = str[hIndex++];
		length[2] = str[hIndex++];
		length[3] = str[hIndex++];
		r.length = std::stoi(length);
	}

	static std::string parserequest(request& r) {
		std::stringstream str;
		switch (r.type)
		{
		case MessageType::Register:
			str << "1";
			str << "0020";
			str << r.buffer;
			break;
		case MessageType::Login:
			str << "2";
			str << "0020";

			str << r.buffer;
			break;
		case MessageType::Message:
			str << "3";
			str << r.sender;
			str << r.recipient;
			str << r.length;
			str << r.buffer;
			break;
		case MessageType::Group:

			str << "4";
			str << r.sender;
			str << r.recipient;
			str << r.length;
			str << r.buffer;


			break;
		case MessageType::Broadcast:
			str << "5";
			str << r.sender;
			str << r.recipient;
			str << r.length;
			str << r.buffer;
			break;
		case MessageType::Image:
			break;
		case MessageType::File:
			break;
		default:
			break;
		}
		return str.str();
	}
};

// make
class response {
public:
	char type;
	char *length;
	char *buffer;
};

class responsehelper {
public:
	static response* createresponse(RequestStatus status) {
		response *r = new response();
		/*switch (status)
		{
		case RequestStatus::Invalid:
			r->type = ResponseType::Invalid;
			break;
		case RequestStatus::HeaderError:
			r->type = ResponseType::HeaderError;
			break;
		case RequestStatus::Blocked:
			r->type = ResponseType::Blocked;
			break;
		case RequestStatus::Unauthorized:
			r->type = ResponseType::Unauthorized;
			break;
		case RequestStatus::OK:
			r->type = ResponseType::OK;
			break;
		default:
			break;
		}*/
		return r;
	}

	static std::string parseresponse(response &r) {
		// here.
		std::stringstream ss;
		ss << r.type << r.length << r.buffer;
		return ss.str();
	}
};


// 17, parse, validate, parse, install sql , separate project  -- output would be dll. connect,send request, and other client side functions, jjust taske input from cleint and how server will take it.