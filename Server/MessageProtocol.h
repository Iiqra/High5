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
 enum class ResponseMessage { CantLogin=1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK,LoginOK, 
	GroupCreated, AddedInGroup,MemberList,GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers, 
	 InvalidSpecifier, Unauthorized, Logout, Custom=255 };



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
	
	static void getlength(char ** buffer, int length) {
		std::stringstream ss;
		ss << std::setw(4) << std::setfill('0') << length;

		//buffer = (char*)ss.str().c_str();
		*buffer = _strdup(ss.str().c_str());
	}

	//static std::string parseresponse1(response &r) {
	//	// here.
	//	std::stringstream ss;
	//	ss << r.type << r.length << r.buffer;
	//	return ss.str();
	//}
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
				resp.buffer = "This usernane or group  already Exist, Try Somthing Better!";
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
			case ResponseMessage::GroupNotFound:
				resp.buffer = "Group not found!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::NoMembers:
				resp.buffer = "No members yet!";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::InvalidSpecifier:
				resp.buffer = "Invalid Specifier";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::Unauthorized:
				resp.buffer = "Unauthorized";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::Logout:
				resp.buffer = "Your session will be closed in 1-4 seconds.";
				getlength(&resp.length, strlen(resp.buffer));
				break;
			case ResponseMessage::Custom:
				break;
			case ResponseMessage::UserMessage:
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::GroupCreated:
				resp.buffer = _strdup(payload.c_str()); // how send the paylload -groupid
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::AddedInGroup:
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::GroupMessage:
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::MemberList:
				resp.buffer = _strdup(payload.c_str());
				size = strlen(resp.buffer);
				getlength(&resp.length, size);
				break;
			case ResponseMessage::ActiveUsers:
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



