// ManagedWrapper.h

#pragma once
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\Dll\Dll\MsgProtocol.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\Dll\Dll\MsgProtocol.cpp"

using namespace System;

namespace ManagedWrapper {

	enum  class  MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
	enum  class  ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK };
	enum  class  ResponseMessage {
		CantLogin = 1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK, LoginOK,
		GroupCreated, AddedInGroup, AlreadyMember, PrivateGroup, MemberList, GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers,
		InvalidSpecifier, InvalidToorFrom, Unauthorized, AlreadyLoggedIn, Logout, Custom = 255
	};
	public ref class requestWrapper
	{
	public:
		MessageType type;
		//array <char>^ sender
		array<Byte>^ sender; //= gcnew array<char>(5);
		array<Byte>^ recipient;// = gcnew array<char>(5);
		int length;
		char* buffer;
	private:
	//	static request *reqObj;
		request *reqObj;
		 
	//public:
	//	requestWrapper() : reqObj(new request) {}
	//	~requestWrapper() { delete reqObj; }
		
	};
	public ref class responseWrapper
	{
	public:
		ACE_SOCK_Stream* socket;
		char type;
		char *length;
		char *buffer;
	};
	public ref class requesthelperWrapper
	{
	public:
		
	private: 
		
	};
	public ref class responsehelperWrapper
	{
		static void getlength(char ** buffer, int length);
	};
}


/*https://stackoverflow.com/questions/2138473/how-do-i-specify-a-fixed-size-buffer-in-c-cli*/
/*https://stackoverflow.com/questions/16490098/mixed-types-are-not-supported*/