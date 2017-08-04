#pragma once 
#include "MessageProtocol.h"
 int requesthelper::request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len){
	char* buffer = new char[len];
	container = "";
	buffer[len] = '\0';
	int read = _peer.recv_n(buffer, len);
	if (read == len) {
		container += buffer;
		//delete buffer;
		return 1;
	}
	else {
	  //delete buffer;
		return 0;
	}
}
 

 void responsehelper::getlength(char ** buffer, int length) {
	 std::stringstream ss;
	 ss << std::setw(4) << std::setfill('0') << length;
	 *buffer = _strdup(ss.str().c_str());
 }

 std::string responsehelper::parseresponse(response &resp, std::string payload, bool string) {
	 std::stringstream ss;
	 std::stringstream length;
	 int size; std::string len;
	 auto tempPayload = payload;

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
		 case ResponseMessage::AlreadyMember:
			 resp.buffer = "You're Already member of it.";
			 getlength(&resp.length, strlen(resp.buffer));
			 break;
		 case ResponseMessage::UsernamePasswordMismatch:
			 resp.buffer = "Username & Password Dont Match, Think Again!";
			 getlength(&resp.length, strlen(resp.buffer));
			 break;
		 case ResponseMessage::RegisterOK:
			 resp.buffer = _strdup(payload.c_str());
			 size = strlen(resp.buffer);
			 getlength(&resp.length, size);
			 break;
		 case ResponseMessage::Notfound:
			 resp.buffer = "User with credentials not found!";
			 getlength(&resp.length, strlen(resp.buffer));
			 break;
		 case ResponseMessage::ClientOffline:
			 resp.buffer = "Ahh, You're late, This user has just gone!";
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
			 resp.buffer = "Okay. You Added";
			 size = strlen(resp.buffer);
			 getlength(&resp.length, size);
			 break;
		 case ResponseMessage::PrivateGroup:
			 resp.buffer = "Ask Admin to add you";
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
		 case ResponseMessage::AlreadyLoggedIn:
			 resp.buffer = "Already LoggedIn";
			 getlength(&resp.length, strlen(resp.buffer));
			 break;
		 default:
			 break;
		 }
	 }
	 ss << resp.type << resp.length << resp.buffer;
	 return ss.str();
 }

 