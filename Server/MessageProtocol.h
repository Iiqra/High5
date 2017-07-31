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
 enum class MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
 enum class RequestStatus { InvalidPacketLength, InvalidSenderId, InvalidReceiverId, InvalidRequestSpecifier, BlockedSender, BlockedRecipient,Unauthorized, OK};
 enum class ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK};
 enum class ResponseMessage { CantLogin=1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK,LoginOK, ClientOffline, UserMessage, GroupMessage, Custom=255 };



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
//	static int registerHandler(ACE_SOCK_Stream stream,request r) {
//		printf("|Register Request|");
//		r.type = MessageType::Register;
//		r.buffer = new char[20];
//		stream.recv_n(r.buffer, 20);
//#pragma region parsingUsernamePassword
//		char username[10];
//		char password[10];
//		int bIndex = 0, vIndex = 0;
//		username[vIndex++] = r.buffer[bIndex++];// 0-1
//		username[vIndex++] = r.buffer[bIndex++];// 1-2
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		username[vIndex++] = r.buffer[bIndex++];
//		vIndex = 0;
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++];
//		password[vIndex++] = r.buffer[bIndex++]; 
//		char u[11];
//		char p[11];
//		bIndex = 0, vIndex = 0;
//		u[vIndex++] = username[bIndex++];// 0-11
//		u[vIndex++] = username[bIndex++];// 1-12
//		u[vIndex++] = username[bIndex++];// 2- 13
//		u[vIndex++] = username[bIndex++];
//		u[vIndex++] = username[bIndex++];
//		u[vIndex++] = username[bIndex++];
//		u[vIndex++] = username[bIndex++];
//		u[vIndex++] = username[bIndex++];
//		u[vIndex++] = username[bIndex++];
//		u[vIndex++] = username[bIndex++]; // 9- 20
//		u[vIndex++] = '\0';
//		vIndex = 0; bIndex = 0;
//		p[vIndex++] = password[bIndex++];// 0-1
//		p[vIndex++] = password[bIndex++];// 1-2
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];
//		p[vIndex++] = password[bIndex++];// 9-10
//		p[vIndex++] = '\0';
//		vIndex = 0;
//
//#pragma endregion
//		userauthenticationstatus accountSts;
//		response res;
//		accountSts = UserManager::registerUser(std::string(u), std::string(p));
//
//		if (accountSts == userauthenticationstatus::OK) {
//			// Login
//			accountSts = UserManager::authenticateUser(std::string(u), std::string(p));
//			if (accountSts == userauthenticationstatus::OK) {
//				res.type = 1;
//				std::string userId = UserManager::getsenderId(u);
//				std::string message = userId + "Registered";
//				//res.message = (char*)message.c_str();
//				res.buffer = (char*)message.c_str();
//				res.length = "0006";
//				std::string responseStr = responsehelper::parseresponse(res);
//
//				readytowrite = true;
//				buffer = responseStr;
//			}
//		}
//		return 0;
//	}
//	static int loginHandler(int type, request r) {
//			printf("|Login Request|");
//			r.type = MessageType::Register;
//		
//		return 0;
//		}
//	static int messageHandler(int type, request r) {
//			printf("|Message Request|");
//			r.type = MessageType::Register;
//		
//		return 0;
//	}
//

	/*
	char* buffer = new char[20];
	buffer[20] = '\0';
	request_reader(__peer, buffer, 10);

	// parse username/password

	*/

	static int request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len ){
		char* buffer = new char[len];
		container = "";
		buffer[len] = '\0';
		int read = _peer.recv_n(buffer, len);
		if (read == len) {
			container += buffer;
			return 1;
		} else{
			return 0;
		}
	}


	static void authentication_parser(char * buff, request & r) {

		
	}
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
		try {
			r.length = std::stoi(length);
		}
		catch (std::exception e){
			// Setting the length to 0 --> other end will know 0 size means something went wrong.
			r.length = 0;
		}
	}
	// 2mnts plzz
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
	ACE_SOCK_Stream* socket;
	char type;
     char *length;
	 char *buffer;
	//dont initialize char pointer with nullptr. instead try to use new keyword before using it 

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

	static void getlength(char ** buffer, int length) {
		std::stringstream ss;
		ss << std::setw(4) << std::setfill('0') << length;

		//buffer = (char*)ss.str().c_str();
		*buffer = _strdup(ss.str().c_str());
	}

	/*static void generateFields(response& res, std::string payload) {
		switch (ResponseMessage(res.type))
		{
		case ResponseMessage::LoginOK:
			res.buffer = "OK";
			getlength(res.length, sizeof(res.buffer));
			break;
		}
	}
	*/
	static std::string parseresponse(response &r) {
		// here.
		std::stringstream ss;
		ss << r.type << r.length << r.buffer;
		return ss.str();
	}
	static std::string parseresponse(response &resp, std::string payload, bool string= true) {
		std::stringstream ss;
		std::stringstream length;
		int size; std::string len;
		auto tempPayload = payload;
		/*resp.buffer = new char;
		resp.length = new char;*/
		if (!string) {
			switch (ResponseMessage(resp.type))
			{
			case ResponseMessage::LoginOK:

				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::CantLogin:
				resp.buffer = "Cant login, Try Again!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::ExistAlready:
				resp.buffer = "This usernane already Exist, Try Somthing Else!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::UsernamePasswordMismatch:
				resp.buffer = "Username & Password Dont Match, Think Again!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::RegisterOK:

		     /*	resp.buffer = "successful registration!";
				resp.length = "00006";*/
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);				
				getlength(&resp.length, size);
				break;
			case ResponseMessage::Notfound:
				resp.buffer = "User with credentials not found!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::ClientOffline:
				resp.buffer = "Recipient Is Offline!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::Custom:
				break;
			case ResponseMessage::UserMessage:
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::GroupMessage:
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			default:
				break;
			}
		}
		ss << resp.type << resp.length << resp.buffer;
		return ss.str();
	}

	static void writer(ACE_SOCK_Stream & _peer, std::string response) {

		_peer.send_n(response.c_str(), sizeof(response));
	}
};
// 1username33password55 --- 50006u00001 



