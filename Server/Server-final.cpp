#include "ace/Reactor.h"
#include <iostream>
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include <string>
#include <cctype>
#include "ace/OS.h"
#include "ClientManager.h"
#include "MessageProtocol.h"
#include "QueueManager.h"
#include "User.h"

class MyServiceHandler; //forward declaration

char id = 'A';
std::string ids = "A";

typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;

enum RequestTypes {Register=1, Login, Message, CreateGroup, MyGroupList, JoinGroup, MemberList, ActiveUsers, Broadcast, Logout=100 };
//enum ActiveGroup { TechTalks = 1, Foodbar, CricketInsighter, Custom };
//enum MessageReceiver { User=1, Group, Offline, Unknown };

class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
	ACE_thread_t thread_names[1];
	Connection con;
public:
	int open(void*) {
		__peer = peer();
		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
		activate(THR_NEW_LWP,1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this, 0, 0, 0, thread_names);

		// peer().send_n("It works", sizeof("It works"));
		return 0;
	}

	void read(void) {
		std::string container; int status; request r;
		std::vector<Connection> conVector;

		status = requesthelper::request_reader(__peer, container, 1);
		if (container == "") {
			return;
		}
		// int -> 0-255 (ALT + 1)
		// char -> 0-9 
		int _type = (int)container[0]; // char - unsigned char
		response _response; 
		response _forward;
		
		userauthenticationstatus accountSts;
		groupauthentication groupSts;
		std::string __response = "";
		std::string buffer = "";
		std::string content; // buffer for user and group message
		std::string username, password, groupname;
		std::string sender = "", recipient = ""; 
		std::string buffResp;
		std::string members;
	//	std::vector<Connection> conVector;
		switch ((RequestTypes)_type)
		{
		case Register:
			status = requesthelper::request_reader(__peer, container, 10);
			username= container;
			status = requesthelper::request_reader(__peer, container, 10);
			password= container;
			//userauthenticationstatus accountSts; 	std::string username;
			accountSts = UserManager::registerUser(username, password);
			if (accountSts != userauthenticationstatus::OK) {
				_response.type = (int)(ResponseMessage::ExistAlready);
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
			}
			else {
				_response.type = (int)(ResponseMessage::RegisterOK);
				Connection con(id, &this->peer_, UserManager::getsenderId(username));
				ClientManager::addconnection(con);
				responsehelper::parseresponse(_response, con.userid, false);
				_response.socket = con.socket;
				QueueManager::addresponse(_response);
			}
			break;
		case Login:
			status = requesthelper::request_reader(__peer, container, 10);
			username= container;
			status = requesthelper::request_reader(__peer, container, 10);
			password= container;
			//userauthenticationstatus accountSts;
			//accountSts = UserManager::authenticateUser(buffer.substr(0, 9), buffer.substr(10, 19));
			accountSts = UserManager::authenticateUser(username,password);
			if (accountSts == userauthenticationstatus::OK){
				_response.type = (int)ResponseMessage::LoginOK;
				con = Connection(id, &this->peer_, UserManager::getsenderId(username));
				ClientManager::addconnection(con);
				_response.socket = con.socket;
				responsehelper::parseresponse(_response, con.userid, false);
				
			}
			else if (accountSts == userauthenticationstatus::UsernamePasswordMismatch)
			{
				// User not found or password mismatch
				_response.type = (int)(ResponseMessage::UsernamePasswordMismatch);
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);

			}
			else if (accountSts == userauthenticationstatus::UserNotfound) {
			_response.type = (int)(ResponseMessage::Notfound);
			_response.socket = &__peer;
			responsehelper::parseresponse(_response, "", false);
			}
			QueueManager::addresponse(_response);
			break;// 3Amessage
		case Message: 
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return;
			}

			requesthelper::request_reader(__peer, container, 6);
			sender = container;
			requesthelper::request_reader(__peer, container, 6); // u00001
			recipient = container;
			if (container[0] == 'u' || container[0] == 'U') {
				// username = content;
				_response.type = (int)(ResponseMessage::UserMessage);
				_forward.type = (int)(ResponseMessage::UserMessage);
				requesthelper::request_reader(__peer, container, 4);
				char* msgLength = (char*)container.c_str();
				int length = std::stoi(msgLength);
				for (auto _con : ClientManager::connections) {
					if (_con.userid == recipient.c_str()) {
						requesthelper::request_reader(__peer, container, length); // limit of a text
						content += container;
						///send call
						_forward.socket = _con.socket;
						_response.socket = &__peer;
						responsehelper::parseresponse(_forward, content, false);
						responsehelper::parseresponse(_response, "Okay", false);
						QueueManager::addresponse(_forward);
						QueueManager::addresponse(_response);
						break;
					}
				}
			}
			else if (container[0] == 'g' || container[0] == 'G') { // 3u00001g
				
				requesthelper::request_reader(__peer, container, 4);
				requesthelper::request_reader(__peer, container, std::stoi(container));
				
				if (!GroupManager::groupExists(recipient))
				{
					_response.type = (int)(ResponseMessage::GroupNotFound);
					_response.socket = &__peer;
					responsehelper::parseresponse(_response, "", false);
					QueueManager::addresponse(_response);
					return;
				}
				GroupManager::getconnections(recipient, conVector);
				for (auto s : conVector) {
					_forward.type = (int)(ResponseMessage::GroupMessage);
					_forward.socket = s.socket;
					responsehelper::parseresponse(_forward, container, false);
					QueueManager::addresponse(_forward);

				}
				_response.socket = &__peer;
				_response.type = (int)(ResponseMessage::GroupMessage);
				responsehelper::parseresponse(_response, "Message Sent to Group", false);
				QueueManager::addresponse(_response);

					}
			break;
		case CreateGroup: 
			// 4u00001gtecht (do something that useer wont need to wrte this first g instead we can embed it ourselves)
			// 40010somename
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return;
			}
			requesthelper::request_reader(__peer, container, 4);
			buffer = container;
			requesthelper::request_reader(__peer, container, std::stoi(buffer)); // u00001
			groupname = container;
			groupSts = GroupManager::creategroup(groupname);

			if (groupSts == groupauthentication::Exist){
				_response.type = (int)(ResponseMessage::ExistAlready);
				_response.socket = &__peer;
				buffer = "Group already exists.";
			}
			else if (groupSts == groupauthentication::Created) {
				_response.type = (int)(ResponseMessage::GroupCreated);
				_response.socket = &__peer;
				
				buffer = GroupManager::getGroupId(groupname);
		}
			responsehelper::parseresponse(_response, buffer, false);
			QueueManager::addresponse(_response);
			break;
		case JoinGroup: // 6u00001gtecht
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return;
			}
			requesthelper::request_reader(__peer, container, 6);
			sender = container;
			requesthelper::request_reader(__peer, container, 6); // u00001
			groupname = container;
		    groupSts = GroupManager::joinGroup(groupname, con); // all  connections 
			if (groupSts == groupauthentication::Added) {
				_response.type = (int)(ResponseMessage::AddedInGroup);
				_response.socket = &__peer;
				buffer = "Okay- You Added";
			 }
			else if (groupSts == groupauthentication::Exist) {
				// you are already part of it 
				_response.type = (int)(ResponseMessage::ExistAlready);
				buffer = "Already exists.";
			}
			responsehelper::parseresponse(_response, buffer, false);
			QueueManager::addresponse(_response);
			break;
		case MemberList: // show active clients in this group.      7gtecht
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return;
			}
			requesthelper::request_reader(__peer, container, 6); // u00001
			groupname = container;

			if (!GroupManager::groupExists(groupname)) {
				_response.type = (int)(ResponseMessage::GroupNotFound);
				_response.socket = &__peer;
				buffer = "Group not found";
				responsehelper::parseresponse(_response, buffer, false);
				QueueManager::addresponse(_response);

				return;
			}
			GroupManager::getconnections(groupname, conVector);
			if (conVector.size() == 0) {

			//	buffer = "no members!";
				_response.type = (int)(ResponseMessage::NoMembers);
				_response.socket = &__peer;
				buffer = members;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return;
			}
			for (auto _ : conVector) {
				members += _.userid + ",";
			}
			_response.type = (int)(ResponseMessage::MemberList);
			_response.socket = &__peer;
			buffer = members;
			responsehelper::parseresponse(_response, buffer, false);
				QueueManager::addresponse(_response);
			break;

		case ActiveUsers: // 8 --> ?0050hefriowejrwoimc
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return;
			}
			buffer = "";
			for (auto _ : ClientManager::connections) {
				buffer += _.userid + ",";
			}

			_response.type = (int)ResponseMessage::ActiveUsers;
			_response.socket = &__peer;
			responsehelper::parseresponse(_response, buffer, false);
			QueueManager::addresponse(_response);
			break;
		case Logout:
			con.userid = "";
			ClientManager::removeconnection(con.id);
			GroupManager::removeFromAll(con.userid);
			_response.socket = &__peer;
			_response.type = (int)ResponseMessage::Logout;
			responsehelper::parseresponse(_response, "", false);
			QueueManager::addresponse(_response);
			
			ACE_OS::sleep(3);
			con.socket = nullptr;
			break;
		default:
			_response.type = (int)ResponseMessage::InvalidSpecifier;
			_response.socket = &__peer;
			responsehelper::parseresponse(_response, "", false);
			QueueManager::addresponse(_response);
			break;
		}
	}
	static void write(void) {
		while (1) {
			// Continue to loop
			response res;
			if (QueueManager::getresponse(res) == 1) {
				std::string responseBuffer = responsehelper::parseresponse(res, "", true);
			//	printf("%s", responseBuffer);

				res.socket->send_n(responseBuffer.c_str(), responseBuffer.length());
				
				//__peer.send_n(responseBuffer.c_str(), 3);
				ACE_DEBUG((LM_DEBUG, "(%t) QueueThread \n"));
			}
		}
	}

	int svc(void) {
		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
		if (ACE_Thread::self() == thread_names[0]) {
			while (1) {
				read();
			}
		}
		return 0; // keep the compiler happy.
	}

private:
	ACE_SOCK_Stream __peer;
	bool readytowrite = false;
	std::string buffer;
};

// Helper function
void addData() {
	UserManager::registerUser("username00", "password00");
	UserManager::registerUser("username11", "password11");
	UserManager::registerUser("username22", "password22");
	UserManager::registerUser("username33", "password33");
	
	GroupManager::creategroup("FFooBBar");
	GroupManager::creategroup("Fruits");
	GroupManager::creategroup("Cars");


}

int main(int argc, char* argv[]) {
	ACE_INET_Addr addr(50009);
	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main \n"));
	
	Acceptor myacceptor(addr, Reactor::instance());
	ACE_Thread::spawn((ACE_THR_FUNC)MyServiceHandler::write);
	GroupManager::defaultgroups(); /// will load default groups

	addData();
	while (1)
		Reactor::instance()->handle_events();
	return 0;
}

