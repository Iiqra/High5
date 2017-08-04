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
static bool logout = false;
static int instance = 1;
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;

//enum RequestTypes {Register=1, Login, Message, CreateGroup, MyGroupList, JoinGroup, MemberList, ActiveUsers, Broadcast, Logout=100 };
enum RequestTypes { Register = 1, Login, Message, CreateGroup, JoinGroup, MemberList, ActiveUsers, Broadcast, Logout };

class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
	ACE_thread_t thread_names[1];
	Connection con;
public:
	int open(void*) {
		__peer = peer();
		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));


	/*	activate(THR_NEW_LWP, 1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this,
			0, 0, 0, thread_names);*/

			Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
		int a = instance++;
		peer().send_n(std::to_string(a).c_str(), 1);
		 peer().send_n("It works\n ", sizeof("It works\n")); 
		return 0;
	}
	int _type;
    int handle_input(ACE_HANDLE)
	{
		printf("ACE_Event_Handler::handle_input");
		//ACE_TRACE();
		std::string container; int status; request r;
		std::vector<Connection> conVector;
		response _response;
		userauthenticationstatus accountSts;
		std::string __response = "";
		std::string buffer = "";
		std::string username, password;
		std::string type;
		status = requesthelper::request_reader(__peer, type, 1);
		if (type == "" || (type < "1" || type > "9")) {
			// __peer.send_n("Try something useful", sizeof ("Try something useful")); cus of telnet single packet reading behaviour em niot orintnig this message here....
			return 0;
		}

		// 1,2,3,4,5,6 = 1,2 let handle inp hadle it ... 

		_type = std::stoi(type);
		if (_type== 1 || _type== 2) // reg or login
		{
			switch ((RequestTypes)_type)
			{
			case Register:
				status = requesthelper::request_reader(__peer, container, 10);
				username = container;
				status = requesthelper::request_reader(__peer, container, 10);
				password = container;
				//userauthenticationstatus accountSts; std::string username;
				accountSts = UserManager::registerUser(username, password);
				if (accountSts != userauthenticationstatus::OK) {
					_response.type = (int)(ResponseMessage::ExistAlready);
					_response.socket = &__peer;
					responsehelper::parseresponse(_response, "", false);
					QueueManager::addresponse(_response);
				}
				else {
					_response.type = (int)(ResponseMessage::RegisterOK);
					con = Connection(id, &this->peer_, UserManager::getsenderId(username));
					ClientManager::addconnection(con);
					// here.
					responsehelper::parseresponse(_response, con.userid, false);
					_response.socket = con.socket;
					QueueManager::addresponse(_response);
				}
				break;
			case Login:
				if (con.userid != "") {
					// send the response

					_response.type = (int)ResponseMessage::AlreadyLoggedIn;
					_response.socket = &__peer;
					responsehelper::parseresponse(_response, "", false);
					QueueManager::addresponse(_response);
					return 0;
				}
				status = requesthelper::request_reader(__peer, container, 10);
				//username= container;
				username = container;
				status = requesthelper::request_reader(__peer, container, 10);
				password = container;
				//userauthenticationstatus accountSts;
				//accountSts = UserManager::authenticateUser(buffer.substr(0, 9), buffer.substr(10, 19));
				accountSts = UserManager::authenticateUser(username, password);
				if (accountSts == userauthenticationstatus::OK) {
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
				break;
				return 0;
			} // swicth close
		} //end of if 1 or 2 

		else { // any other request
			activate(THR_NEW_LWP, 1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this,
				0, 0, 0, thread_names);
		}
		return 0;
	}
	//3u00003u000060003hey

	int read(int t) {

#pragma region initialization

		std::string container2; int status; request r;
		std::vector<Connection> conVector;
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
		bool isPrivate;
		std::string grpadminid;
#pragma endregion 
		
		switch ((RequestTypes)_type)
		{
		case Message:
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
			}
			container2 = "";
			requesthelper::request_reader(__peer, container2, 6);
			sender = container2; //u00001
			requesthelper::request_reader(__peer, container2, 6); // u00001
			recipient = container2; //u00003
			if (recipient[0] == 'u' || recipient[0] == 'U') {
				// username = content;
				_response.type = (int)(ResponseMessage::UserMessage);
				_forward.type = (int)(ResponseMessage::UserMessage);
				container2.clear();// 0003 003h
				requesthelper::request_reader(__peer, container2, 4);
				char* msgLength = (char*)container2.c_str();
				int length = std::stoi(msgLength);
				for (auto _con : ClientManager::connections) {
					if (_con.userid == recipient.c_str()) {
						container2 = "";
						requesthelper::request_reader(__peer, container2, length); // limit of a text
						content += container2;
						///send call
						_forward.socket = _con.socket;
						_response.socket = &__peer;
						responsehelper::parseresponse(_forward, content, false);
						responsehelper::parseresponse(_response, "Okay", false);
						QueueManager::addresponse(_forward);
						QueueManager::addresponse(_response);
					}
				} break;
				_response.type = (int)(ResponseMessage::ClientOffline);
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
				break;
			}
			else if (container2[0] == 'g' || container2[0] == 'G') { // 3u00001g
				requesthelper::request_reader(__peer, container2, 4);
				requesthelper::request_reader(__peer, container2, std::stoi(container2));
				if (!GroupManager::groupExists(recipient))
				{
					_response.type = (int)(ResponseMessage::GroupNotFound);
					_response.socket = &__peer;
					responsehelper::parseresponse(_response, "", false);
					QueueManager::addresponse(_response);
					return 0;
				}
				GroupManager::getconnections(recipient, conVector);
				for (auto s : conVector) {
					if (s.userid == con.userid) {
						continue;
					}
					_forward.type = (int)(ResponseMessage::GroupMessage);
					_forward.socket = s.socket;
					responsehelper::parseresponse(_forward, container2, false);
					QueueManager::addresponse(_forward);

				}
				_response.socket = &__peer;
				_response.type = (int)(ResponseMessage::GroupMessage);
				responsehelper::parseresponse(_response, "Message Sent to Group", false);
				QueueManager::addresponse(_response);
			}
			break;
		case CreateGroup:
			// 410010somename // create specifier|private or public| len|groupname
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
			}
			// Read if private or public group
			requesthelper::request_reader(__peer, container2, 1);
			isPrivate = std::stoi(container2);
			requesthelper::request_reader(__peer, container2, 4);
			buffer = container2;
			requesthelper::request_reader(__peer, container2, std::stoi(buffer)); // u00001
			groupname = container2;
			groupSts = GroupManager::creategroup(groupname, con.userid, isPrivate); // 
			if (groupSts == groupauthentication::Exist) {
				_response.type = (int)(ResponseMessage::ExistAlready);
				_response.socket = &__peer;

			}
			else if (groupSts == groupauthentication::Created) {
				_response.type = (int)(ResponseMessage::GroupCreated);
				_response.socket = &__peer;
				buffer = GroupManager::getGroupId(groupname);
			}
			responsehelper::parseresponse(_response, buffer, false);
			QueueManager::addresponse(_response);
			break;
		case JoinGroup: // 5u00001gtecht
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
			}

			requesthelper::request_reader(__peer, container2, 6);
			sender = container2;
			requesthelper::request_reader(__peer, container2, 6); // u00001
			groupname = container2;
			groupSts = GroupManager::joinGroup(groupname, con, sender); // all  connections 
			if (groupSts == groupauthentication::Added) {
				_response.type = (int)(ResponseMessage::AddedInGroup);
				_response.socket = &__peer;

			}
			else if (groupSts == groupauthentication::Exist) {
				// you are already part of it 
				_response.type = (int)(ResponseMessage::ExistAlready);
				_response.socket = &__peer;

			}
			else if (groupSts == groupauthentication::Private) {
				// you are already part of it 
				_response.type = (int)(ResponseMessage::PrivateGroup);
				_response.socket = &__peer;

			}
			responsehelper::parseresponse(_response, "", false);
			QueueManager::addresponse(_response);
			break;
		case MemberList: // show active clients in this group.      6gtecht
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
			}
			requesthelper::request_reader(__peer, container2, 6); // u00001
			groupname = container2;

			if (!GroupManager::groupExists(groupname)) {
				_response.type = (int)(ResponseMessage::GroupNotFound);
				_response.socket = &__peer;
				buffer = "Group not found";
				responsehelper::parseresponse(_response, buffer, false);
				QueueManager::addresponse(_response);

				return 0;
			}
			GroupManager::getconnections(groupname, conVector);
			if (conVector.size() == 0) {

				// buffer = "no members!";
				_response.type = (int)(ResponseMessage::NoMembers);
				_response.socket = &__peer;
				buffer = members;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
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

		case ActiveUsers: // 7 --> ?0050hefriowejrwoimc
			if (con.userid == "") {
				_response.type = (int)ResponseMessage::Unauthorized;
				_response.socket = &__peer;
				responsehelper::parseresponse(_response, "", false);
				QueueManager::addresponse(_response);
				return 0;
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
		case Logout:  // 9
			con.userid = "";
			ClientManager::removeconnection(con.id);
			GroupManager::removeFromAll(con.userid);
			_response.socket = &__peer;
			_response.type = (int)ResponseMessage::Logout;
			responsehelper::parseresponse(_response, "", false);
			QueueManager::addresponse(_response);
			ACE_OS::sleep(3);
			con.socket->close();
			ACE_Thread::cancel(t); // cancel the thread
			ACE_Thread::exit();
			logout = true;
			return -1; // deregister from reactor

			break;
		default:
			_response.type = (int)ResponseMessage::InvalidSpecifier;
			_response.socket = &__peer;
			responsehelper::parseresponse(_response, "", false);
			QueueManager::addresponse(_response);
			break;
		}
		return 0;
	}
	static void write(void) {
		while (1) {
			ACE_OS::sleep(0.3);
			response res;
			if (QueueManager::getresponse(res) == 1) {
				std::string responseBuffer = responsehelper::parseresponse(res, "", true);
				// printf("%s", responseBuffer);

				res.socket->send_n(responseBuffer.c_str(), responseBuffer.length());
				//__peer.send_n(responseBuffer.c_str(), 3);
				ACE_DEBUG((LM_DEBUG, "(%t) QueueThread \n"));
			}
		}
	}

	int svc(void) {
		ACE_thread_t tid = ACE_OS::thr_self();
		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
		//if (ACE_Thread::self() == thread_names[0]) {
		while (1) {
			ACE_OS::sleep(0.1);
			read(tid);
		}
		//}
		return 0;
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

	GroupManager::creategroup("FFooBBar", "u00001", 1);
	GroupManager::creategroup("Fruits", "u00001", 1);
	GroupManager::creategroup("Cars", "u00001", 1);


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


/*


char *a = new char[13];
char *a = new char[1024 * 1024];

delete[] a;


char *i= new char(5);
response obj;

delete i;

response* obj = new response;
response^ obj = gcnew response;
response& obj = *(new response);

response, response& -> gc.

*/
