// This is the main DLL file.

#include "stdafx.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\15August\unamanaged\myprotocol.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\15August\unamanaged\myprotocol.cpp"
#include <msclr\marshal_cppstd.h>
using namespace System;

namespace managedcode {

	public enum class MessageTypeManaged { Register = 1, Login, Message, Group, Broadcast, Image, File };
	public enum class ResponseMessageManaged {
		CantLogin = 1, ExistAlready, UsernamePasswordMismatch, Notfound, RegisterOK, LoginOK,
		GroupCreated, AddedInGroup, AlreadyMember, PrivateGroup, MemberList, GroupNotFound, NoMembers, ClientOffline, UserMessage, GroupMessage, ActiveUsers,
		InvalidSpecifier, InvalidToorFrom, Unauthorized, AlreadyLoggedIn, Logout, Custom = 255
	};
	public ref class RequestManaged {
	public:
	 void SetFields(MessageTypeManaged mtw, String^ sender, String^ recipient, String^ buffer, int len) {
			req->type = MessageType((int)mtw);
			std::string _sender = msclr::interop::marshal_as<std::string>(sender);
			strcpy_s(req->sender, _sender.c_str());
			std::string _recipient = msclr::interop::marshal_as<std::string>(recipient);
			strcpy_s(req->recipient, _recipient.c_str());
			std::string _buffer = msclr::interop::marshal_as<std::string>(buffer);
		//	strcpy_s(req->buffer, _buffer.c_str());
			req->length = len;
		}

		String^ GetString() {
			System::Text::StringBuilder^ builder = gcnew System::Text::StringBuilder();
			builder->Append((int)req->type);
			return "";
		}

	private:
		request* req;
	};
	public ref class RequestHelperManaged
	{
	public: 
		static void SendRequest(System::Net::Sockets::TcpClient^ client, String^ content) {
			array<unsigned char>^ _buffer = System::Text::Encoding::ASCII->GetBytes(content);
			client->GetStream()->Write(_buffer, 0, _buffer->Length);
		}
	};
	public ref class ResponseManaged {
		void SetFields(String^ type, String^ length, String^ buffer) {
			std::string _type = msclr::interop::marshal_as<std::string>(type);
			std::string _length = msclr::interop::marshal_as<std::string>(length);
			std::string _buffer = msclr::interop::marshal_as<std::string>(buffer);
		//	strcpy_s(res->buffer, _length.c_str());
		//	strcpy_s(res->buffer, _buffer.c_str());
			res->type = _type[0];
		}

	private:
		response *res;
	};
	public ref class ResponseHelperManaged
	{
	public:
		void GetLength(char ** buffer, int length) {}
		static void ParseResponse(ResponseManaged^ resp, String^ payload, bool string) {

		}

	private:
		responsehelper *resh;
	};

}