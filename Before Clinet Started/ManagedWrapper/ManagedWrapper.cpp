#include "stdafx.h"
#include <msclr\marshal_cppstd.h>
#include <string>
#include "MsgProtocol.h"
using namespace abc;
using namespace System;

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
			strcpy(req->sender, _sender.c_str());
			std::string _recipient = msclr::interop::marshal_as<std::string>(recipient);
			strcpy(req->recipient, _recipient.c_str());
			std::string _buffer = msclr::interop::marshal_as<std::string>(buffer);
			strcpy(req->buffer, _buffer.c_str());
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
